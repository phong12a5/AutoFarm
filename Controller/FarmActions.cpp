#include "FarmActions.hpp"

#define MODEL Model::instance()
#define AUTOFARMERJNI AutoFarmerJNI::instance()

FarmActions::FarmActions(QObject *parent,AutoFarmerAPIsWraper* famerAPIs) :
    QObject(parent),
    m_famerAPIs(famerAPIs)
{

}

void FarmActions::doActions()
{
    LOG_DEBUG;
    MODEL->setActionList(m_famerAPIs->w_doAction(MODEL->currentControlledUser().uid));
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
    LOG_DEBUG << action["fbid"].toString();
    if(action["fbid"].toString() != ""){
        AUTOFARMERJNI->openFBLiteWithUserID(MODEL->currentControlledPkg(),action["fbid"].toString());

        int count = 0;
        int failureCount = 0;
        while(count < NUMBER_CLICK){

            if(m_famerAPIs->findAndClick(LIKE_ICON)){
                count ++;
                failureCount = 0;
            }else
                failureCount ++;

            if(failureCount >= 10){
                LOG_DEBUG << "Couldn't find any like button";
                break;
            }

            if(count < NUMBER_CLICK) m_famerAPIs->w_swipe(QPoint(540,1200),QPoint(540,230),1000);
            delay(1000);
        }
    }else {
        LOG_DEBUG << "fbid is empty";
    }
}
