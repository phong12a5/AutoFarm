#include "ShellOperation.h"

ShellOperation::ShellOperation(QObject *parent) : QObject(parent)
{

}

#ifdef ANDROID_KIT
void ShellOperation::callTapEvent(const int x, const int y)
{
    LOG << QString("Tapping at [%1,%2]").arg(x).arg(y);
    QStringList arg = QStringList()<< QString("-c")
                                   << QString("/system/bin/input")
                                   << QString("tap")
                                   << QString::number(x)
                                   << QString::number(y);
    LOG << "Args: " << arg;
    QProcess process;
    process.start("su",arg );
    process.waitForFinished(-1);
}

void ShellOperation::callScrollEvent(QPoint point1, QPoint point2)
{
    LOG << QString("Scrolling from [%1,%2] to [%3,%4]")\
           .arg(point1.x()).arg(point1.y())\
           .arg(point2.x()).arg(point2.y());

    QStringList arg = QStringList()<< "-c"
                                   << "input"
                                   << "swipe"
                                   << QString::number(point1.x())\
                                   << QString::number(point1.y())\
                                   << QString::number(point2.x())\
                                   << QString::number(point2.y());
    LOG << "Args: " << arg;

    QProcess process;
    process.start("su",arg);
    process.waitForFinished(-1);
}

QString ShellOperation::getCurrentActivity()
{
    QString retVal = "";
    QProcess proc;
    proc.start("su -c dumpsys window windows | grep -E 'mCurrentFocus'");
    proc.waitForFinished(-1);
    QStringList output = QString(proc.readAllStandardOutput()).split(' ');
    if(output.length() > 4){
        retVal = QString(output.at(4)).simplified().remove('}');
    }
    return retVal;
}

QString ShellOperation::screenShot(QString fileName)
{
    LOG << "Taking screen >> fileName: " << fileName;
    QString path = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    if (path.isEmpty()){
        LOG << "Could't not save to " << path;
        return QString("");
    }

    path.append(QString("/%1").arg(fileName));
    LOG << "Path: " << path;

    QProcess process;
    process.start("su", QStringList()<< "-c" << "/system/bin/screencap" << "-p" << path);
    process.waitForFinished(-1);

    return path;
}
#endif
