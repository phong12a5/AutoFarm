#ifndef CHECKCURRACTIVITYTHREAD_H
#define CHECKCURRACTIVITYTHREAD_H

#include <QObject>
#include <AppDefines.hpp>
#include <QTimer>
#include <QMutex>
#include "Controller/ShellOperation.hpp"

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
    void sigResponeContent(QString *);

public slots:
    void doWork(const QString &parameter);
    void onUpdateCurrentActivity();
};

#endif // CHECKCURRACTIVITYTHREAD_H
