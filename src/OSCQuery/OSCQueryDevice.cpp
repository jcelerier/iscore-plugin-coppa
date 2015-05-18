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


void insertNode(const coppa::oscquery::Parameter& p, Node* rootNode)
{
    QStringList splitted = QString::fromStdString(p.destination).split("/");
    if(splitted.first().isEmpty())
        splitted.removeFirst();
    if(splitted.size() == 1) // It's the root node
    {

        return;
    }

    Node* currentNode = rootNode;
    for(int i = 0; i < splitted.size(); i++)
    {
        auto cdrn = currentNode->children();
        auto beg = cdrn.begin();
        auto end = cdrn.end();
        auto it = std::find_if(beg, end,
                      [&] (Node* n) { return n->name() == splitted[i]; });

        if(it != end)
        {
            if(i == splitted.size() - 1)
            {
                // Set the parameters on the node
                return;
            }
            else
            {
                currentNode = *it;
                continue;
            }
        }
        else
        {
            auto n = new Node;
            n->setName(splitted[i]);
            currentNode->addChild(n);
            if(i == splitted.size() - 1)
            {
                // Create and set the parameters on the node
                return;
            }
            else
            {
                currentNode = n;
                continue;
            }
        }
    }

}
#include <iostream>
using namespace std;
Node OSCQueryDevice::refresh()
{
    m_dev->queryNamespace();
    std::this_thread::sleep_for(std::chrono::milliseconds(2500));

    const auto& theMap = m_dev->safeMap();

    Node device;
    // Print the real parameters in the tree
    for(auto&& elt : theMap.unsafeMap())
    {
        insertNode(elt, &device);
        // Get a string list and create the nodes
    }

    device.setName(settings().name);


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
