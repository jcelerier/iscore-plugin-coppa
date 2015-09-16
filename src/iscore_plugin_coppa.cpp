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

QVector<iscore::FactoryInterface*> iscore_plugin_coppa::factories(const QString& factoryName)
{
    if(factoryName == "Protocol")
    {
        return {new OSCQueryProtocolFactory};
    }

    return {};
}


iscore::PluginControlInterface*iscore_plugin_coppa::make_control(iscore::Presenter* parent)
{
    return new coppaControl{parent, parent};
}
