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

OSSIA::Node&coppa::ow::Device::setName(std::string)
{
    return *this;
}

const std::shared_ptr<OSSIA::Address>&coppa::ow::Device::getAddress() const
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

OSSIA::Container<OSSIA::Node>::iterator coppa::ow::Device::emplace(OSSIA::Container<OSSIA::Node>::const_iterator, std::string)
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

/*
OSSIA::Node *createNodeFromPath(const coppa::Destination &path, OSSIA::Device* dev)
{
    using namespace OSSIA;
    // Find the relevant node to add in the device
    OSSIA::Node* node = dev;
    for(int i = 0; i < path.size(); i++)
    {
        const auto& children = node->children();
        auto it = boost::range::find_if(
                    children,
                    [&] (const auto& ossia_node) { return ossia_node->getName() == path[i].toStdString(); });

        if(it == children.end())
        {
            // We have to start adding sub-nodes from here.
            OSSIA::Node* parentnode = node;
            for(int k = i; k < path.size(); k++)
            {
                auto newNodeIt = parentnode->emplace(parentnode->children().begin(), path[k].toStdString());

                if(k == path.size() - 1)
                {
                    node = newNodeIt->get();
                }
                else
                {
                    parentnode = newNodeIt->get();
                }
            }

            break;
        }
        else
        {
            node = it->get();
        }
    }

    return node;
}


OSSIA::Node* findNodeFromPath(const QStringList& path, OSSIA::Device* dev)
{
    using namespace OSSIA;
    // Find the relevant node to add in the device
    OSSIA::Node* node = dev;
    for(int i = 0; i < path.size(); i++)
    {
        const auto& children = node->children();
        auto it = boost::range::find_if(children,
                                        [&] (const auto& ossia_node)
        { return ossia_node->getName() == path[i].toStdString(); });
        if(it != children.end())
            node = it->get();
        else
            return nullptr;
    }

    return node;
}
*/

#include <boost/algorithm/string/split.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/range/algorithm/find_if.hpp>
void make_tree_rec(
        coppa::ow::Node& root,
        const coppa::oscquery::remote_device& remote_dev)
{
    // Print the real parameters in the tree
    for(const auto& elt : remote_dev.safeMap().unsafeMap())
    {
        coppa::ow::Node* node = &root;
        // Go to the furthest known point in the node hierarchy
        const auto& addr = elt.destination;

        std::vector<std::string> path;
        boost::split(path, addr, boost::lambda::_1 == '/');

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
                    auto n = std::make_shared<coppa::ow::Node>(
                                 std::dynamic_pointer_cast<coppa::ow::Device>(root.getDevice()),
                                 parentnode->shared_from_this(),
                                 addr);

                    parentnode->children().push_back(n);

                    if(k == path.size() - 1)
                    {
                        node = n.get();
                    }
                    else
                    {
                        parentnode = n.get();
                    }
                }

                break;
            }
            else
            {
                node = static_cast<coppa::ow::Node*>(it->get());
            }
        }
    }
}

bool coppa::ow::Device::updateNamespace()
{
    m_proto->dev().queryNamespace();
    m_children.clear();

    // Note : shared_from_this of Device ?
    auto rootnode = std::make_shared<coppa::ow::Node>(
                                     shared_from_this(),
                                     nullptr,
                                     ""); // Root node.

    children().push_back(rootnode);
    make_tree_rec(*rootnode.get(), m_proto->dev());




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
