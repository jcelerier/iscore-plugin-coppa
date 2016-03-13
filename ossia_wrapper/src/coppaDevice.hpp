#pragma once
#include <Network/Device.h>
#include <Network/Protocol.h>
#include <Editor/Value.h>
#include <Editor/Domain.h>
#include <coppa/coppa.hpp>
#include <coppa/oscquery/device/remote.hpp>
#include <Misc/Container.h>
#include <boost/algorithm/string/split.hpp>

#include <boost/lambda/lambda.hpp>
#include <boost/range/algorithm/find_if.hpp>
#include <ossia_wrapper/src/AtomicUpdate.hpp>

#include "coppaNode.hpp"
namespace coppa
{
namespace oscquery
{
class remote_device;
}
namespace ossia_wrapper // ossia wrapper
{
namespace OSCQuery
{
class Protocol;

template<typename Protocol_T>
class Device :
        public OSSIA::Device,
        public Node<Device<Protocol_T>>
{
        std::shared_ptr<Protocol_T> m_proto;

    public:
        using node_type = coppa::ossia_wrapper::OSCQuery::Node<Device<Protocol_T>>;
        using protocol_type = typename Protocol_T::protocol_t;
        Device(std::shared_ptr<Protocol_T> prot):
            m_proto{prot}
        {

        }

        virtual ~Device() noexcept
        {

        }

        protocol_type& dev()
        {
            return m_proto->dev();
        }

        std::shared_ptr<OSSIA::Device> getDevice() const override
        {
            return nullptr;
        }
        std::shared_ptr<OSSIA::Node> getParent() const override
        {
            return nullptr;
        }
        std::shared_ptr<OSSIA::Node> getThis() override
        {
            return nullptr;
        }

        std::string getName() const override
        {
            return "";
        }
        OSSIA::Node& setName(std::string) override
        {
            return *this;
        }

        std::shared_ptr<OSSIA::Address> getAddress() const override
        {
            return nullptr;
        }
        std::shared_ptr<OSSIA::Address> createAddress(OSSIA::Value::Type) override
        {
            return nullptr;
        }

        bool removeAddress() override
        {
            return false;
        }

        OSSIA::Container<OSSIA::Node>::iterator emplace(
                OSSIA::Container<OSSIA::Node>::const_iterator,
                std::string) override
        {
            return {};
        }

        OSSIA::Container<OSSIA::Node>::iterator insert(
                OSSIA::Container<OSSIA::Node>::const_iterator,
                std::shared_ptr<OSSIA::Node>,
                std::string) override
        {
            return {};
        }

        std::shared_ptr<OSSIA::Protocol> getProtocol() const override
        {
            return nullptr;
        }
        bool updateNamespace() override
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

    private:
        void make_tree_rec(
                const coppa::oscquery::remote_device& remote_dev)
        {
            auto dev_ptr = std::dynamic_pointer_cast<Device>(this->shared_from_this());

            // TODO LOCKME
            // Print the real parameters in the tree
            for(const auto& elt : remote_dev.map().get_data_map())
            {
                node_type* node = this;
                // Go to the furthest known point in the node hierarchy
                const auto& addr = elt.destination;
                if(addr == "/")
                    continue;

                std::vector<std::string> path;
                boost::split(path, addr, boost::lambda::_1 == '/');
                path.erase(path.begin());

                for(auto i = 0U; i < path.size(); i++)
                {
                    const auto& children = node->children();
                    auto it = boost::range::find_if(
                                  children,
                                  [&] (const auto& ossia_node) { return ossia_node->getName() == path[i]; });

                    if(it == children.end())
                    {
                        // We have to start adding sub-nodes from here.
                        node_type* parentnode = node;
                        for(auto k = i; k < path.size(); k++)
                        {
                            std::string new_addr;
                            for(auto str_idx = 0U; str_idx <= k; str_idx++)
                            {
                                new_addr += "/" + path[str_idx];
                            }
                            auto new_node_it = parentnode->emplace(parentnode->children().end(), new_addr);
                            auto n = dynamic_cast<node_type*>(new_node_it->get());
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
                        node = dynamic_cast<node_type*>(it->get());
                    }
                }
            }
        }
};
}

}
}
