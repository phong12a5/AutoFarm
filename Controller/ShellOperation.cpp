#include "ShellOperation.hpp"
#include "Model.hpp"

#define MODEL           Model::instance()

ShellOperation::ShellOperation(QObject *parent) : QObject(parent)
{

}


#ifdef ANDROID_KIT

bool ShellOperation::installPackage(QString packagePath)
{
    LOG << packagePath;
    LOG << "Thread ID " << QThread::currentThreadId();

    if(!QFile(packagePath).exists()){
        LOG << "packagePath not existed";
        return false;
    }else{
        QProcess process;
        process.start(SHELL_CMD_PREFIX, QStringList() << "-c" << QString("pm install %1").arg(packagePath));
        process.waitForFinished(-1);

        LOG << process.readAllStandardError();
        LOG << process.readAllStandardOutput();

        return true;
    }
}

void ShellOperation::callScrollEvent(QPoint point1, QPoint point2)
{
    LOG << QString("Scrolling from [%1,%2] to [%3,%4]")\
           .arg(point1.x()).arg(point1.y())\
           .arg(point2.x()).arg(point2.y());

    QProcess process;
#if 0
    process.start(SHELL_CMD_PREFIX,QStringList() << "-c" << QString("input swipe %1 %2 %3 %4").arg(QString::number(point1.x())).\
                                                            arg(QString::number(point1.y())).\
                                                            arg(QString::number(point2.x())).\
                                                            arg(QString::number(point2.y())));
#endif
    process.start(SHELL_CMD_PREFIX, QStringList() << "-c" << "input swipe 500 1200 500 500");

    process.waitForFinished(-1);
}

QString ShellOperation::getCurrentActivity()
{
    QString retVal = "";
    QProcess process;
    process.start(SHELL_CMD_PREFIX, QStringList() << "-c" << QString("dumpsys window windows | grep -E 'mCurrentFocus'"));
    process.waitForFinished(-1);
    QStringList output = QString(process.readAllStandardOutput()).split(' ');
    if(output.length() > 4){
        retVal = QString(output.at(4)).simplified().remove('}');
    }
    return retVal;
}

bool ShellOperation::findAndClick(QString iconPath)
{
    LOG << iconPath;
    QString screenImgPath = ShellOperation::screenShot();
    QPoint point = ImageProcessing::findImageOnImage(iconPath,screenImgPath);
    if(!point.isNull()){
        ShellOperation::tapScreen(point);
        return true;
    }else{
        screenImgPath = ShellOperation::screenShot();
        point = ImageProcessing::findImageOnImage(iconPath,screenImgPath);
        if(!point.isNull()){
            ShellOperation::tapScreen(point);
            return true;
        }else{
            return false;
        }
    }
}

bool ShellOperation::findAndClickList(QString iconPath)
{
    QString screenImgPath = ShellOperation::screenShot();
    QList<QPoint> pointList = ImageProcessing::findImageListOnImage(iconPath,screenImgPath);
    if(!pointList.isEmpty()){
        foreach (QPoint point, pointList) {
            ShellOperation::tapScreen(point);
            delay(1000);
        }
        return true;
    }else{
        LOG << "There is no the matching image";
        return false;
    }
}

void ShellOperation::tapScreen(QPoint point)
{
    LOG << "Tapping at [" << point.x() << "," << point.y() << "]";
    QProcess proc;
    proc.start(SHELL_CMD_PREFIX, QStringList() << "-c" << QString("input tap %1 %2").arg(point.x()).arg(point.y()));
    proc.waitForFinished(-1);
    LOG << proc.readAllStandardError();
    LOG << proc.readAllStandardOutput();
    return;
}

bool ShellOperation::enterText(QString text)
{
    LOG << "Entering text: " << text;
    QProcess proc;
#ifdef INPUT_STRING
    proc.start(SHELL_CMD_PREFIX, QStringList() << "-c" << QString("input text %1").arg(text));
    proc.waitForFinished(-1);
    QString error = proc.readAllStandardError();
    if(error != ""){
        LOG << "ERROR: " << error;
        return false;
    }else {
        return true;
    }
#else
    for(int i = 0; i < text.length(); i++){
            proc.start(SHELL_CMD_PREFIX, QStringList() << "-c" << QString("input text %1").arg(text.at(i));
        }
        proc.waitForFinished(-1);
    }
    return true;
#endif
}

void ShellOperation::killSpecificApp(QString packageName)
{
    LOG << "Killing " << packageName;
    QProcess proc;
    proc.start(SHELL_CMD_PREFIX, QStringList() << "-c" << QString("am force-stop %1").arg(packageName));
    proc.waitForFinished(-1);
    delay(100);
    return;
}

QPoint ShellOperation::findAnImageOnScreen(QString iconPath)
{
    QString screenImgPath = ShellOperation::screenShot();
    QPoint point = ImageProcessing::findImageOnImage(iconPath,screenImgPath);
    return  point;
}

void ShellOperation::clearPackageData(QString packageName)
{
    LOG << packageName;
    QProcess proc;
    proc.start(SHELL_CMD_PREFIX, QStringList() << "-c" << QString("pm clear %1").arg(packageName));
    proc.waitForFinished(-1);
}

bool ShellOperation::pressTap()
{
    LOG << "Pressing Tap Key ...";
    QProcess proc;
    QProcess process;
    proc.start(SHELL_CMD_PREFIX, QStringList() << "-c" << QString("input keyevent KEYCODE_TAB"));
    proc.waitForFinished(-1);
    QString error = proc.readAllStandardError();
    if(error != ""){
        LOG << "ERROR: " << error;
        return false;
    }else {
        return true;
    }
}

DISPLAY_INFO ShellOperation::getDisplayInfo()
{
    DISPLAY_INFO info;
    QProcess proc;

    proc.start(SHELL_CMD_PREFIX, QStringList() << "-c" << "wm size");
    proc.waitForFinished(-1);
    QString sizeInfo = proc.readAllStandardOutput();

    proc.start(SHELL_CMD_PREFIX, QStringList() << "-c" << "wm density");
    proc.waitForFinished(-1);
    QString dpiInfo = proc.readAllStandardOutput();

    if(sizeInfo.contains("Physical size")){
        info.width = (sizeInfo.split(":").length() < 2? info.width : sizeInfo.split(":").at(1).simplified().split("x").at(0).simplified().toInt());
        info.height = (sizeInfo.split(":").length() < 2? info.height : sizeInfo.split(":").at(1).simplified().split("x").at(1).simplified().toInt());
    }

    if(sizeInfo.contains("Physical density")){
        info.dpi = (sizeInfo.split(":").length() < 2? info.dpi : sizeInfo.split(":").at(1).simplified().toInt());
    }
    return info;
}

void ShellOperation::removeFile(QString path)
{
    QProcess process;
    process.start(SHELL_CMD_PREFIX, QStringList() << "-c" << QString("rm %1").arg(path));
    process.waitForFinished(-1);
}

void ShellOperation::enterKeyBoard()
{
    LOG << "Pressing Tap Key ...";
    QProcess proc;
    QProcess process;
    proc.start(SHELL_CMD_PREFIX, QStringList() << "-c" << QString("input keyevent KEYCODE_ENTER"));
    proc.waitForFinished(-1);
    QString error = proc.readAllStandardError();
    if(error != ""){
        LOG << "ERROR: " << error;
    }else {
        // Do nothing
    }
}

QString ShellOperation::screenShot(QString fileName)
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    if (path.isEmpty()){
        LOG << "Could't not save to " << path;
        return QString("");
    }

    path.append(QString("/%1").arg(fileName));

    QProcess process;
    process.start(SHELL_CMD_PREFIX, QStringList() << "-c" << QString("screencap -p %1").arg(path));
    process.waitForFinished(-1);
    return path;
}
#endif
