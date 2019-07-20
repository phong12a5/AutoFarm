#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <iostream>
#include <stdio.h>
#include <QObject>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#ifndef ANDROID_KIT
#include <opencv2/imgproc/types_c.h>
#endif
#include "AppDefines.hpp"
#include "QPoint"


class ImageProcessing : public QObject
{
    Q_OBJECT
private:
    explicit ImageProcessing(QObject *parent = nullptr);

public:
    static QPoint findImageOnImage(const QString &smallImagePath, const QString &largeImagePath);
    static float getThreshhold();
    static float getScale();
#ifdef ANDROID_KIT
    static QList<QPoint> findImageListOnImage(const QString &smallImagePath, const QString &largeImagePath);
    static cv::Mat QImage2Mat(QImage const& src);
#endif

public slots:
};

#endif // IMAGEPROCESSING_H
