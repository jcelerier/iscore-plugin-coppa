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
namespace ossia_wrapper // ossia wrapper
{
namespace OSC
{
class Protocol;
template<typename Node_T>
class Address;

template<typename Device_T> // coppa::ow::Device
class Node :
        public virtual OSSIA::Node,
        public std::enable_shared_from_this<Node<Device_T>>
{
    protected:
        std::shared_ptr<OSSIA::Address> m_address;
        std::shared_ptr<Device_T> m_device;
        std::shared_ptr<OSSIA::Node> m_parent;

        std::string m_fullDestination;
        std::string m_name;

    public:
        using node_type = Node<Device_T>;
        using device_type = Device_T;
        const std::string& destination() const { return m_fullDestination; }
        Node():
            m_fullDestination{"/"}
        {

        }

        virtual ~Node()
        {

        }

        Node(const std::shared_ptr<Node>& parent,
             const std::shared_ptr<Device_T>& dev,
             const std::string& dest):
          m_parent{parent},
          m_device{dev},
          m_fullDestination{dest}
        {
          std::vector<std::string> vec;
          boost::split(vec, m_fullDestination, boost::lambda::_1 == '/');

          m_name = vec.back();
        }

        void setDevice(std::shared_ptr<device_type> dev)
        {
            m_device = dev;
        }

        std::shared_ptr<OSSIA::Node> getParent() const override
        {
            return m_parent;
        }

        std::shared_ptr<OSSIA::Device> getDevice() const override
        {
            return m_device;
        }

        std::string getName() const override
        {
            return m_name;
        }

        OSSIA::Node& setName(std::string) override
        {
            // Remove the old parameter and create a new one with the new name
            return *this;
        }

        std::shared_ptr<OSSIA::Address> getAddress() const override
        {
            return m_address;
        }

        std::shared_ptr<OSSIA::Address> createAddress(OSSIA::Value::Type) override
        {
            // We don't change the address for now.
            if(!m_address)
                m_address = std::make_shared<Address<node_type>>(this->shared_from_this());
            return m_address;
        }

        bool removeAddress() override
        {
          bool res(m_address);
          m_address.reset();
          return res;
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

        OSSIA::Container<OSSIA::Node>::iterator emplace(
                OSSIA::Container<OSSIA::Node>::const_iterator,
                const std::string&,
                OSSIA::Value::Type,
                OSSIA::AccessMode = {},
                const std::shared_ptr<OSSIA::Domain>& = {},
                OSSIA::BoundingMode = {},
                bool repetitionFilter = {}) override
        {
            return {};
        }

        OSSIA::Container<OSSIA::Node>::iterator erase(
                OSSIA::Container<OSSIA::Node>::const_iterator) override
        {
            return {};
        }
};
}
}
}
