#include "OSCQueryProtocolSettingsWidget.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QApplication>
#include <QAction>
#include <i-score/base/plugins/iscore-plugin-network/Zeroconf/ZeroconfBrowser.hpp>
OSCQueryProtocolSettingsWidget::OSCQueryProtocolSettingsWidget(QWidget* parent)
    : ProtocolSettingsWidget(parent)
{
    m_zeroconfBrowser = new ZeroconfBrowser{"_coppa._tcp", qApp->activeWindow()};
    connect(m_zeroconfBrowser, &ZeroconfBrowser::sessionSelected,
            this, &OSCQueryProtocolSettingsWidget::on_sessionSelected);
    m_browserAct = m_zeroconfBrowser->makeAction();

    buildGUI();
}

void
OSCQueryProtocolSettingsWidget::buildGUI()
{

    QLabel* deviceNameLabel = new QLabel(tr("Device name"), this);
    m_deviceNameEdit = new QLineEdit(this);

    QLabel* localHostLabel = new QLabel(tr("Host"), this);
    m_localHostEdit = new QLineEdit(this);

    QPushButton* browseButton = new QPushButton(tr("Browse"));

    connect(browseButton, &QPushButton::clicked, m_browserAct, [=] () { m_browserAct->trigger(); });

    QGridLayout* gLayout = new QGridLayout;

    gLayout->addWidget(deviceNameLabel, 0, 0, 1, 1);
    gLayout->addWidget(m_deviceNameEdit, 0, 1, 1, 1);

    gLayout->addWidget(localHostLabel, 1, 0, 1, 1);
    gLayout->addWidget(m_localHostEdit, 1, 1, 1, 1);

    gLayout->addWidget(browseButton, 1, 2, 1, 1);

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

void OSCQueryProtocolSettingsWidget::on_sessionSelected(
        const QString& host,
        int port)
{
    QString addr = QString("http://") + host + ":" + QString::number(port);
    m_localHostEdit->setText(addr);

}

#include "OSCQuerySpecificSettings.hpp"
iscore::DeviceSettings OSCQueryProtocolSettingsWidget::getSettings() const
{
    Q_ASSERT(m_deviceNameEdit);

    iscore::DeviceSettings s;
    s.name = m_deviceNameEdit->text();

    OSCQuerySpecificSettings specificsettings;
    specificsettings.host = m_localHostEdit->text();

    s.deviceSpecificSettings = QVariant::fromValue(specificsettings);
    return s;
}

void
OSCQueryProtocolSettingsWidget::setSettings(const iscore::DeviceSettings &settings)
{
    m_deviceNameEdit->setText(settings.name);
    OSCQuerySpecificSettings specificsettings;
    if(settings.deviceSpecificSettings.canConvert<OSCQuerySpecificSettings>())
    {
        specificsettings = settings.deviceSpecificSettings.value<OSCQuerySpecificSettings>();
        m_localHostEdit->setText(specificsettings.host);
    }
}
