#include "coppa.Control.hpp"
coppaControl::coppaControl(iscore::Presenter* pres, QObject* parent):
    iscore::PluginControlInterface {pres, "coppaControl", parent}
{
}
