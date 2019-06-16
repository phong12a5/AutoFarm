#include "JavaCommunication.h"

JavaCommunication* JavaCommunication::m_instance = nullptr;

JavaCommunication::JavaCommunication(QObject *parent) : QObject(parent)
{
    // Do nothing
}

JavaCommunication *JavaCommunication::instance()
{
    if(!m_instance)
        m_instance = new JavaCommunication();
    return m_instance;
}
#ifdef ANDROID_KIT

void JavaCommunication::openApplication(QString packageName, QString activityName) const
{
    LOG << "Openning " << packageName + "/" + activityName;
    QAndroidJniObject activity = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative", "activity", "()Landroid/app/Activity;");   //activity is valid
    if ( activity.isValid() )
    {
        // Equivalent to Jave code: 'Intent intent = new Intent();'
        QAndroidJniObject intent("android/content/Intent","()V");
        if ( intent.isValid() )
        {
            QAndroidJniObject param1 = QAndroidJniObject::fromString(packageName);
            QAndroidJniObject param2 = QAndroidJniObject::fromString(activityName);

            if ( param1.isValid() && param2.isValid() )
            {
                // Equivalent to Jave code: 'intent.setClassName(packageName, activityName);'
                intent.callObjectMethod("setClassName","(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;",param1.object<jobject>(),param2.object<jobject>());

                // Equivalent to Jave code: 'startActivity(intent);'
                activity.callMethod<void>("startActivity","(Landroid/content/Intent;)V",intent.object<jobject>());
            }
        }
    }
}

void JavaCommunication::backToAppMain() const
{
    LOG;
    this->openApplication("org.qtproject.example.AutoTouchQtAndroid","org.qtproject.qt5.android.bindings.QtActivity");
}

QString JavaCommunication::getDeviceIMEI()
{
    QAndroidJniEnvironment env;
    jclass contextClass = env->FindClass("android/content/Context");
    jfieldID fieldId = env->GetStaticFieldID(contextClass, "TELEPHONY_SERVICE", "Ljava/lang/String;");
    jstring telephonyManagerType = (jstring) env->GetStaticObjectField(contextClass, fieldId);

    jclass telephonyManagerClass = env->FindClass("android/telephony/TelephonyManager");
    jmethodID methodId = env->GetMethodID(contextClass, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");

    QAndroidJniObject qtActivityObj = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative",  "activity", "()Landroid/app/Activity;");
    jobject telephonyManager = env->CallObjectMethod(qtActivityObj.object<jobject>(), methodId, telephonyManagerType);

    methodId = env->GetMethodID(telephonyManagerClass, "getDeviceId", "()Ljava/lang/String;");
    jstring jstr = (jstring) env->CallObjectMethod(telephonyManager, methodId);

    return QAndroidJniObject::fromLocalRef(jstr).toString();
}

QString JavaCommunication::getGoogleSF(){

    /*Cursor query = context.getContentResolver().query(sUri, null, null, new String[] { "android_id" }, null); */

    // Get ContentResolver: context.getContentResolver()
    LOG << "Get ContentResolver";
    QAndroidJniObject qtActivityObj = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative",  "activity", "()Landroid/app/Activity;");
    QAndroidJniObject contentResolver = qtActivityObj.callObjectMethod("getContentResolver", "()Landroid/content/ContentResolver;");

    // Init String[] { "android_id"}
    LOG << "Init String[] {\"android_id\"}";
    QAndroidJniEnvironment env;
    jobjectArray stringArrJava;
    stringArrJava = (jobjectArray)env->NewObjectArray(1,env->FindClass("java/lang/String"),env->NewStringUTF(""));
    env->SetObjectArrayElement(stringArrJava,0,env->NewStringUTF("android_id"));

    // Init sUri
    // String: "content://com.google.android.gsf.gservices"
    LOG << "Init sUri";
    jstring uriString = env->NewStringUTF("content://com.google.android.gsf.gservices");

    QAndroidJniObject sUriObj;
    sUriObj = QAndroidJniObject::callStaticObjectMethod("android/net/Uri",\
                                                         "parse",\
                                                         "(Ljava/lang/String;)Landroid/net/Uri;",\
                                                         uriString);

    LOG << "Get Cursor";
    QAndroidJniObject query = contentResolver.callObjectMethod("query", "(Landroid/net/Uri;[Ljava/lang/String;Ljava/lang/String;[Ljava/lang/String;Ljava/lang/String;)Landroid/database/Cursor;", sUriObj.object<jobject>(), 0, 0, stringArrJava, 0);

    QString retVal("");

    if(query == NULL){
        LOG << "query is NULL";
    }else{
        jint columns = query.callMethod<jint>("getColumnCount", "()I");
        LOG << static_cast<int>(columns);
        if(static_cast<int>(columns) >= 2){
            jboolean moveFirst = query.callMethod<jboolean>("moveToFirst", "()Z");
            Q_UNUSED(moveFirst);
            QAndroidJniObject string = query.callObjectMethod("getString", "(I)Ljava/lang/String;",0x01);
            retVal = string.toString();
        }
    }

    query.callMethod<void>("close", "()V");

    return retVal;
}


QString JavaCommunication::getDeiceIMSI()
{
    QAndroidJniEnvironment env;
    jclass contextClass = env->FindClass("android/content/Context");
    jfieldID fieldId = env->GetStaticFieldID(contextClass, "TELEPHONY_SERVICE", "Ljava/lang/String;");
    jstring telephonyManagerType = (jstring) env->GetStaticObjectField(contextClass, fieldId);

    jclass telephonyManagerClass = env->FindClass("android/telephony/TelephonyManager");
    jmethodID methodId = env->GetMethodID(contextClass, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");

    QAndroidJniObject qtActivityObj = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative",  "activity", "()Landroid/app/Activity;");
    jobject telephonyManager = env->CallObjectMethod(qtActivityObj.object<jobject>(), methodId, telephonyManagerType);

    methodId = env->GetMethodID(telephonyManagerClass, "getSubscriberId", "()Ljava/lang/String;");
    jstring jstr = (jstring) env->CallObjectMethod(telephonyManager, methodId);

    return QAndroidJniObject::fromLocalRef(jstr).toString();
}

QString JavaCommunication::getDeviceModel()
{
    QAndroidJniEnvironment env;
    jclass contextClass = env->FindClass("android/os/Build");
    jfieldID fieldId = env->GetStaticFieldID(contextClass, "MODEL", "Ljava/lang/String;");
    jstring model = (jstring) env->GetStaticObjectField(contextClass, fieldId);
    return QAndroidJniObject::fromLocalRef(model).toString();;
}

QString JavaCommunication::getAndroidVersion()
{
#ifdef PREFER_QT
    return QSysInfo::productVersion();
#else
    QAndroidJniEnvironment env;
    jclass contextClass = env->FindClass("android/os/Build$VERSION");
    jfieldID fieldId = env->GetStaticFieldID(contextClass, "RELEASE", "Ljava/lang/String;");
    jstring osVersion = (jstring) env->GetStaticObjectField(contextClass, fieldId);
    return QAndroidJniObject::fromLocalRef(osVersion).toString();
#endif
}

QString JavaCommunication::getAndroidID()
{
    QAndroidJniObject androidIdField = QAndroidJniObject::getStaticObjectField("android/provider/Settings$Secure", "ANDROID_ID", "Ljava/lang/String;");

    QAndroidJniObject qtActivityObj = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative",  "activity", "()Landroid/app/Activity;");
    QAndroidJniObject contentResolver = qtActivityObj.callObjectMethod("getContentResolver", "()Landroid/content/ContentResolver;");

    QString androidID = QAndroidJniObject::callStaticObjectMethod("android/provider/Settings$Secure","getString", "(Landroid/content/ContentResolver;Ljava/lang/String;)Ljava/lang/String;",contentResolver.object<jobject>(),androidIdField.object<jobject>()).toString();

    return androidID;
}

QString JavaCommunication::getSimCardSerialNo()
{
   /*TelephonyManager telemamanger = (TelephonyManager) getSystemService(Context.TELEPHONY_SERVICE);
    String getSimSerialNumber = telemamanger.getSimSerialNumber();*/

    QAndroidJniEnvironment env;
    jclass contextClass = env->FindClass("android/content/Context");
    jfieldID fieldId = env->GetStaticFieldID(contextClass, "TELEPHONY_SERVICE", "Ljava/lang/String;");
    jstring telephonyManagerType = (jstring) env->GetStaticObjectField(contextClass, fieldId);

    jclass telephonyManagerClass = env->FindClass("android/telephony/TelephonyManager");
    jmethodID methodId = env->GetMethodID(contextClass, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");

    QAndroidJniObject qtActivityObj = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative",  "activity", "()Landroid/app/Activity;");
    jobject telephonyManager = env->CallObjectMethod(qtActivityObj.object<jobject>(), methodId, telephonyManagerType);

    methodId = env->GetMethodID(telephonyManagerClass, "getSimSerialNumber", "()Ljava/lang/String;");
    jstring jstr = (jstring) env->CallObjectMethod(telephonyManager, methodId);

    return QAndroidJniObject::fromLocalRef(jstr).toString();;
}

QString JavaCommunication::getSimNumber()
{
    /*TelephonyManager telemamanger = (TelephonyManager) getSystemService(Context.TELEPHONY_SERVICE);
     String getSimSerialNumber = telemamanger.getSimSerialNumber();*/

     QAndroidJniEnvironment env;
     jclass contextClass = env->FindClass("android/content/Context");
     jfieldID fieldId = env->GetStaticFieldID(contextClass, "TELEPHONY_SERVICE", "Ljava/lang/String;");
     jstring telephonyManagerType = (jstring) env->GetStaticObjectField(contextClass, fieldId);

     jclass telephonyManagerClass = env->FindClass("android/telephony/TelephonyManager");
     jmethodID methodId = env->GetMethodID(contextClass, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");

     QAndroidJniObject qtActivityObj = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative",  "activity", "()Landroid/app/Activity;");
     jobject telephonyManager = env->CallObjectMethod(qtActivityObj.object<jobject>(), methodId, telephonyManagerType);

     methodId = env->GetMethodID(telephonyManagerClass, "getLine1Number", "()Ljava/lang/String;");
     jstring jstrNumber = (jstring) env->CallObjectMethod(telephonyManager, methodId);

     return QAndroidJniObject::fromLocalRef(jstrNumber).toString();
}

QString JavaCommunication::getWifiMacAdd()
{
    /*WifiManager wifiManager = (WifiManager) MyApplication.getInstance().getApplicationContext().getSystemService(Context.WIFI_SERVICE);
      WifiInfo wInfo = wifiManager.getConnectionInfo();
      String macAddress = wInfo.getMacAddress();*/

    QAndroidJniObject androidIdField = QAndroidJniObject::getStaticObjectField("android/content/Context", "WIFI_SERVICE", "Ljava/lang/String;");
    QAndroidJniObject qtActivityObj = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative",  "activity", "()Landroid/app/Activity;");
    QAndroidJniObject wifiManager = qtActivityObj.callObjectMethod("getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;",androidIdField.object<jobject>());
    QAndroidJniObject wifiInfo = wifiManager.callObjectMethod("getConnectionInfo","()Landroid/net/wifi/WifiInfo;");
    QString macAddress = wifiInfo.callObjectMethod("getMacAddress","()Ljava/lang/String;").toString();
    return macAddress;
}

#endif
