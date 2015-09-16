#include "OSCQueryDevice.hpp"

#include "OSCQuerySpecificSettings.hpp"

#include <coppa/oscquery/device/remote.hpp>

bool OSCQueryDevice::canRefresh() const
{
    return true;
}


void OSCQueryDevice::updateSettings(const iscore::DeviceSettings&)
{
}
