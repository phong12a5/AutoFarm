#ifndef THREADCONTROLLER_H
#define THREADCONTROLLER_H

#include <QObject>
#include <QThread>
#include "Processing/CheckCurrSrcThread.h"
#include "Processing/CheckCurrActivityThread.h"

class ThreadController : public QObject
{
    Q_OBJECT
public:
    explicit ThreadController(QObject *parent = nullptr);
    ~ThreadController();

private:
    QThread m_checkScreenThread;
    QThread m_checkActivityThread;
    CheckCurrSrcThread checkScreenWorker;
    CheckCurrActivityThread checkActivityWorker;

    QThread m_readFile;


public:
    void startCheckCurrentScreen();
    void startCheckCurrentActivity();

signals:
    void sigStartCheckCurrentScreen(const QString &);
    void sigStartCheckCurrentActivity(const QString &);

public slots:
};

#endif // THREADCONTROLLER_H
