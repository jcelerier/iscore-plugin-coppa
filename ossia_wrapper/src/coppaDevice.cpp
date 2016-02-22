#include "coppaDevice.hpp"
#include "coppaProtocol.hpp"
#include "coppaNode.hpp"

#include <coppa/coppa.hpp>
#include <Misc/Container.h>
coppa::oscquery::remote_device& coppa::ow::Device::dev()
{
    return m_proto->dev();
}

std::shared_ptr<OSSIA::Device> coppa::ow::Device::getDevice() const
{
    return nullptr;
}

std::shared_ptr<OSSIA::Node> coppa::ow::Device::getParent() const
{
    return nullptr;
}

std::string coppa::ow::Device::getName() const
{
    return "";
}

OSSIA::Node& coppa::ow::Device::setName(std::string)
{
    return *this;
}

std::shared_ptr<OSSIA::Address> coppa::ow::Device::getAddress() const
{
    return nullptr;
}

std::shared_ptr<OSSIA::Address> coppa::ow::Device::createAddress(OSSIA::Value::Type)
{
    return nullptr;
}

bool coppa::ow::Device::removeAddress()
{
    return false;
}

OSSIA::Container<OSSIA::Node>::iterator coppa::ow::Device::emplace(
        OSSIA::Container<OSSIA::Node>::const_iterator,
        std::string)
{
    return {};
}

coppa::ow::Device::Device(std::shared_ptr<coppa::ow::OSCQueryClient> prot):
    m_proto{prot}
{

}

std::shared_ptr<OSSIA::Protocol> coppa::ow::Device::getProtocol() const
{
    return nullptr;
}

#include <boost/algorithm/string/split.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/range/algorithm/find_if.hpp>

void coppa::ow::Device::make_tree_rec(
        const coppa::oscquery::remote_device& remote_dev)
{
    auto dev_ptr = std::dynamic_pointer_cast<coppa::ow::Device>(shared_from_this());

    // TODO LOCKME
    // Print the real parameters in the tree
    for(const auto& elt : remote_dev.map().get_data_map())
    {
        coppa::ow::Node* node = this;
        // Go to the furthest known point in the node hierarchy
        const auto& addr = elt.destination;
        if(addr == "/")
            continue;

        std::vector<std::string> path;
        boost::split(path, addr, boost::lambda::_1 == '/');
        path.erase(path.begin());

        for(int i = 0; i < path.size(); i++)
        {
            const auto& children = node->children();
            auto it = boost::range::find_if(
                        children,
                        [&] (const auto& ossia_node) { return ossia_node->getName() == path[i]; });

            if(it == children.end())
            {
                // We have to start adding sub-nodes from here.
                coppa::ow::Node* parentnode = node;
                for(int k = i; k < path.size(); k++)
                {
                    std::string new_addr;
                    for(int str_idx = 0; str_idx <= k; str_idx++)
                    {
                        new_addr += "/" + path[str_idx];
                    }
                    auto new_node_it = parentnode->emplace(parentnode->children().end(), new_addr);
                    auto n = dynamic_cast<coppa::ow::Node*>(new_node_it->get());
                    n->setDevice(dev_ptr);
                    if(remote_dev.map().has(new_addr))
                        n->createAddress({});
/*
                    auto n = std::make_shared<coppa::ow::Node>(
                                 parentnode->shared_from_this(),
                                 new_addr);

                    n->setDevice(dev_ptr);

                    // We create addresses only if it actually exists.
                    if(remote_dev.has(new_addr))
                        n->createAddress({});

                    parentnode->children().push_back(n);
*/
                    if(k == path.size() - 1)
                    {
                        node = n;
                    }
                    else
                    {
                        parentnode = n;
                    }
                }

                break;
            }
            else
            {
                node = dynamic_cast<coppa::ow::Node*>(it->get());
            }
        }
    }
}

bool coppa::ow::Device::updateNamespace()
{
    if(!m_proto->dev().query_is_connected())
        return false;

    atomic_update_wrapper(
                [&] () { m_proto->dev().query_request_namespace(); },
                m_proto->dev().onUpdate);

    m_children.clear();
    make_tree_rec(m_proto->dev());

    m_proto->dev().onUpdate = nullptr;

    return true;
}

std::shared_ptr<OSSIA::Device> OSSIA::Device::create(
        std::shared_ptr<OSSIA::Protocol> p,
        std::string)
{
    if(auto clt = std::dynamic_pointer_cast<coppa::ow::OSCQueryClient>(p))
    {
        return std::make_shared<coppa::ow::Device>(clt);
    }
    return {};
}

OSSIA::Container<OSSIA::Node>::iterator coppa::ow::Device::insert(
        OSSIA::Container<OSSIA::Node>::const_iterator,
        std::shared_ptr<OSSIA::Node>,
        std::string)
{
    return {};
}
