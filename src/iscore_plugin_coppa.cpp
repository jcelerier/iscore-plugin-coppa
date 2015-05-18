#include "iscore_plugin_coppa.hpp"
#include "coppa.Control.hpp"

#include "OSCQuery/OSCQueryProtocolFactory.hpp"


iscore_plugin_coppa::iscore_plugin_coppa() :
    QObject {}
{
}

iscore_plugin_coppa::~iscore_plugin_coppa()
{

}

iscore::PluginControlInterface* iscore_plugin_coppa::control()
{
    return new coppaControl{nullptr};
}



QVector<iscore::FactoryInterface*> iscore_plugin_coppa::factories(const QString& factoryName)
{
    if(factoryName == "Protocol")
    {
        return {new OSCQueryProtocolFactory};
    }

    return {};
}
