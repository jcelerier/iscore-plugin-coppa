#pragma once
#include <OSSIA/Protocols/OSSIADevice.hpp>
#include <thread>
class OSCQueryDevice : public Ossia::OSSIADevice
{
    private:
        std::thread m_serverThread;
    public:
        OSCQueryDevice(const Device::DeviceSettings& settings);
        ~OSCQueryDevice();

        Device::Node refresh() override;
        bool reconnect() override;
};
