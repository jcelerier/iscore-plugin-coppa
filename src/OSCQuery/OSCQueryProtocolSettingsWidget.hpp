#pragma once

class QLineEdit;
class QSpinBox;

#include <DeviceExplorer/Protocol/ProtocolSettingsWidget.hpp>

class OSCQueryProtocolSettingsWidget : public ProtocolSettingsWidget
{
    public:
        OSCQueryProtocolSettingsWidget(QWidget* parent = nullptr);

        virtual iscore::DeviceSettings getSettings() const override;

        virtual void setSettings(const iscore::DeviceSettings& settings) override;

    protected:
        void buildGUI();

        void setDefaults();

    protected:
        QLineEdit* m_deviceNameEdit;
        QLineEdit* m_localHostEdit;

};

