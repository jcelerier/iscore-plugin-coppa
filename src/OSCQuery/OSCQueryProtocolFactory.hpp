#pragma once
#include <DeviceExplorer/Protocol/ProtocolFactoryInterface.hpp>
#include "OSCQuerySpecificSettings.hpp"
#include "OSCQueryDevice.hpp"
#include "OSCQueryProtocolSettingsWidget.hpp"

class OSCQueryProtocolFactory : public ProtocolFactoryInterface
{
        // Implement with OSSIA::Device
        QString name() const override
        { return "OSCQuery"; }

        DeviceInterface* makeDevice(const DeviceSettings& settings) override
        {
            return new OSCQueryDevice{settings};
        }

        virtual ProtocolSettingsWidget* makeSettingsWidget() override
        {
            return new OSCQueryProtocolSettingsWidget;
        }

        QVariant makeProtocolSpecificSettings(const VisitorVariant& visitor) const override
        {
            return makeProtocolSpecificSettings_T<OSCQuerySpecificSettings>(visitor);
        }

        void serializeProtocolSpecificSettings(const QVariant& data, const VisitorVariant& visitor) const override
        {
            serializeProtocolSpecificSettings_T<OSCQuerySpecificSettings>(data, visitor);
        }
};
