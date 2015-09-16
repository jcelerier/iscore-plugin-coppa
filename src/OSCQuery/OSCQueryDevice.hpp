#pragma once
#include "i-score/base/plugins/iscore-plugin-ossia/Protocols/OSSIADevice.hpp"
class OSCQueryDevice : public OSSIADevice
{
    public:
        OSCQueryDevice(const iscore::DeviceSettings& settings);

        void updateSettings(const iscore::DeviceSettings&) override;
        bool canRefresh() const override;
};
