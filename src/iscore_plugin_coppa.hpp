#pragma once
#include <QObject>
#include <iscore/plugins/qt_interfaces/PluginControlInterface_QtInterface.hpp>
#include <iscore/plugins/qt_interfaces/FactoryInterface_QtInterface.hpp>

class iscore_plugin_coppa:
    public QObject,
    public iscore::PluginControlInterface_QtInterface,
    public iscore::FactoryInterface_QtInterface
{
        Q_OBJECT
        Q_PLUGIN_METADATA(IID PluginControlInterface_QtInterface_iid)
        Q_INTERFACES(
            iscore::PluginControlInterface_QtInterface
            iscore::FactoryInterface_QtInterface
        )

    public:
        iscore_plugin_coppa();
        virtual ~iscore_plugin_coppa() = default;

        virtual iscore::PluginControlInterface* control() override;

        // Contains the OSCQuery factory
        QVector<iscore::FactoryInterface*> factories(const QString& factoryName) override;
};

