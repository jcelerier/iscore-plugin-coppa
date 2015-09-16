#include "OSCQueryDevice.hpp"

#include "OSCQuerySpecificSettings.hpp"

#include <coppa/oscquery/device/remote.hpp>
#include "ossia_wrapper/src/coppaDevice.hpp"
#include "ossia_wrapper/src/coppaProtocol.hpp"
#include "OSCQuerySpecificSettings.hpp"

bool OSCQueryDevice::canRefresh() const
{
    return true;
}


OSCQueryDevice::OSCQueryDevice(const iscore::DeviceSettings& settings):
    OSSIADevice{settings}
{
    using namespace OSSIA;

    auto proto = std::make_shared<coppa::ow::OSCQueryClient>(
                settings.deviceSpecificSettings.value<OSCQuerySpecificSettings>().host.toStdString());

    std::this_thread::sleep_for(std::chrono::seconds(1));
    m_dev = std::make_shared<coppa::ow::Device>(proto);
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void OSCQueryDevice::updateSettings(const iscore::DeviceSettings&)
{
}
