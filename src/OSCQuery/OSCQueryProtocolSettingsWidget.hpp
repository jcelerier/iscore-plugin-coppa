#pragma once

class QLineEdit;
class QSpinBox;

#include <DeviceExplorer/Protocol/ProtocolSettingsWidget.hpp>

class ZeroconfBrowser;
class OSCQueryProtocolSettingsWidget : public ProtocolSettingsWidget
{
    public:
        OSCQueryProtocolSettingsWidget(QWidget* parent = nullptr);

        virtual iscore::DeviceSettings getSettings() const override;

        virtual void setSettings(const iscore::DeviceSettings& settings) override;

    protected:
        void buildGUI();

        void setDefaults();

    private:
        void on_sessionSelected(const QString& host, int port);

        ZeroconfBrowser* m_zeroconfBrowser{};
        QAction* m_browserAct{};

        QLineEdit* m_deviceNameEdit{};
        QLineEdit* m_localHostEdit{};
};

