#ifndef SHELLOPERATION_H
#define SHELLOPERATION_H

#include <QObject>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QImage>
#include "AppDefines.hpp"
#include <QStandardPaths>
#include "Processing/ImageProcessing.hpp"

class ShellOperation : public QObject
{
    Q_OBJECT
private:
    explicit ShellOperation(QObject *parent = nullptr);

public:
#ifdef ANDROID_KIT
    static bool installPackage(QString packagePath);
    static QString screenShot(QString fileName = "screen.png");
    static void callTapEvent(const int x, const int y);
    static void callScrollEvent(QPoint point1, QPoint point2);
    static QString getCurrentActivity();
    static bool findAndClick(QString iconPath, float threshold = 0.99);
    static void tapScreen(QPoint point, bool noDelay = false);
    static bool enterText(QString text);
    static void killSpecificApp(QString packageName);
    static QPoint findAnImageOnScreen(QString iconPath);
    static void clearPackageData(QString packageName);
#endif
private:
};

#endif // SHELLOPERATION_H
