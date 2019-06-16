#ifndef APPMAIN_H
#define APPMAIN_H

#include <QObject>
#include "AppDefines.h"
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include "Model.h"
#include "AppEnums.h"
#include "Controller/MainController.h"
#include "Communication/WebAPI.h"

class AppMain : public QObject
{
    Q_OBJECT
public:
    explicit AppMain(QObject *parent = nullptr);
    void initApplication(QQmlApplicationEngine &engine);

private:
    void connectSignalSlot() const;

private:
    WebAPI m_webAPI;

signals:

public slots:
    void onStartProgram();
};

#endif // APPMAIN_H