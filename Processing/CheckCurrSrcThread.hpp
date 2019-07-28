#ifndef CHECKCURRSRCTHREAD_H
#define CHECKCURRSRCTHREAD_H

#include <QObject>
#include <AppDefines.hpp>
#include <QTimer>
#include <QMutex>
#include "AppEnums.hpp"

class CheckCurrSrcThread : public QObject
{
    Q_OBJECT
public:
    explicit CheckCurrSrcThread(QObject *parent = nullptr);
    ~CheckCurrSrcThread();

private:
    bool isOnScreen(QString iconPath, QString screenImgPath = "");
    bool isCurrentScreen(int screenID, QString screenImgPath = "");
    int findScreen() const;

private:
    QTimer* m_updateCurrSrcTimer;

signals:
    void resultReady(const QString &result);

public slots:
    void doWork(const QString &parameter);
    void onUpdateCurrentScreen();
};

#endif // CHECKCURRSRCTHREAD_H
