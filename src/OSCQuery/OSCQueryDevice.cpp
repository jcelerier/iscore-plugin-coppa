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

iscore::Node OSCQueryDevice::refresh()
{
    auto dev = static_cast<coppa::ow::Device*>(m_dev.get());
    if(!dev->dev().queryConnected())
    {
        bool b = true;
        if(m_serverThread.joinable())
            m_serverThread.join();

        m_serverThread = std::thread([&] {
            dev->dev().queryConnect();
            b = false;
        });

        bool ok = coppa::ow::atomic_connect_wrapper(dev->dev(), b);
    }

    return OSSIADevice::refresh();
}


OSCQueryDevice::OSCQueryDevice(const iscore::DeviceSettings& settings):
    OSSIADevice{settings}
{
    using namespace OSSIA;

    auto proto = std::make_shared<coppa::ow::OSCQueryClient>(
                settings.deviceSpecificSettings.value<OSCQuerySpecificSettings>().host.toStdString());

    bool b = true;
    m_serverThread = std::thread([&] {
        proto->dev().queryConnect();
        b = false;
    });

    bool ok = coppa::ow::atomic_connect_wrapper(proto->dev(), b);
    if(!ok && m_serverThread.joinable())
    {
        m_serverThread.join();
    }
    m_dev = std::make_shared<coppa::ow::Device>(proto);
}

void OSCQueryDevice::updateSettings(const iscore::DeviceSettings&)
{
}
