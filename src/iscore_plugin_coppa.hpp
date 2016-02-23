#pragma once
#include <QObject>
#include <iscore/plugins/qt_interfaces/PluginRequirements_QtInterface.hpp>
#include <iscore/plugins/qt_interfaces/FactoryInterface_QtInterface.hpp>

class iscore_addon_coppa:
    public QObject,
        public iscore::Plugin_QtInterface,
    public iscore::FactoryInterface_QtInterface
{
        Q_OBJECT
        Q_PLUGIN_METADATA(IID Plugin_QtInterface_iid)
        Q_INTERFACES(
                iscore::Plugin_QtInterface
            iscore::FactoryInterface_QtInterface
        )

    public:
        iscore_addon_coppa();
        ~iscore_addon_coppa();

        std::vector<std::unique_ptr<iscore::FactoryInterfaceBase>> factories(
                const iscore::ApplicationContext& ctx,
                const iscore::AbstractFactoryKey& factoryName) const override;

        iscore::Version version() const override;
        UuidKey<iscore::Plugin> key() const override;
};

