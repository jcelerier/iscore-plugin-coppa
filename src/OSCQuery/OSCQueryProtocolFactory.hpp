#pragma once
#include <Device/Protocol/ProtocolFactoryInterface.hpp>
#include "OSCQuerySpecificSettings.hpp"
#include "OSCQueryDevice.hpp"
#include "OSCQueryProtocolSettingsWidget.hpp"

class OSCQueryProtocolFactory : public Device::ProtocolFactory
{
        ISCORE_CONCRETE_FACTORY_DECL("882cb66c-06e3-4eb9-9f7d-045e646142a2")
        // Implement with OSSIA::Device
        QString prettyName() const override
        { return "OSCQuery"; }

        Device::DeviceInterface* makeDevice(
                const Device::DeviceSettings& settings,
                const iscore::DocumentContext& ctx) override
        {
            return new OSCQueryDevice{settings};
        }

        const Device::DeviceSettings& defaultSettings() const override
        {
            static const Device::DeviceSettings settings = [&] () {
                Device::DeviceSettings s;
                s.protocol = concreteFactoryKey();
                s.name = "OSCQuery";
                OSCQuerySpecificSettings specif;
                s.deviceSpecificSettings = QVariant::fromValue(specif);
                return s;
            }();
            return settings;
        }

        virtual Device::ProtocolSettingsWidget* makeSettingsWidget() override
        {
            return new OSCQueryProtocolSettingsWidget;
        }

        QVariant makeProtocolSpecificSettings(
                const VisitorVariant& visitor) const override
        {
            return makeProtocolSpecificSettings_T<OSCQuerySpecificSettings>(visitor);
        }

        void serializeProtocolSpecificSettings(
                const QVariant& data,
                const VisitorVariant& visitor) const override
        {
            serializeProtocolSpecificSettings_T<OSCQuerySpecificSettings>(data, visitor);
        }

        bool checkCompatibility(
                const Device::DeviceSettings& a,
                const Device::DeviceSettings& b) const override
        {
            return true;
        }
};
