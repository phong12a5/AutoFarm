#ifndef FARMACTIONS_H
#define FARMACTIONS_H

#include <QObject>
#include "AppDefines.hpp"
#include "AppEnums.hpp"
#include "Model.hpp"
#include "Communication/JavaCommunication.hpp"
#include "Communication/WebAPI.hpp"

#include <QMutex>

class FarmActions : public QObject
{
    Q_OBJECT
private:
    explicit FarmActions(QObject *parent = nullptr);

    void doVipLike(QJsonObject action);
public:
    static FarmActions *instance();

    void doActions();

private:
    static FarmActions *m_instance;

signals:

public slots:
};

#endif // FARMACTIONS_H
