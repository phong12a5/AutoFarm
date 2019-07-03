#include "ShellOperation.hpp"

ShellOperation::ShellOperation(QObject *parent) : QObject(parent)
{

}


#ifdef ANDROID_KIT

bool ShellOperation::installPackage(QString packagePath)
{
    LOG << packagePath;
    if(!QFile(packagePath).exists()){
        LOG << "packagePath not existed";
        return false;
    }else{
        QString cmd = QString("su -c pm install %1").arg(packagePath);
        QProcess process;
        process.start(cmd);
        process.waitForFinished(-1);
        return true;
    }
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
    proc.start(QString("su -c input tap %1 %2").arg(point.x()).arg(point.y()));
    proc.waitForFinished(-1);
    proc.waitForFinished(-1);
    QString error = proc.readAllStandardError();
    if(error != ""){
        LOG << "ERROR: " << error;
    }else {
        // Do nothing
    }
    return;
}

bool ShellOperation::enterText(QString text)
{
    LOG << "Entering text: " << text;
    QProcess proc;

#ifdef INPUT_STRING
    proc.start(QString("su -c input text %1").arg(text));
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
        proc.start(QString("su -c input text %1").arg(text.at(i)));
        proc.waitForFinished(-1);
    }
    return true;
#endif
}

void ShellOperation::killSpecificApp(QString packageName)
{
    LOG << "Killing " << packageName;
    QProcess proc;
    proc.start(QString("su -c am force-stop %1").arg(packageName));
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
    proc.start(QString("su -c pm clear %1").arg(packageName));
    proc.waitForFinished(-1);
}

bool ShellOperation::pressTap()
{
    LOG << "Pressing Tap Key ...";
    QProcess proc;
    proc.start(QString("su -c input keyevent KEYCODE_TAB"));
    proc.waitForFinished(-1);
    QString error = proc.readAllStandardError();
    if(error != ""){
        LOG << "ERROR: " << error;
        return false;
    }else {
        return true;
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
    process.start("su", QStringList()<< "-c" << "/system/bin/screencap" << "-p" << path);
    process.waitForFinished(-1);

    return path;
}
#endif
