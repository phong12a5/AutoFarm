#include "ImageProcessing.hpp"

ImageProcessing::ImageProcessing(QObject *parent) : QObject(parent)
{

}
#ifdef ANDROID_KIT
QPoint ImageProcessing::findImageOnImage(const QString &smallImagePath, const QString &largeImagePath, float threshold)
{
    QPoint retVal;

//    cv::Mat _smallImage = ImageProcessing::QImage2Mat(QImage(smallImagePath));
//    cv::Mat _largeImage = ImageProcessing::QImage2Mat(QImage(largeImagePath));
    cv::Mat _smallImage = cv::imread(smallImagePath.toUtf8().constData(),CV_LOAD_IMAGE_COLOR);
    cv::Mat _largeImage = cv::imread(largeImagePath.toUtf8().constData(),CV_LOAD_IMAGE_COLOR);

    //kiểm tra kích cỡ của ảnh input & template
    if (_smallImage.rows > _largeImage.rows || _smallImage.cols > _largeImage.cols)
    {
        LOG << "Mat template must be smaller than matInput";
        LOG << "Small image: " << QImage(smallImagePath).size();
        LOG << "Large image: " << QImage(largeImagePath).size();
        return retVal;
    }else if(_smallImage.rows <= 0 || _smallImage.cols <= 0 || _largeImage.rows <= 0 || _largeImage.cols <= 0){
        LOG << "Invalid Image";
        LOG << "Small image: " << QImage(smallImagePath).size();
        LOG << "Large image: " << QImage(largeImagePath).size();
        return retVal;
    }

    cv::Mat result;
    int result_cols = _largeImage.cols - _smallImage.cols + 1;
    int result_rows = _largeImage.rows - _smallImage.rows + 1;
    result.create(result_rows, result_cols, CV_32FC1);

    //tìm mẫu
//    cv::matchTemplate(_largeImage, _smallImage, result, CV_TM_CCORR_NORMED);
    cv::matchTemplate(_largeImage, _smallImage, result, CV_TM_CCOEFF_NORMED);

    cv::threshold(result, result, threshold, 1., CV_THRESH_TOZERO);
    double minval, maxval;
    double bestMaxval = 0;
    //ngưỡng chính xác, giảm xuống thì sẽ tìm được nhiều đối tượng hơn nhưng sai số nhiều hơn

    while (true)
    {
        cv::Point minloc, maxloc;
        cv::minMaxLoc(result, &minval, &maxval, &minloc, &maxloc);

        if (maxval > threshold)
        {
            //vẽ hình chữ nhật lên đối tượng tìm được
            if(maxval > bestMaxval){
                bestMaxval = maxval;
                retVal = QPoint(maxloc.x + _smallImage.cols/2, maxloc.y + _smallImage.rows/2);
            }
            cv::floodFill(result, maxloc, cv::Scalar(0), 0, cv::Scalar(.1), cv::Scalar(1.));

        }
        else
            break;
    }
    LOG << smallImagePath.split("/").last() << " : " << retVal;
    return retVal;
}

QList<QPoint> ImageProcessing::findImageListOnImage(const QString &smallImage, const QString &largeImage, float threshold)
{
    QList<QPoint> retVal;

//    cv::Mat _smallImage = ImageProcessing::QImage2Mat(QImage(smallImagePath));
//    cv::Mat _largeImage = ImageProcessing::QImage2Mat(QImage(largeImagePath));
    cv::Mat _smallImage = cv::imread(smallImagePath.toUtf8().constData(),CV_LOAD_IMAGE_COLOR);
    cv::Mat _largeImage = cv::imread(largeImagePath.toUtf8().constData(),CV_LOAD_IMAGE_COLOR);

    //kiểm tra kích cỡ của ảnh input & template
    if (_smallImage.rows > _largeImage.rows || _smallImage.cols > _largeImage.cols)
    {
        LOG << "Mat template must be smaller than matInput";
        LOG << "Small image: " << QImage(smallImagePath).size();
        LOG << "Large image: " << QImage(largeImagePath).size();
        return retVal;
    }else if(_smallImage.rows <= 0 || _smallImage.cols <= 0 || _largeImage.rows <= 0 || _largeImage.cols <= 0){
        LOG << "Invalid Image";
        LOG << "Small image: " << QImage(smallImagePath).size();
        LOG << "Large image: " << QImage(largeImagePath).size();
        return retVal;
    }

    cv::Mat result;
    int result_cols = _largeImage.cols - _smallImage.cols + 1;
    int result_rows = _largeImage.rows - _smallImage.rows + 1;
    result.create(result_rows, result_cols, CV_32FC1);

    //tìm mẫu
//    cv::matchTemplate(_largeImage, _smallImage, result, CV_TM_CCORR_NORMED);
    cv::matchTemplate(_largeImage, _smallImage, result, CV_TM_CCOEFF_NORMED);

    cv::threshold(result, result, threshold, 1., CV_THRESH_TOZERO);
    double minval, maxval;
    //ngưỡng chính xác, giảm xuống thì sẽ tìm được nhiều đối tượng hơn nhưng sai số nhiều hơn

    while (true)
    {
        cv::Point minloc, maxloc;
        cv::minMaxLoc(result, &minval, &maxval, &minloc, &maxloc);

        if (maxval > threshold)
        {
            retVal << QPoint(maxloc.x + _smallImage.cols/2, maxloc.y + _smallImage.rows/2);
            cv::floodFill(result, maxloc, cv::Scalar(0), 0, cv::Scalar(.1), cv::Scalar(1.));
        }
        else
            break;
    }
    LOG << smallImagePath.split("/").last() << " : " << retVal;
    return retVal;
}

cv::Mat ImageProcessing::QImage2Mat(const QImage &src)
{
    cv::Mat tmp(src.height(),src.width(),CV_8UC3,(uchar*)src.bits(),src.bytesPerLine());
    cv::Mat result; // deep copy just in case (my lack of knowledge with open cv)
    cvtColor(tmp, result,CV_BGR2RGB);
    return result;
}
#endif
