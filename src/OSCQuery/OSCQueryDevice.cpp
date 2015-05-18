#include "OSCQueryDevice.hpp"

#include "OSCQuerySpecificSettings.hpp"

#include <coppa/oscquery/device/remote.hpp>

OSCQueryDevice::OSCQueryDevice(const DeviceSettings &settings):
    DeviceInterface{settings}
{
    m_dev = new coppa::oscquery::RemoteDevice("http://127.0.0.1:9002/");

}

bool OSCQueryDevice::canRefresh() const
{
    return true;
}

#include <iostream>
using namespace std;
Node OSCQueryDevice::refresh()
{
    m_dev->queryNamespace();

    const auto& theMap = m_dev->safeMap();
    cout << endl << "Current addresses: " << endl;

    // Print the real parameters in the tree
    for(auto&& elt : theMap.unsafeMap())
    {
        cout << elt.destination << endl;
    }

    Node device;

    return device;
}


void OSCQueryDevice::addAddress(const FullAddressSettings &settings)
{
}


void OSCQueryDevice::updateAddress(const FullAddressSettings &settings)
{
}


void OSCQueryDevice::removeAddress(const QString &address)
{
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
