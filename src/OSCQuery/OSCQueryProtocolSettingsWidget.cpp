#include "OSCQueryProtocolSettingsWidget.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>

OSCQueryProtocolSettingsWidget::OSCQueryProtocolSettingsWidget(QWidget* parent)
    : ProtocolSettingsWidget(parent)
{
    buildGUI();
}

void
OSCQueryProtocolSettingsWidget::buildGUI()
{
    QLabel* deviceNameLabel = new QLabel(tr("Device name"), this);
    m_deviceNameEdit = new QLineEdit(this);

    QLabel* localHostLabel = new QLabel(tr("Host"), this);
    m_localHostEdit = new QLineEdit(this);


    QGridLayout* gLayout = new QGridLayout;

    gLayout->addWidget(deviceNameLabel, 0, 0, 1, 1);
    gLayout->addWidget(m_deviceNameEdit, 0, 1, 1, 1);

    gLayout->addWidget(localHostLabel, 1, 0, 1, 1);
    gLayout->addWidget(m_localHostEdit, 1, 1, 1, 1);

    setLayout(gLayout);

    setDefaults();
}

void
OSCQueryProtocolSettingsWidget::setDefaults()
{
    Q_ASSERT(m_deviceNameEdit);
    Q_ASSERT(m_localHostEdit);

    //TODO: we should use QSettings ?

    m_deviceNameEdit->setText("OSCQueryDevice");
    m_localHostEdit->setText("http://127.0.0.1:9002");
}

#include "OSCQuerySpecificSettings.hpp"
DeviceSettings OSCQueryProtocolSettingsWidget::getSettings() const
{
    Q_ASSERT(m_deviceNameEdit);

    DeviceSettings s;
    s.name = m_deviceNameEdit->text();

    OSCQuerySpecificSettings specificsettings;
    specificsettings.host = m_localHostEdit->text();

    s.deviceSpecificSettings = QVariant::fromValue(specificsettings);
    return s;
}

void
OSCQueryProtocolSettingsWidget::setSettings(const DeviceSettings &settings)
{
    m_deviceNameEdit->setText(settings.name);
    OSCQuerySpecificSettings specificsettings;
    if(settings.deviceSpecificSettings.canConvert<OSCQuerySpecificSettings>())
    {
        specificsettings = settings.deviceSpecificSettings.value<OSCQuerySpecificSettings>();
        m_localHostEdit->setText(specificsettings.host);
    }
}
