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

    QLabel* portOutputLabel = new QLabel(tr("Port (destination)"), this);
    m_portOutputSBox = new QSpinBox(this);
    m_portOutputSBox->setRange(0, 65535);

    QLabel* localHostLabel = new QLabel(tr("Host"), this);
    m_localHostEdit = new QLineEdit(this);


    QGridLayout* gLayout = new QGridLayout;

    gLayout->addWidget(deviceNameLabel, 0, 0, 1, 1);
    gLayout->addWidget(m_deviceNameEdit, 0, 1, 1, 1);

    gLayout->addWidget(portOutputLabel, 1, 0, 1, 1);
    gLayout->addWidget(m_portOutputSBox, 1, 1, 1, 1);

    gLayout->addWidget(localHostLabel, 2, 0, 1, 1);
    gLayout->addWidget(m_localHostEdit, 2, 1, 1, 1);

    setLayout(gLayout);

    setDefaults();
}

void
OSCQueryProtocolSettingsWidget::setDefaults()
{
    Q_ASSERT(m_deviceNameEdit);
    Q_ASSERT(m_portOutputSBox);
    Q_ASSERT(m_localHostEdit);

    //TODO: we should use QSettings ?

    m_deviceNameEdit->setText("OSCQueryDevice");
    m_portOutputSBox->setValue(9998);
    m_localHostEdit->setText("127.0.0.1");
}

#include "OSCQuerySpecificSettings.hpp"
DeviceSettings OSCQueryProtocolSettingsWidget::getSettings() const
{
    Q_ASSERT(m_deviceNameEdit);

    DeviceSettings s;
    s.name = m_deviceNameEdit->text();

    OSCQuerySpecificSettings minuit;
    minuit.host = m_localHostEdit->text();
    minuit.port = m_portOutputSBox->value();

    s.deviceSpecificSettings = QVariant::fromValue(minuit);
    return s;
}

void
OSCQueryProtocolSettingsWidget::setSettings(const DeviceSettings &settings)
{
    m_deviceNameEdit->setText(settings.name);
    OSCQuerySpecificSettings minuit;
    if(settings.deviceSpecificSettings.canConvert<OSCQuerySpecificSettings>())
    {
        minuit = settings.deviceSpecificSettings.value<OSCQuerySpecificSettings>();
        m_portOutputSBox->setValue(minuit.port);
        m_localHostEdit->setText(minuit.host);
    }
}
