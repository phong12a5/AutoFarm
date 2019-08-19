#ifndef FARMACTIONS_H
#define FARMACTIONS_H

#include <QObject>
#include "AppDefines.hpp"
#include "AppEnums.hpp"
#include "Model.hpp"
#include "APIs/AutoFarmerJNI.hpp"
#include "AutoFarmerAPIsWraper.hpp"

#include <QMutex>

class FarmActions : public QObject
{
    Q_OBJECT

public:
    explicit FarmActions(QObject *parent = nullptr);
    void doActions();
    void setFarmerAPIs(AutoFarmerAPIsWraper _farmerAPIs);

private:
    void doVipLike(QJsonObject action);

private:
    AutoFarmerAPIsWraper m_famerAPIs;

signals:

public slots:
};

#endif // FARMACTIONS_H
