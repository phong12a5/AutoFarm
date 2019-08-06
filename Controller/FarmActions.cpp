#include "FarmActions.h"

#define WEB_API WebAPI::instance()
#define MODEL Model::instance()
#define JAVA_COM JavaCommunication::instance()

FarmActions* FarmActions::m_instance = nullptr;

FarmActions::FarmActions(QObject *parent) : QObject(parent)
{

}

FarmActions *FarmActions::instance()
{
    QMutex mutex;
    mutex.lock();
    if(m_instance == nullptr){
        m_instance = new FarmActions();
    }
    mutex.unlock();
    return m_instance;
}

void FarmActions::doActions()
{
    LOG;
    WEB_API->getDoAction();
    foreach (QJsonObject action, MODEL->actionList()) {
        if(action["action"].toString() == "viplike"){
            this->doVipLike(action);
        }
    }
    emit MODEL->currentActionListDone();
    MODEL->nextCurrentControlledObj();
}

void FarmActions::doVipLike(QJsonObject action)
{
    LOG << action["fbid"].toString();
#ifdef ANDROID_KIT
    if(action["fbid"].toString() != ""){
        JAVA_COM->openFBLiteWithUserID(MODEL->currentControlledPkg(),action["fbid"].toString());

        int count = 0;
        int failureCount = 0;
        while(count < NUMBER_CLICK){

            if(ShellOperation::findAndClick(LIKE_ICON)){
                count ++;
                failureCount = 0;
            }else
                failureCount ++;

            if(failureCount >= 10){
                LOG << "Couldn't find any like button";
                break;
            }

            if(count < NUMBER_CLICK)ShellOperation::callScrollEvent(QPoint(540,1200),QPoint(540,230));
            delay(1000);
        }
    }else {
        LOG << "fbid is empty";
    }
#endif
}
