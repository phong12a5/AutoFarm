#ifndef THREADCONTROLLER_H
#define THREADCONTROLLER_H

#include <QObject>
#include <QThread>
#include "Processing/CheckCurrSrcThread.hpp"

class ThreadController : public QObject
{
    Q_OBJECT
public:
    explicit ThreadController(QObject *parent = nullptr);
    ~ThreadController();

private:
    QThread m_checkScreenThread;
    QThread m_startNewActivityThread;

    CheckCurrSrcThread      checkScreenWorker;
    StartNewActivityThread  startNewActivityWorker;

    QThread m_readFile;


public:
    void startCheckCurrentScreen();

signals:
    void sigStartCheckCurrentScreen(const QString &);

public slots:
};

#endif // THREADCONTROLLER_H
