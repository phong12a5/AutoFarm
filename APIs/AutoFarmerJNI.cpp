#include "AutoFarmerJNI.hpp"

AutoFarmerJNI* AutoFarmerJNI::m_instance = nullptr;

AutoFarmerJNI::AutoFarmerJNI(QObject *parent) : QObject(parent)
{
    // Do nothing
}

AutoFarmerJNI *AutoFarmerJNI::instance()
{
    if(!m_instance)
        m_instance = new AutoFarmerJNI();
    return m_instance;
}

void AutoFarmerJNI::openFBLiteApplication(QString packageName, QString activityName) const
{
    LOG_DEBUG << "Openning " << packageName + "/" + packageName + "." + activityName;
    QAndroidJniObject activity = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative", "activity", "()Landroid/app/Activity;");   //activity is valid
    if ( activity.isValid() )
    {
        // Equivalent to Jave code: 'Intent intent = new Intent();'
        QAndroidJniObject intent("android/content/Intent","()V");
        if ( intent.isValid() )
        {
            QAndroidJniObject param1 = QAndroidJniObject::fromString(packageName);
            QAndroidJniObject param2 = QAndroidJniObject::fromString(packageName + "." + activityName);

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

void AutoFarmerJNI::openFBLiteWithUserID(QString packageName, QString userID) const
{
    LOG_DEBUG << "packageName: " << packageName;
    LOG_DEBUG << "userID: " << userID;

    QAndroidJniObject activity = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative", "activity", "()Landroid/app/Activity;");   //activity is valid
    if ( activity.isValid() )
    {
        // Intent(Intent.ACTION_VIEW,Uri.parse("fb://profile/vongocanh.vo"));

        QAndroidJniObject actionView_Field = QAndroidJniObject::getStaticObjectField("android/content/Intent", "ACTION_VIEW", "Ljava/lang/String;");

        QAndroidJniEnvironment env;
        jstring uriString = env->NewStringUTF(QString("https://www.facebook.com/%1").arg(userID).toLocal8Bit().data());
        QAndroidJniObject sUriObj;
        sUriObj = QAndroidJniObject::callStaticObjectMethod("android/net/Uri",\
                                                             "parse",\
                                                             "(Ljava/lang/String;)Landroid/net/Uri;",\
                                                             uriString);

        QAndroidJniObject intent("android/content/Intent","(Ljava/lang/String;Landroid/net/Uri;)V",actionView_Field.object<jobject>(),sUriObj.object<jobject>());
        if ( intent.isValid() )
        {
            QAndroidJniObject param1 = QAndroidJniObject::fromString(packageName);
            QAndroidJniObject param2 = QAndroidJniObject::fromString(packageName + ".MainActivity");

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
