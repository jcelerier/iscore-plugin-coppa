#include "OSCQueryDevice.hpp"

#include "OSCQuerySpecificSettings.hpp"

#include <coppa/oscquery/device/remote.hpp>
#include <ossia_wrapper/src/coppaDevice.hpp>
#include <ossia_wrapper/src/coppaProtocol.hpp>
#include <ossia_wrapper/src/coppaAddress.hpp>
#include "OSCQuerySpecificSettings.hpp"

using device_t = coppa::ossia_wrapper::OSCQuery::Device<coppa::ossia_wrapper::OSCQuery::OSCQueryClient>;
Device::Node OSCQueryDevice::refresh()
{
    auto dev = static_cast<device_t*>(m_dev.get());
    if(!dev->dev().query_is_connected())
    {
        bool b = true;
        if(m_serverThread.joinable())
            m_serverThread.join();

        m_serverThread = std::thread([&] {
            dev->dev().query_connect();
            b = false;
        });

        bool ok = coppa::ossia_wrapper::atomic_connect_wrapper(dev->dev(), b);
    }

    return OSSIADevice::refresh();
}

bool OSCQueryDevice::reconnect()
{
    return true;
}


OSCQueryDevice::OSCQueryDevice(const Device::DeviceSettings& settings):
    OSSIADevice{settings}
{
    using namespace OSSIA;

    m_capas.canRefreshTree = true;
    auto proto = std::make_shared<coppa::ossia_wrapper::OSCQuery::OSCQueryClient>(
                settings.deviceSpecificSettings.value<OSCQuerySpecificSettings>().host.toStdString());

    bool b = true;
    m_serverThread = std::thread([&] {
        proto->dev().query_connect();
        b = false;
    });

    bool ok = coppa::ossia_wrapper::atomic_connect_wrapper(proto->dev(), b);
    if(!ok && m_serverThread.joinable())
    {
        m_serverThread.join();
    }
    m_dev = std::make_shared<device_t>(proto);
}


OSCQueryDevice::~OSCQueryDevice()
{
  auto coppa_dev = static_cast<device_t*>(m_dev.get());
  if(coppa_dev->dev().query_is_connected())
    coppa_dev->dev().query_close();

  if(m_serverThread.joinable())
    m_serverThread.join();
}
