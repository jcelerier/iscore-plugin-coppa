#pragma once
#include <DeviceExplorer/Protocol/DeviceInterface.hpp>

#include "OSCQuerySpecificSettings.hpp"
#include "i-score/base/plugins/iscore-plugin-ossia/Protocols/OSSIADevice.hpp"
#include "ossia_wrapper/src/coppaDevice.hpp"
#include "ossia_wrapper/src/coppaProtocol.hpp"
class OSCQueryDevice : public OSSIADevice
{
    public:
        OSCQueryDevice(const iscore::DeviceSettings& settings):
            OSSIADevice{settings}
        {
            using namespace OSSIA;

            auto proto = std::make_shared<coppa::ow::OSCQueryClient>(
                        settings.deviceSpecificSettings.value<OSCQuerySpecificSettings>().host.toStdString());
            m_dev = std::make_shared<coppa::ow::Device>(proto);
        }

        void updateSettings(const iscore::DeviceSettings&) override;
        bool canRefresh() const override;
};
