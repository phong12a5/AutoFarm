#ifndef APPDEFINES_H
#define APPDEFINES_H

#include <QDebug>
#include <QTest>
#include <QMap>
#include <AppEnums.hpp>
#include <QDir>

#define LOG qDebug() << "[" << __FUNCTION__ << "][" << __LINE__ << "] >>"

#define delay(milescond)        QTest::qSleep(milescond)

#define CURRENT_DIR             QDir::currentPath()+"/"
#define USER_DATA_LIST_PATH     CURRENT_DIR+"userDataList.json"
#define API_SERVER              "https://api.autofarmer.xyz/api/"
#define DCIM_FOLDER             "/sdcard/DCIM/"
#define ENDSCRIPT_PATH          "/sdcard/DCIM/"
#define ENDSCRIPT_FILENAME      "endScript.st"

#define LOADING_ICON            "assets:/images/loadingIcon.png"
#define NEWSFEED_ICON           "assets:/images/newsFeedIcon.png"
#define SKIP_FIND_FRIEND_BTN    "assets:/images/SkipFindFriend.png"
#define OK_BTN_FOR_SAVE_LOGIN   "assets:/images/OKBtnForSaveInfoLogin.png"
#define LOGIN_SCREEN            "assets:/images/FBLoginScreen.png"
#define TURNON_FIND_FRIEND      "assets:/images/TurnOnFindFriendScreen.png"
#define SAVE_LOGIN_SCREEN       "assets:/images/SaveLoginInfoScreen.png"
#define LIKE_ICON               "assets:/images/LikeIcon_FBLite.png"
#define LOGIN_BTN               "assets:/images/LoginBtn.png"
#define EMAIL_FIELD             "assets:/images/MailAndMobileNumberIcon_1.png"
#define PASSWORD_FIELD          "assets:/images/PasswordIcon_1.png"
#define ENGLISH_BTN             "assets:/images/EnglishBtn.png"
#define INCORRECT_PASSWORD      "assets:/images/IncorrectPassowrd.png"
#define CONFIRM_IDENTIFY        "assets:/images/ConfirmIdentifyScreen.png"

#define FACEBOOK_LITE_SCREEN    "com.facebook.lite/com.facebook.lite.MainActivity"
#define FACEBOOK_LITE_PKGNAME   "com.facebook.lite"
#define FACEBOOK_LITE_ACT       "MainActivity"

#define NUMBER_CLICK            2

typedef struct device_info_struct {
    QString googleSF;
    QString androidID;
    QString iMEI;
    QString iMSI;
    QString sIMCardSerial;
    QString wifiMacAddress;
    QString android_verion;
    QString model;
    QString isNox;

    device_info_struct(){
        googleSF = "31b75eb8d";
        androidID = "391f3";
        iMEI = "35697865";
        iMSI = "456789987776";
        sIMCardSerial = "89876656";
        wifiMacAddress = "24:26:38";
        android_verion = "5.0.2";
        model = "Sony Z3";
        isNox = "false";
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