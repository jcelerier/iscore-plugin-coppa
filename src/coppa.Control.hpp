#pragma once
#include <iscore/plugins/plugincontrol/PluginControlInterface.hpp>
class coppaControl : public iscore::PluginControlInterface
{
    public:
        coppaControl(iscore::Presenter* pres, QObject* parent);
};
