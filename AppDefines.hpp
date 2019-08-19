#ifndef APPDEFINES_H
#define APPDEFINES_H

#include <QDebug>
#include <QTest>
#include <AppEnums.hpp>
#include <QDir>

#define delay(milescond)        QTest::qSleep(milescond)

#define CURRENT_DIR             QDir::currentPath()+"/"
#define USER_DATA_LIST_PATH     CURRENT_DIR+"userDataList.json"
#define CONFIG_FILE_PATH        CURRENT_DIR+"config.json"

#define API_SERVER              "https://api.autofarmer.xyz/api/"
#define DCIM_FOLDER             "/sdcard/DCIM/"
#define CHECKING_SCREEN_IMG     "/sdcard/DCIM/screen_checking.png"
#define SCREEN_IMG              "/sdcard/DCIM/screen.png"
#define ENDSCRIPT_FILENAME      "endScript.st"
#define INITSCRIPT_FILENAME     "/sdcard/DCIM/initApp.st"

#define LOADING_ICON            "/sdcard/DCIM/PDT17/Icons/loadingIcon.png"
#define LOADING_SCREEN          "/sdcard/DCIM/PDT17/Icons/loadingScreen.png"
#define LOADING_SCREEN2         "/sdcard/DCIM/PDT17/Icons/loadingScreen2.png"
#define NEWSFEED_ICON           "/sdcard/DCIM/PDT17/Icons/NewFeedsIcon.png"
#define NEWSFEED_ICON2          "/sdcard/DCIM/PDT17/Icons/NewFeedsIcon2.png"
#define SKIP_FIND_FRIEND_BTN    "/sdcard/DCIM/PDT17/Icons/SkipFindFriend.png"
#define OK_BUTTON               "/sdcard/DCIM/PDT17/Icons/OKButton.png"
#define TURNON_FIND_FRIEND      "/sdcard/DCIM/PDT17/Icons/TurnOnFindFriendScreen.png"
#define SAVE_LOGIN_SCREEN       "/sdcard/DCIM/PDT17/Icons/SaveLoginInfoScreen.png"
#define LIKE_ICON               "/sdcard/DCIM/PDT17/Icons/LikeIcon_FBLite.png"
#define LOGIN_BTN               "/sdcard/DCIM/PDT17/Icons/LoginBtn.png"
#define LOGIN_SCREEN            "/sdcard/DCIM/PDT17/Icons/Login_screen.png"
#define EMAIL_FIELD             "/sdcard/DCIM/PDT17/Icons/MailAndMobileNumberIcon.png"
#define PASSWORD_FIELD          "/sdcard/DCIM/PDT17/Icons/PasswordIcon.png"
#define ENGLISH_BTN             "/sdcard/DCIM/PDT17/Icons/EnglishBtn.png"
#define INCORRECT_PASSWORD      "/sdcard/DCIM/PDT17/Icons/IncorrectPassowrd.png"
#define CONFIRM_IDENTIFY        "/sdcard/DCIM/PDT17/Icons/ConfirmIdentifyScreen.png"
#define CONFIRM_IDENTIFY2       "/sdcard/DCIM/PDT17/Icons/ConfirmIdentifyScreen2.png"
#define CHOOSE_AVATAR           "/sdcard/DCIM/PDT17/Icons/ChooseAvt.png"
#define SKIP_AVARTAR            "/sdcard/DCIM/PDT17/Icons/SkipAvatar.png"
#define ADD_FRIEND_SUGGESTION   "/sdcard/DCIM/PDT17/Icons/AddFriendScreen.png"
#define ADD_FRIEND_SUGGESTION2  "/sdcard/DCIM/PDT17/Icons/AddFriendScreen-2.png"
#define DOWNLOAD_INFO_BTN       "/sdcard/DCIM/PDT17/Icons/DownloadYourInfomation.png"
#define MISSING_PWD_SCREEN      "/sdcard/DCIM/PDT17/Icons/MissingPasswordIcon.png"
#define SELECT_LANGUAGE_SCREEN  "/sdcard/DCIM/PDT17/Icons/SelectLanguage.png"
#define LOGIN_AGAIN_ICON        "/sdcard/DCIM/PDT17/Icons/LoginAgain.png"
#define ACC_NOT_FOUND           "/sdcard/DCIM/PDT17/Icons/AccountNotFound.png"

#define FACEBOOK_LITE_SCREEN    "com.facebook.lite/com.facebook.lite.MainActivity"
#define FACEBOOK_LITE_PKGNAME   "com.facebook.lite"
#define FACEBOOK_LITE_ACT       "MainActivity"

#define TOKEN_PROP_KEY          "farm_token"
#define AUTO_START_KEY          "auto_start"

#define NUMBER_CLICK            5


struct USER_DATA {
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

    bool operator ==(USER_DATA& input){
        if(this->_id            == input._id            &&\
           this->uid            == input.uid            &&\
           this->password       == input.password       &&\
           this->cookie         == input.cookie         &&\
           this->token          == input.token          &&\
           this->birthday       == input.birthday       &&\
           this->name           == input.name           &&\
           this->sex            == input.sex            &&\
           this->country        == input.country        &&\
           this->email          == input.email          &&\
           this->avartar        == input.avartar        &&\
           this->created_date   == input.created_date   &&\
           this->farming_status == input.farming_status &&\
           this->alive_status   == input.alive_status   &&\
           this->created_at     == input.created_at     &&\
           this->updated_at     == input.updated_at     &&\
           this->user_id        == input.user_id)
        {
            return true;
        }else{
            return false;
        }
    }

    bool operator !=(USER_DATA& input){
        if(this->_id            != input._id            ||\
           this->uid            != input.uid            ||\
           this->password       != input.password       ||\
           this->cookie         != input.cookie         ||\
           this->token          != input.token          ||\
           this->birthday       != input.birthday       ||\
           this->name           != input.name           ||\
           this->sex            != input.sex            ||\
           this->country        != input.country        ||\
           this->email          != input.email          ||\
           this->avartar        != input.avartar        ||\
           this->created_date   != input.created_date   ||\
           this->farming_status != input.farming_status ||\
           this->alive_status   != input.alive_status   ||\
           this->created_at     != input.created_at     ||\
           this->updated_at     != input.updated_at     ||\
           this->user_id        != input.user_id)
        {
            return true;
        }else{
            return false;
        }
    }
};

#endif // APPDEFINES_H
