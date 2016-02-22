#include "iscore_plugin_coppa.hpp"
#include "coppa.Control.hpp"

#include <src/OSCQuery/OSCQueryProtocolFactory.hpp>

#include <iscore/plugins/customfactory/FactorySetup.hpp>

iscore_plugin_coppa::iscore_plugin_coppa() :
    QObject {}
{
}

iscore_plugin_coppa::~iscore_plugin_coppa()
{

}

std::vector<std::unique_ptr<iscore::FactoryInterfaceBase>> iscore_plugin_coppa::factories(
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

iscore::Version iscore_plugin_coppa::version() const
{
    return iscore::Version{1};
}

UuidKey<iscore::Plugin> iscore_plugin_coppa::key() const
{
    return "43207798-4e20-439a-a1df-abcac8b20c0c";
}
