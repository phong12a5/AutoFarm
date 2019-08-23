#include "ScreenAnalysis.h"

ScreenAnalysis::ScreenAnalysis(QObject *parent, AutoFarmerAPIsWraper* famerAPIs) :
    QObject(parent),
    m_famerAPIs(famerAPIs)
{
    m_checkScreenTimer = new QTimer(this);
    m_checkScreenTimer->setInterval(2000);
    m_checkScreenTimer->setSingleShot(false);
    connect(m_checkScreenTimer, SIGNAL(timeout()), this, SLOT(onCheckScreen));
    m_checkScreenTimer->start();
}

void ScreenAnalysis::onCheckScreen()
{
    LOG_DEBUG;
    QString screenImgPath = m_famerAPIs->w_screenCapture(CHECKING_SCREEN_IMG);
    if(screenImgPath != ""){
        QList<TEXT_COMPOENT> textCompList = m_famerAPIs->w_getTextFromImage(screenImgPath);
        foreach (TEXT_COMPOENT textComp, textCompList) {

        }
    }
}
