#include "iscore_plugin_coppa.hpp"
#include "coppa.Control.hpp"

#include <src/OSCQuery/OSCQueryProtocolFactory.hpp>

#include <iscore/plugins/customfactory/FactorySetup.hpp>

iscore_addon_coppa::iscore_addon_coppa() :
    QObject {}
{
}

iscore_addon_coppa::~iscore_addon_coppa()
{

}

std::vector<std::unique_ptr<iscore::FactoryInterfaceBase>> iscore_addon_coppa::factories(
        const iscore::ApplicationContext& ctx,
        const iscore::AbstractFactoryKey& key) const
{
    return instantiate_factories<
            iscore::ApplicationContext,
    TL<
        FW<Device::ProtocolFactory,
            OSCQueryProtocolFactory>
    >>(ctx, key);
}

iscore::Version iscore_addon_coppa::version() const
{
    return iscore::Version{1};
}

UuidKey<iscore::Plugin> iscore_addon_coppa::key() const
{
    return "43207798-4e20-439a-a1df-abcac8b20c0c";
}
