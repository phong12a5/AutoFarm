#include "MainController.hpp"
#include "Communication/WebAPI.hpp"
#include "Model.hpp"

#define WEB_API         WebAPI::instance()
#define MODEL           Model::instance()
#define JAVA_COM        JavaCommunication::instance()
#define FARM_ACTIONS    FarmActions::instance()

MainController* MainController::m_instance = nullptr;

MainController::MainController(QObject *parent) : QObject(parent)
{
    m_changeScreenTimer.setSingleShot(true);
    m_changeScreenTimer.setInterval(120000);
}


MainController *MainController::instance()
{
    if(m_instance == nullptr){
        m_instance = new MainController();
    }
    return m_instance;
}

void MainController::initController()
{
    LOG;
    connect(&m_changeScreenTimer, SIGNAL(timeout()), this, SLOT(onchangeScreenTimerTimeout()));
//    connect(MODEL,SIGNAL(currentScreenChanged()),this,SLOT(onChangeScreen()));
    connect(MODEL,SIGNAL(nextCurrentControlledObjChanged()),this,SLOT(executeRequiredActions()));
    connect(MODEL,SIGNAL(currentActionListDone()),this,SLOT(updateResult()));
    connect(MODEL,SIGNAL(finishedListObject()),this,SLOT(onFinishedListObject()));
    connect(MODEL,SIGNAL(sigStartProgram()),this,SLOT(onStartProgram()));
}

QJsonDocument MainController::loadJson(QString fileName)
{
    LOG << "[Model]";
    QFile jsonFile(fileName);
    jsonFile.open(QFile::ReadOnly);
    return QJsonDocument().fromJson(jsonFile.readAll());
}

void MainController::saveJson(QJsonDocument document, QString fileName)
{
    LOG <<"document: " << document;
    LOG <<"fileName: " << fileName;
    QFile jsonFile(fileName);
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(document.toJson());
}

void MainController::downloadAndInstallPackages()
{
    QJsonObject retVal = m_farmerAPIs.getApk();

    QJsonDocument jdoc = QJsonDocument(retVal);

    LOG << jdoc;

    QMap<QString, USER_DATA>  userDataList = MODEL->userDataList();
    QList<QString> keys = userDataList.keys();

    foreach (QJsonValue data, jdoc.array()) {
        if(data.isObject()){
            QJsonObject obj = data.toObject();
            if(!keys.contains(obj["package"].toString())){
                QJsonObject apkPathObj = m_farmerAPIs.downloadApk(QUrl(obj["apk"].toString()));
                if(apkPathObj["ResponseData"] != ""){
                    m_farmerAPIs.installPackage(apkPathObj["ResponseData"]);
                    USER_DATA data;
                    userDataList.insert(obj["package"].toString(),data);
                }
            }else{
                LOG << obj["package"].toString() << " EXISTED";
            }
        }
    }

}

void MainController::startCheckCurrentScreen()
{
    multiThreadController.startCheckCurrentScreen();
}

void MainController::loadUserDataList()
{
    QFile file(USER_DATA_LIST_PATH);
    if(file.exists()){

        QJsonDocument userDataListJson = this->loadJson(USER_DATA_LIST_PATH);
        if(!userDataListJson.isNull()){

            QMap<QString, USER_DATA> userDataList;
            userDataList.clear();

            foreach (QJsonValue data, userDataListJson.array()) {
                if(data.isObject()){
                    QJsonObject obj = data.toObject();
                    QString packageName = obj["package_name"].toString();
                    QJsonObject userDataJson = obj["user_data"].toObject();
                    USER_DATA user_data;
                    user_data._id              = userDataJson["_id"].toString();
                    user_data.uid              = userDataJson["uid"].toString();
                    user_data.password         = userDataJson["password"].toString();
                    user_data.cookie           = userDataJson["cookie"].toString();
                    user_data.token            = userDataJson["token"].toString();
                    user_data.birthday         = userDataJson["birthday"].toString();
                    user_data.name             = userDataJson["name"].toString();
                    user_data.sex              = userDataJson["sex"].toString();
                    user_data.country          = userDataJson["country"].toString();
                    user_data.email            = userDataJson["email"].toString();
                    user_data.avartar          = userDataJson["avartar"].toString();
                    user_data.created_date     = userDataJson["created_date"].toString();
                    user_data.farming_status   = userDataJson["farming_status"].toString();
                    user_data.alive_status     = userDataJson["alive_status"].toString();
                    user_data.created_at       = userDataJson["created_at"].toString();
                    user_data.updated_at       = userDataJson["updated_at"].toString();
                    user_data.user_id          = userDataJson["user_id"].toString();

                    LOG << "user_data.uid           :" << user_data.uid;
                    if(m_farmerAPIs.isExistPackage(packageName)){
                        userDataList.insert(packageName,user_data);
                    }
                }
            }

            MODEL->setUserDataList(userDataList);
        }
    }else{
        LOG << CURRENT_DIR + "userDataList.json" << " not exist";
    }
}

void MainController::saveUserDataList()
{
    LOG;
    QMap<QString, USER_DATA>::const_iterator i = MODEL->userDataList().constBegin();
    QJsonArray objectArray;
    while (i != MODEL->userDataList().constEnd()) {
        QJsonObject obj;
        USER_DATA user_data = static_cast<USER_DATA>(i.value());
        QJsonObject userDataJson;
        userDataJson["_id"]             = user_data._id           ;
        userDataJson["uid"]             = user_data.uid           ;
        userDataJson["password"]        = user_data.password      ;
        userDataJson["cookie"]          = user_data.cookie        ;
        userDataJson["token"]           = user_data.token         ;
        userDataJson["birthday"]        = user_data.birthday      ;
        userDataJson["name"]            = user_data.name          ;
        userDataJson["sex"]             = user_data.sex           ;
        userDataJson["country"]         = user_data.country       ;
        userDataJson["email"]           = user_data.email         ;
        userDataJson["avartar"]         = user_data.avartar       ;
        userDataJson["created_date"]    = user_data.created_date  ;
        userDataJson["farming_status"]  = user_data.farming_status;
        userDataJson["alive_status"]    = user_data.alive_status  ;
        userDataJson["created_at"]      = user_data.created_at    ;
        userDataJson["updated_at"]      = user_data.updated_at    ;
        userDataJson["user_id"]         = user_data.user_id       ;

        obj["package_name"] = i.key();
        obj["user_data"] = userDataJson;
        objectArray.append(QJsonValue(obj));
        ++i;
    }
    this->saveJson(QJsonDocument(objectArray),USER_DATA_LIST_PATH);
}

void MainController::onStartProgram()
{
    QJsonObject retVal = m_farmerAPIs.initEnv(MODEL->token(),APPNAME_ID_FACEBOOK);
    if(retVal["Status"].toBool()){

        this->loadUserDataList();
        this->downloadAndInstallPackages();
        this->saveUserDataList();
        MODEL->nextCurrentControlledObj();
    }else{
        LOG << "Init API false";
    }
}

void MainController::executeRequiredActions()
{
    if(MODEL->currentControlledUser().uid == ""){
        MODEL->updateCurrentControlleredUser(WEB_API->cloneUserData());
    }else{
        LOG << "User info has storaged already";
    }

    JavaCommunication::instance()->openFBLiteWithUserID(packageName,sxtraData);
    MODEL->setCurrentScreen(AppEnums::HMI_UNKNOW_SCREEN);
    startCheckCurrentScreen();
}

void MainController::updateResult()
{
    WEB_API->getDoResult();
#ifdef ANDROID_KIT
    ShellOperation::killSpecificApp(MODEL->currentControlledPkg());
#endif
}

void MainController::onFinishedListObject()
{
    LOG << "The last user: " << MODEL->currentControlledUser().uid;
    if(MODEL->deviceInfo().isNox == "true"){
#ifdef ANDROID_KIT
        ShellOperation::shellCommand(QString("touch %1%2").arg(ENDSCRIPT_PATH).arg(ENDSCRIPT_FILENAME));
#endif
    }
}

void MainController::onchangeScreenTimerTimeout()
{
    LOG << MODEL->screenStr(MODEL->currentScreen());
#ifdef ANDROID_KIT
    if(MODEL->currentScreen() == AppEnums::HMI_LOGIN_SCREEN){
        LOG << "Click Login again!";
        if(ShellOperation::findAndClick(LOGIN_BTN)){
            return;
        }
    }

    if(MODEL->currentScreen() != AppEnums::HMI_NEW_FEED_SCREEN){
        ShellOperation::clearPackageData(MODEL->currentControlledPkg());
        MODEL->nextCurrentControlledObj();
    }
#endif
}

void MainController::onChangeScreen(int screenID)
{

    if(screenID == MODEL->currentScreen())
        return;

    MODEL->setCurrentScreen(screenID);
    LOG << "currentScreen: " << MODEL->screenStr(MODEL->currentScreen());

#ifdef ANDROID_KIT
    m_changeScreenTimer.stop();

    switch(MODEL->currentScreen()){
    case AppEnums::HMI_UNKNOW_SCREEN:
        ShellOperation::findAndClick(ENGLISH_BTN);
        break;
    case AppEnums::HMI_SELECT_LANGUAGE_SCREEN:
        ShellOperation::findAndClick(ENGLISH_BTN);
        break;
    case AppEnums::HMI_LOGIN_SCREEN:
    {
        ShellOperation::findAndClick(ENGLISH_BTN);
        delay(500);
        ShellOperation::findAndClick(EMAIL_FIELD, true);
        delay(1000);
        ShellOperation::enterText(MODEL->currentControlledUser().uid);
        if(!MODEL->isNoxDevice()){
            ShellOperation::enterKeyBoard();
        }

        delay(1000);
        ShellOperation::findAndClick(PASSWORD_FIELD, true);
        delay(1000);
        ShellOperation::enterText(MODEL->currentControlledUser().password);
        if(!MODEL->isNoxDevice()){
            ShellOperation::enterKeyBoard();
        }

        delay(1000);
        ShellOperation::findAndClick(LOGIN_BTN);
    }
        break;
    case AppEnums::HMI_MISSING_PASSWORD_SCREEN:
    case AppEnums::HMI_ACCOUNT_NOT_FOUND_SCREEN:
        MODEL->updateCurrentControlleredUser(WEB_API->cloneUserData());
        ShellOperation::clearPackageData(MODEL->currentControlledPkg());
        MODEL->nextCurrentControlledObj();
        break;

    case AppEnums::HMI_INCORRECT_PASSWORD_SCREEN:
    case AppEnums::HMI_CONFIRM_INDENTIFY_SCREEN:
    case AppEnums::HMI_DEACTIVE_ACCOUNT_SCREEN:
        WEB_API->updateCheckPoint();
        MODEL->updateCurrentControlleredUser(WEB_API->cloneUserData());
        ShellOperation::clearPackageData(MODEL->currentControlledPkg());
        MODEL->nextCurrentControlledObj();
        break;
    case AppEnums::HMI_TURNON_FIND_FRIEND_SCREEN:
        ShellOperation::findAndClick(SKIP_FIND_FRIEND_BTN);
        break;
    case AppEnums::HMI_SAVE_LOGIN_INFO_SCREEN:
        ShellOperation::findAndClick(OK_BUTTON);
        break;
    case AppEnums::HMI_CHOOSE_AVATAR_SCREEN:
        ShellOperation::findAndClick(SKIP_AVARTAR);
        break;
    case AppEnums::HMI_ADDFRIEND_SUGGESTION_SCREEN:
        ShellOperation::findAndClick(SKIP_AVARTAR);
        delay(1000);
        JAVA_COM->openFBLiteWithUserID(MODEL->currentControlledPkg(),"");
        break;
    case AppEnums::HMI_NEW_FEED_SCREEN:
        MODEL->clearActionList();
//        FARM_ACTIONS->doActions();
        MODEL->nextCurrentControlledObj();
        break;
    case AppEnums::HMI_LOGIN_AGAIN_SCREEN:
        break;
    default:
        break;
    }

    m_changeScreenTimer.start();
#endif
}

