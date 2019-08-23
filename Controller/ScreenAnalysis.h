#ifndef SCREENANALYSIS_H
#define SCREENANALYSIS_H

#include <QObject>
#include <QTimer>
#include <AutoFarmerAPIsWraper.hpp>
#include <AutoFarmerLogger.hpp>

class ScreenAnalysis : public QObject
{
    Q_OBJECT
public:
    explicit ScreenAnalysis(QObject *parent, AutoFarmerAPIsWraper* famerAPIs);

private:
    QTimer* m_checkScreenTimer;
    AutoFarmerAPIsWraper* m_famerAPIs;

signals:

public slots:
    void onCheckScreen();
};

#endif // SCREENANALYSIS_H
