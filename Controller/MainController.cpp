#include "MainController.hpp"
#include "Model.hpp"

#define MODEL           Model::instance()
#define AUTOFARMERJNI   AutoFarmerJNI::instance()

MainController::MainController(QObject *parent) : QObject(parent)
{
    m_farmAction = nullptr;
    m_screenAnalysis = nullptr;

    m_changeScreenTimer.setSingleShot(true);
    m_changeScreenTimer.setInterval(120000);
}

MainController::~MainController()
{
    m_checkScreenThread.quit();
    m_checkScreenThread.wait();
}

void MainController::initController()
{
    LOG_DEBUG;
    connect(&m_changeScreenTimer, SIGNAL(timeout()), this, SLOT(onchangeScreenTimerTimeout()));
    connect(MODEL,SIGNAL(currentScreenChanged()),this,SLOT(onChangeScreen()));
    connect(MODEL,SIGNAL(nextCurrentControlledObjChanged()),this,SLOT(executeRequiredActions()));
    connect(MODEL,SIGNAL(currentActionListDone()),this,SLOT(updateResult()));
    connect(MODEL,SIGNAL(finishedListObject()),this,SLOT(onFinishedListObject()));
    connect(MODEL,SIGNAL(sigStartProgram()),this,SLOT(onStartProgram()));
}

QJsonDocument MainController::loadJson(QString fileName)
{
    LOG_DEBUG << "[Model]";
    QFile jsonFile(fileName);
    jsonFile.open(QFile::ReadOnly);
    return QJsonDocument().fromJson(jsonFile.readAll());
}

void MainController::saveJson(QJsonDocument document, QString fileName)
{
    LOG_DEBUG <<"document: " << document;
    LOG_DEBUG <<"fileName: " << fileName;
    QFile jsonFile(fileName);
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(document.toJson());
}

void MainController::downloadAndInstallPackages()
{
    QJsonDocument jdoc = m_famerAPIs.w_getApk();

    LOG_DEBUG << jdoc;

    QMap<QString, USER_DATA>  userDataList = MODEL->userDataList();
    QList<QString> keys = userDataList.keys();

    foreach (QJsonValue data, jdoc.array()) {
        if(data.isObject()){
            QJsonObject obj = data.toObject();
            if(!keys.contains(obj["package"].toString())){
                QString apkPath = m_famerAPIs.w_downloadApk(obj["apk"].toString());
                if(apkPath != ""){
                    m_famerAPIs.w_installPackage(apkPath);
                    USER_DATA data;
                    userDataList.insert(obj["package"].toString(),data);
                }
            }else{
                LOG_DEBUG << obj["package"].toString() << " EXISTED";
            }
        }
    }
    MODEL->setUserDataList(userDataList);

}

#if 0
void MainController::startCheckCurrentScreen()
{
    if(m_checkScreenThread.isRunning()){
        LOG_DEBUG << "m_checkScreenThread is running already";
        return;
    }
    checkScreenWorker.setAutoFarmerAPIs(m_famerAPIs);
    checkScreenWorker.moveToThread(&m_checkScreenThread);
    connect(&m_checkScreenThread, &QThread::finished, &checkScreenWorker, &QObject::deleteLater);
    connect(this, &MainController::sigStartCheckCurrentScreen, &checkScreenWorker, &CheckCurrSrcWorker::doWork);
    connect(&checkScreenWorker, &CheckCurrSrcWorker::updateCurrentScreen,this, &MainController::onUpdateCurrentScreen);
    m_checkScreenThread.start();
    emit sigStartCheckCurrentScreen();
}
#endif

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

                    LOG_DEBUG << "user_data.uid           :" << user_data.uid;
                    if(m_famerAPIs.w_isExistPackage(packageName)){
                        userDataList.insert(packageName,user_data);
                    }
                }
            }

            MODEL->setUserDataList(userDataList);
        }
    }else{
        LOG_DEBUG << CURRENT_DIR + "userDataList.json" << " not exist";
    }
}

void MainController::saveUserDataList()
{
    LOG_DEBUG;
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
    if(m_famerAPIs.w_initEnv(MODEL->token(),APPNAME_ID_FACEBOOK)){
        m_farmAction->setFarmerAPIs(m_famerAPIs);
        this->loadUserDataList();
        this->downloadAndInstallPackages();
        this->saveUserDataList();
        m_famerAPIs.w_closePackage(MODEL->currentControlledPkg());

        m_farmAction = new FarmActions(this,&m_famerAPIs);
        m_screenAnalysis = new ScreenAnalysis(this,m_famerAPIs);

        MODEL->nextCurrentControlledObj();
    }else{
        LOG_DEBUG << "Init API false";
    }
}

void MainController::executeRequiredActions()
{
    if(MODEL->currentControlledUser().uid == ""){
        MODEL->updateCurrentControlleredUser(m_famerAPIs.w_getClone());
        this->saveUserDataList();
    }else{
        LOG_DEBUG << "User info has storaged already";
    }

    AUTOFARMERJNI->openFBLiteWithUserID(MODEL->currentControlledPkg(),MODEL->currentControlledUser().uid);
    MODEL->setCurrentScreen(AppEnums::HMI_UNKNOW_SCREEN);
//    startCheckCurrentScreen();
}

void MainController::updateResult()
{
    foreach (QJsonObject action, MODEL->actionList()) {
        m_famerAPIs.w_doResult(MODEL->currentControlledUser().uid, action);
    }
    m_famerAPIs.w_closePackage(MODEL->currentControlledPkg());
}

void MainController::onFinishedListObject()
{
    LOG_DEBUG << "The last user: " << MODEL->currentControlledUser().uid;
    if(MODEL->deviceInfo().isNox == "true"){
//        m_farmerAPIsshellCommand(QString("touch %1%2").arg(DCIM_FOLDER).arg(ENDSCRIPT_FILENAME));
    }
}

void MainController::onchangeScreenTimerTimeout()
{
    LOG_DEBUG << MODEL->screenStr(MODEL->currentScreen());
    if(MODEL->currentScreen() == AppEnums::HMI_LOGIN_SCREEN){
        LOG_DEBUG << "Click Login again!";
        if(m_famerAPIs.findAndClick(LOGIN_BTN)){
            return;
        }
    }

    if(MODEL->currentScreen() != AppEnums::HMI_NEW_FEED_SCREEN){
        m_famerAPIs.w_wipePackage(QStringList() << MODEL->currentControlledPkg());
        m_famerAPIs.w_closePackage(MODEL->currentControlledPkg());
        MODEL->nextCurrentControlledObj();
    }
}

void MainController::onUpdateCurrentScreen(int screenID)
{
    LOG_DEBUG << MODEL->screenStr(screenID);
    MODEL->setCurrentScreen(screenID);
}

void MainController::onChangeScreen()
{
    LOG_DEBUG  << MODEL->screenStr(MODEL->currentScreen());

    m_changeScreenTimer.stop();

    switch(MODEL->currentScreen()){
    case AppEnums::HMI_UNKNOW_SCREEN:
        m_famerAPIs.findAndClick(ENGLISH_BTN);
        break;
    case AppEnums::HMI_SELECT_LANGUAGE_SCREEN:
        m_famerAPIs.findAndClick(ENGLISH_BTN);
        break;
    case AppEnums::HMI_LOGIN_SCREEN:
    {
        m_famerAPIs.findAndClick(ENGLISH_BTN);
        delay(500);
        m_famerAPIs.findAndClick(EMAIL_FIELD);
        delay(1000);
        m_famerAPIs.w_inputText(MODEL->currentControlledUser().uid);
        if(!MODEL->isNoxDevice()){
            m_famerAPIs.enterKeyBoard();
        }

        delay(1000);
        m_famerAPIs.findAndClick(PASSWORD_FIELD);
        delay(1000);
        m_famerAPIs.w_inputText(MODEL->currentControlledUser().password);
        if(!MODEL->isNoxDevice()){
            m_famerAPIs.enterKeyBoard();
        }

        delay(1000);
        m_famerAPIs.findAndClick(LOGIN_BTN);
    }
        break;
    case AppEnums::HMI_MISSING_PASSWORD_SCREEN:
    case AppEnums::HMI_ACCOUNT_NOT_FOUND_SCREEN:
        MODEL->updateCurrentControlleredUser(m_famerAPIs.w_getClone());
        this->saveUserDataList();
        m_famerAPIs.w_wipePackage(QStringList() << MODEL->currentControlledPkg());
        m_famerAPIs.w_closePackage(MODEL->currentControlledPkg());
        MODEL->nextCurrentControlledObj();
        break;

    case AppEnums::HMI_INCORRECT_PASSWORD_SCREEN:
    case AppEnums::HMI_CONFIRM_INDENTIFY_SCREEN:
    case AppEnums::HMI_DEACTIVE_ACCOUNT_SCREEN:
//        WEB_API->updateCheckPoint();
        MODEL->updateCurrentControlleredUser(m_famerAPIs.w_getClone());
        this->saveUserDataList();
        m_famerAPIs.w_wipePackage(QStringList() << MODEL->currentControlledPkg());
        m_famerAPIs.w_closePackage(MODEL->currentControlledPkg());
        MODEL->nextCurrentControlledObj();
        break;
    case AppEnums::HMI_TURNON_FIND_FRIEND_SCREEN:
        m_famerAPIs.findAndClick(SKIP_FIND_FRIEND_BTN);
        break;
    case AppEnums::HMI_SAVE_LOGIN_INFO_SCREEN:
        m_famerAPIs.findAndClick(OK_BUTTON);
        break;
    case AppEnums::HMI_CHOOSE_AVATAR_SCREEN:
        m_famerAPIs.findAndClick(SKIP_AVARTAR);
        break;
    case AppEnums::HMI_ADDFRIEND_SUGGESTION_SCREEN:
        m_famerAPIs.findAndClick(SKIP_AVARTAR);
        delay(1000);
        AUTOFARMERJNI->openFBLiteWithUserID(MODEL->currentControlledPkg(),"");
        break;
    case AppEnums::HMI_NEW_FEED_SCREEN:
        MODEL->clearActionList();
        m_farmAction->doActions();
        MODEL->nextCurrentControlledObj();
        m_famerAPIs.w_closePackage(MODEL->currentControlledPkg());
        break;
    case AppEnums::HMI_LOGIN_AGAIN_SCREEN:
        break;
    default:
        break;
    }

    m_changeScreenTimer.start();
}

