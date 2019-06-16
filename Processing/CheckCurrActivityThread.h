#ifndef CHECKCURRACTIVITYTHREAD_H
#define CHECKCURRACTIVITYTHREAD_H

#include <QObject>
#include <AppDefines.h>
#include <QTimer>
#include <QMutex>
#include "Controller/ShellOperation.h"

class CheckCurrActivityThread : public QObject
{
    Q_OBJECT
public:
    explicit CheckCurrActivityThread(QObject *parent = nullptr);
    ~CheckCurrActivityThread();


private:
    QTimer* m_updateCurrActTimer;

signals:
    void resultReady(const QString &result);

public slots:
    void doWork(const QString &parameter);
    void onUpdateCurrentActivity();
};

#endif // CHECKCURRACTIVITYTHREAD_H