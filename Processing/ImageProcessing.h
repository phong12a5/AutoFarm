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
#include "AppDefines.h"
#include "QPoint"


class ImageProcessing : public QObject
{
    Q_OBJECT
private:
    explicit ImageProcessing(QObject *parent = nullptr);

public:
#ifdef ANDROID_KIT
    static QPoint findImageOnImage(const QString& smallImage, const QString& largeImage);
    static cv::Mat QImage2Mat(QImage const& src);
#endif

public slots:
};

#endif // IMAGEPROCESSING_H
