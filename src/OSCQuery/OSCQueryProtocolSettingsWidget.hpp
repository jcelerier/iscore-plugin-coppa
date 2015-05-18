#pragma once

class QLineEdit;
class QSpinBox;

#include <DeviceExplorer/Protocol/ProtocolSettingsWidget.hpp>

class OSCQueryProtocolSettingsWidget : public ProtocolSettingsWidget
{
    public:
        OSCQueryProtocolSettingsWidget(QWidget* parent = nullptr);

        virtual DeviceSettings getSettings() const override;

        virtual void setSettings(const DeviceSettings& settings) override;

    protected:
        void buildGUI();

        void setDefaults();

    protected:
        QLineEdit* m_deviceNameEdit;
        QSpinBox* m_portOutputSBox;
        QLineEdit* m_localHostEdit;

};

