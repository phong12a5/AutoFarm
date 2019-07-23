#include "ShellOperation.hpp"
#include "Model.hpp"

#define MODEL           Model::instance()

ShellOperation::ShellOperation(QObject *parent) : QObject(parent)
{

}

bool ShellOperation::shellCommand(QString cmd)
{
       int retVal = QProcess::execute("su", QStringList() << "-c" << cmd);
       LOG << retVal << " : " << cmd;
       return retVal == 0;
}

bool ShellOperation::shellCommand(QString cmd, QString &output)
{
    QProcess process;
    process.start("su", QStringList() << "-c" << cmd);
    process.waitForFinished(-1);
    output = process.readAllStandardOutput();
    return process.exitCode() == 0;
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
        return ShellOperation::shellCommand(QString("pm install %1").arg(packagePath));
    }
}

void ShellOperation::callScrollEvent(QPoint point1, QPoint point2)
{
    LOG << QString("Scrolling from [%1,%2] to [%3,%4]")\
           .arg(point1.x()).arg(point1.y())\
           .arg(point2.x()).arg(point2.y());

    ShellOperation::shellCommand(QString("input swipe %1 %2 %3 %4").arg(QString::number(point1.x())).\
                                 arg(QString::number(point1.y())).\
                                 arg(QString::number(point2.x())).\
                                 arg(QString::number(point2.y())));
}

QString ShellOperation::getCurrentActivity()
{
    QString retVal = "";
    QString output;
    ShellOperation::shellCommand(QString("dumpsys window windows | grep -E 'mCurrentFocus'"),output);
    QStringList outputList = output.split(' ');
    if(outputList.length() > 4){
        retVal = QString(outputList.at(4)).simplified().remove('}');
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
    ShellOperation::shellCommand(QString("input tap %1 %2").arg(point.x()).arg(point.y()));
}

bool ShellOperation::enterText(QString text)
{
    LOG << "Entering text: " << text;
    return ShellOperation::shellCommand(QString("input text %1").arg(text));
}

void ShellOperation::killSpecificApp(QString packageName)
{
    LOG << "Killing " << packageName;
    ShellOperation::shellCommand(QString("am force-stop %1").arg(packageName));
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
    ShellOperation::shellCommand(QString("pm clear %1").arg(packageName));
}

bool ShellOperation::pressTap()
{
    LOG << "Pressing Tap Key ...";
    return ShellOperation::shellCommand("input keyevent KEYCODE_TAB");
}

DISPLAY_INFO ShellOperation::getDisplayInfo()
{
    DISPLAY_INFO info;

    QString sizeInfo;
    ShellOperation::shellCommand("wm size",sizeInfo);

    QString dpiInfo;
    ShellOperation::shellCommand("wm density",dpiInfo);

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
    ShellOperation::shellCommand(QString("rm %1").arg(path));
}

void ShellOperation::enterKeyBoard()
{
    LOG << "Pressing Tap Key ...";
    ShellOperation::shellCommand("input keyevent KEYCODE_ENTER");
}

QString ShellOperation::screenShot(QString fileName)
{
//    QString path = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
//    if (path.isEmpty()){
//        LOG << "Could't not save to " << path;
//        return QString("");
//    }
    QString path = "/sdcard/Pictures/screen.png";

//    path.append(QString("/%1").arg(fileName));
    ShellOperation::shellCommand(QString("screencap -p %1").arg(path));
    return path;
}
#endif
