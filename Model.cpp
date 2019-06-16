#include "Model.h"
#include <QWidgetList>
#include <QtWidgets/qwidget.h>
#include <QWindow>

Model* Model::m_instance = nullptr;

Model::Model(QObject *parent) : QObject(parent)
{
    m_token = "c6a52e63a8d52869b80af5af1a22c2b5";
}

Model *Model::instance()
{
    if(m_instance == nullptr){
        m_instance = new Model();
    }
    return m_instance;
}

void Model::start()
{
    emit sigStartProgram();
}

QString Model::token() const
{
    return m_token;
}

void Model::setToken(QString data)
{
    LOG << data;
    if(m_token != data){
        m_token = data;
        emit tokenChanged();
    }
}

void Model::setDeviceInfo(DEVICE_INFO data)
{
    m_deviceInfo = data;
}



