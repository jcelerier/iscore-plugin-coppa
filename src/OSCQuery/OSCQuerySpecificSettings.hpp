#pragma once
#include <QString>
#include <QMetaType>

struct OSCQuerySpecificSettings
{
    int port{};
    QString host;
};
Q_DECLARE_METATYPE(OSCQuerySpecificSettings)
