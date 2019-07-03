#ifndef APPMAIN_H
#define APPMAIN_H

#include <QObject>
#include "AppDefines.hpp"
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include "Model.hpp"
#include "AppEnums.hpp"
#include "MainController.hpp"

class AppMain : public QObject
{
    Q_OBJECT
public:
    explicit AppMain(QObject *parent = nullptr);
    void initApplication(QQmlApplicationEngine &engine);

private:
    void connectSignalSlot() const;
    void coppyFolder(QString src, QString dst);
private:

signals:

public slots:
    void onStartProgram();
};

#endif // APPMAIN_H
