#pragma once
#include "i-score/base/plugins/iscore-plugin-ossia/Protocols/OSSIADevice.hpp"
#include <thread>
class OSCQueryDevice : public OSSIADevice
{
    private:
        std::thread m_serverThread;
    public:
        OSCQueryDevice(const iscore::DeviceSettings& settings);
        ~OSCQueryDevice();

        void updateSettings(const iscore::DeviceSettings&) override;
        bool canRefresh() const override;

        iscore::Node refresh() override;
};
