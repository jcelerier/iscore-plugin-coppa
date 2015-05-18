#pragma once
#include <QString>
#include <QMetaType>

struct OSCQuerySpecificSettings
{
    QString host;
};
Q_DECLARE_METATYPE(OSCQuerySpecificSettings)
