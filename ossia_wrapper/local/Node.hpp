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
namespace Local
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
        OSSIA::Container<OSSIA::Node>::const_iterator it,
        std::string name) override
    {
      coppa::ossia::Parameter p;
      p.destination = m_fullDestination + "/" + name;
      auto& map = m_device->dev().map();
      bool inserted = false;
      int num = 0;
      while(!inserted)
      {
        auto it = map.find(p.destination);
        if(it == map.end())
        {
          map.insert(p);
          inserted = true;
        }
        else
        {
          p.destination = m_fullDestination + "/" + name + "." + std::to_string(num);
        }
      }
      auto node = std::make_shared<node_type>(
                    this->shared_from_this(),
                    m_device,
                    p.destination);
      auto res_it = m_children.insert(it, std::move(node));
      return res_it;
    }

    OSSIA::Container<OSSIA::Node>::iterator insert(
        OSSIA::Container<OSSIA::Node>::const_iterator,
        std::shared_ptr<OSSIA::Node>,
        std::string) override
    {
      return {};
    }

    OSSIA::Container<OSSIA::Node>::iterator emplace(
        OSSIA::Container<OSSIA::Node>::const_iterator it,
        const std::string& name,
        OSSIA::Value::Type t,
        OSSIA::AccessMode = {},
        const std::shared_ptr<OSSIA::Domain>& = {},
        OSSIA::BoundingMode = {},
        bool repetitionFilter = {}) override
    {
      coppa::ossia::Parameter p;
      p.destination = m_fullDestination + "/" + name;
      auto& map = m_device->dev().map();
      bool inserted = false;
      int num = 0;
      while(!inserted)
      {
        auto it = map.find(p.destination);
        if(it == map.end())
        {
          map.insert(p);
          inserted = true;
        }
        else
        {
          p.destination = m_fullDestination + "/" + name + "." + std::to_string(num);
        }
      }
      auto node = std::make_shared<node_type>(
                    this->shared_from_this(),
                    m_device,
                    p.destination);
      auto res_it = m_children.insert(it, std::move(node));
      return res_it;
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
