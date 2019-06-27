#ifndef APPMAIN_H
#define APPMAIN_H

#include <QObject>
#include "AppDefines.h"
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include "Model.h"
#include "AppEnums.h"
#include "MainController.h"

class AppMain : public QObject
{
    Q_OBJECT
public:
    explicit AppMain(QObject *parent = nullptr);
    void initApplication(QQmlApplicationEngine &engine);

private:
    void connectSignalSlot() const;

private:

signals:

public slots:
    void onStartProgram();
};

#endif // APPMAIN_H
