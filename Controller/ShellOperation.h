#ifndef SHELLOPERATION_H
#define SHELLOPERATION_H

#include <QObject>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QImage>
#include "AppDefines.h"
#include <QStandardPaths>


class ShellOperation : public QObject
{
    Q_OBJECT
private:
    explicit ShellOperation(QObject *parent = nullptr);

public:
#ifdef ANDROID_KIT
    static QString screenShot(QString fileName = "screen.png");
    static void callTapEvent(const int x, const int y);
    static void callScrollEvent(QPoint point1, QPoint point2);
    static QString getCurrentActivity();
#endif
    static bool installPackage(QString packagePath);
private:
};

#endif // SHELLOPERATION_H
