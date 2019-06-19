#ifndef APPDEFINES_H
#define APPDEFINES_H

#include <QDebug>
#include <QTest>
#include <QMap>
#include <AppEnums.h>

#define LOG qDebug() << "[" << __FUNCTION__ << "][" << __LINE__ << "] >>"

#define delay(milescond)    QTest::qSleep(milescond)

#define API_SERVER  "https://api.autofarmer.xyz/api/"

#define LOADING_ICON            "assets:/images/loadingIcon.png"
#define PROFILE_ICON            "assets:/images/profileIcon.png"

#define FACEBOOK_LITE_SCREEN    "com.facebook.lite/com.facebook.lite.MainActivity"
#define FACEBOOK_LITE_PKGNAME   "com.facebook.lite"
#define FACEBOOK_LITE_ACT       "com.facebook.lite.MainActivity"

typedef struct device_info_struct {
    QString googleSF;
    QString androidID;
    QString iMEI;
    QString iMSI;
    QString sIMCardSerial;
    QString wifiMacAddress;
    QString android_verion;
    QString model;

    device_info_struct(){
        googleSF = "31b75eb8d";
        androidID = "391f3";
        iMEI = "35697865";
        iMSI = "456789987776";
        sIMCardSerial = "89876656";
        wifiMacAddress = "24:26:38";
        android_verion = "7.1.1";
        model = "Sony Z3";
    }
} DEVICE_INFO;

typedef struct app_config_struct {
    int timeout;
    int reset_3g;
    QString debug_mode;
    int user_type;
    QString encryption;

    app_config_struct(){
        timeout = 30;
        reset_3g = 10;
        debug_mode = "test";
        user_type = 0;
        encryption = "00110001";
    }
} APP_CONFIG;

typedef struct user_data_struct {
    QString _id;
    QString uid;
    QString password;
    QString cookie;
    QString token;
    QString birthday;
    QString name;
    QString sex;
    QString country;
    QString email;
    QString avartar;
    QString created_date;
    QString farming_status;
    QString alive_status;
    QString created_at;
    QString updated_at;
    QString user_id;
} USER_DATA;

#endif // APPDEFINES_H
