#ifndef MYENCRYPT_H
#define MYENCRYPT_H

#include <QObject>
#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include "AppDefines.h"

using namespace std;

class MyEncrypt : public QObject
{
    Q_OBJECT
public:
    explicit MyEncrypt(QObject *parent = nullptr);

private:
    void AddRoundKey(unsigned char * state, unsigned char * roundKey);
    void SubBytes(unsigned char * state);
    void ShiftRows(unsigned char * state);
    void MixColumns(unsigned char * state);
    void Round(unsigned char * state, unsigned char * key);
    void FinalRound(unsigned char * state, unsigned char * key);
    void AESEncrypt(unsigned char * message, unsigned char * expandedKey, unsigned char * encryptedMessage);

public:
    QByteArray encode(/*QByteArray message,QByteArray key*/);

signals:

public slots:
};

#endif // MYENCRYPT_H
