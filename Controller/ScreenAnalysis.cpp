#include "ScreenAnalysis.h"

ScreenAnalysis::ScreenAnalysis(QObject *parent, AutoFarmerAPIsWraper* famerAPIs) :
    QObject(parent),
    m_famerAPIs(famerAPIs)
{
    m_checkScreenTimer = new QTimer(this);
    m_checkScreenTimer->setInterval(2000);
    m_checkScreenTimer->setSingleShot(false);
    connect(m_checkScreenTimer, SIGNAL(timeout()), this, SLOT(onCheckScreen()));
    m_checkScreenTimer->start();
}

void ScreenAnalysis::onCheckScreen()
{
    LOG_DEBUG;
    QString screenImgPath = m_famerAPIs->w_screenCapture(CHECKING_SCREEN_IMG);
    if(screenImgPath != ""){
        QList<TEXT_COMPOENT> textCompList = m_famerAPIs->w_getTextFromImage(screenImgPath);
        QString fullContent;
        foreach (TEXT_COMPOENT textComp, textCompList) {
            fullContent += (textComp.text + " ");
        }
        LOG_DEBUG << "fullContent: " << fullContent;

        if(fullContent.contains("incorrect password")){
            LOG_DEBUG << "Incorrect password screen";
        }else if(fullContent.contains("confirm") && fullContent.contains("identify")){
            LOG_DEBUG << "Checkpoint screen";
        }else if(fullContent.contains("missing email or password")){
            LOG_DEBUG << "Missing email or password screen";
        }else if(fullContent.contains("download your information")){
            LOG_DEBUG << "Checkpoint screen";
        }else if(fullContent.contains("find an account")){
            LOG_DEBUG << "Couldn't find account";
        }else if(fullContent.contains("save login info")){
            LOG_DEBUG << "Login screen";
        }else if(fullContent.contains("choose from gallery")){
            LOG_DEBUG << "Upoad avatar screen";
        }else if(fullContent.contains("facebook is better with friends")){
            LOG_DEBUG << "Upoad avatar screen";
        }else if(fullContent.contains("people you may know")){
            LOG_DEBUG << "suggest add friend screen";
        }else if(fullContent.contains("mobile number or email") && fullContent.contains("password")){
            LOG_DEBUG << "Save login info screen";
        }else if(fullContent.contains("english") && fullContent.contains("more languages")){
            LOG_DEBUG << "Need to change language";
        }
    }
}
