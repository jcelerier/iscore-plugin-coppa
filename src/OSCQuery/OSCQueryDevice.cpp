#include "OSCQueryDevice.hpp"

#include "OSCQuerySpecificSettings.hpp"

OSCQueryDevice::OSCQueryDevice(const DeviceSettings &settings):
    DeviceInterface{settings}
{

}

bool OSCQueryDevice::canRefresh() const
{
    return true;
}

Node OSCQueryDevice::refresh()
{
    Node device;

    return device;
}


void OSCQueryDevice::addAddress(const FullAddressSettings &settings)
{
    using namespace OSSIA;
}


void OSCQueryDevice::updateAddress(const FullAddressSettings &settings)
{
    using namespace OSSIA;
}


void OSCQueryDevice::removeAddress(const QString &address)
{
    using namespace OSSIA;
}


void OSCQueryDevice::sendMessage(Message &mess)
{
    qDebug() << Q_FUNC_INFO << "TODO";
}


bool OSCQueryDevice::check(const QString &str)
{
    qDebug() << Q_FUNC_INFO << "TODO";
    return false;
}
