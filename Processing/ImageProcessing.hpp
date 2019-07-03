#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <iostream>
#include <stdio.h>
#include <QObject>
#ifdef ANDROID_KIT
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#endif
#include "AppDefines.hpp"
#include "QPoint"


class ImageProcessing : public QObject
{
    Q_OBJECT
private:
    explicit ImageProcessing(QObject *parent = nullptr);

public:
#ifdef ANDROID_KIT
    static QPoint findImageOnImage(const QString &smallImagePath, const QString &largeImagePath);
    static QList<QPoint> findImageListOnImage(const QString &smallImagePath, const QString &largeImagePath);
    static cv::Mat QImage2Mat(QImage const& src);
    static float getThreshhold();
    static float getScale();
#endif

public slots:
};

#endif // IMAGEPROCESSING_H
