#pragma once
#include <Misc/Container.h>
#include <Network/Node.h>
#include <Network/Address.h>
#include <Network/Protocol.h>
#include <Network/Device.h>
#include <Editor/Value.h>
#include <Editor/Domain.h>

#include <boost/lambda/lambda.hpp>
#include <boost/algorithm/string/split.hpp>

namespace coppa
{
namespace ow // ossia wrapper
{
class Protocol;
class Address;
class Device;
class Node :
        public virtual OSSIA::Node,
        public std::enable_shared_from_this<coppa::ow::Node>
{
    protected:
        std::shared_ptr<OSSIA::Address> m_address;
        std::shared_ptr<coppa::ow::Device> m_device;
        std::shared_ptr<OSSIA::Node> m_parent;

        std::string m_fullDestination;
        std::string m_name;

    public:
        const std::string& destination() const { return m_fullDestination; }
        Node():
            m_fullDestination{"/"}
        {
            // m_name should be device name
        }

        Node(std::shared_ptr<coppa::ow::Node> parent,
             std::string dest);

        void setDevice(std::shared_ptr<coppa::ow::Device> dev)
        { m_device = dev; }
        std::shared_ptr<OSSIA::Device> getDevice() const override;

        std::shared_ptr<OSSIA::Node> getParent() const override;

        std::string getName() const override;

        OSSIA::Node& setName(std::string) override;

        std::shared_ptr<OSSIA::Address> getAddress() const override;

        std::shared_ptr<OSSIA::Address> createAddress(OSSIA::Value::Type) override;

        bool removeAddress() override;

        OSSIA::Container<OSSIA::Node>::iterator emplace(
                OSSIA::Container<OSSIA::Node>::const_iterator,
                std::string) override;

        OSSIA::Container<OSSIA::Node>::iterator insert(
                OSSIA::Container<OSSIA::Node>::const_iterator,
                std::shared_ptr<OSSIA::Node>,
                std::string) override;
};
}
}
