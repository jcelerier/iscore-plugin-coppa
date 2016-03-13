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
#include <ossia_wrapper/ossia.hpp>
namespace coppa
{
namespace ossia_wrapper
{
template<typename Node_T, typename Impl>
class Address;

template<typename Device_T>
class Node :
    public virtual OSSIA::Node,
    public std::enable_shared_from_this<Node<Device_T>>
{
  protected:
    std::shared_ptr<OSSIA::Address> m_address;
    std::weak_ptr<Device_T> m_device;
    std::weak_ptr<OSSIA::Node> m_parent;

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

    const auto& device() const
    { return m_device; }

    auto& get_children()
    { return m_children; }

    Node(const std::shared_ptr<Node>& parent,
         const std::shared_ptr<Device_T>& dev,
         const std::string& dest):
      m_device{dev},
      m_parent{parent},
      m_fullDestination{dest}
    {
      std::vector<std::string> vec;
      boost::split(vec, m_fullDestination, [] (char c) { return c == '/';});

      m_name = vec.back();
    }

    void setDevice(std::shared_ptr<device_type> dev)
    {
      m_device = std::move(dev);
    }

    std::shared_ptr<OSSIA::Node> getParent() const override
    {
      return m_parent.lock();
    }

    std::shared_ptr<OSSIA::Node> getThis() override
    {
        return this->shared_from_this();
    }

    std::shared_ptr<OSSIA::Device> getDevice() const override
    {
      return m_device.lock();
    }

    std::string getName() const override
    {
      return m_name;
    }

    OSSIA::Node& setName(std::string) override
    {
      // Remove the old parameter and create a new one with the new name
      assert(false);
      return *this;
    }

    std::shared_ptr<OSSIA::Address> getAddress() const override
    {
      return m_address;
    }

    std::shared_ptr<OSSIA::Address> createAddress(OSSIA::Value::Type) override
    {
      using address_impl_t = typename device_type::ossia_protocol_t::address_impl_t;
      // We don't change the address for now.
      if(!m_address)
      {
        m_address = std::make_shared<Address<node_type, address_impl_t>>(this->shared_from_this());
      }
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
      return ossia_wrapper::emplace(*this, it, name, {});
    }

    OSSIA::Container<OSSIA::Node>::iterator insert(
        OSSIA::Container<OSSIA::Node>::const_iterator it,
        std::shared_ptr<OSSIA::Node> node,
        std::string name) override
    {
      // 1. Create a coppa parameter from this node if it has an address.
      assert(false);

      // 2. Add the node locally.
      auto res_it = m_children.insert(it, std::move(node));

      return res_it;
    }

    OSSIA::Container<OSSIA::Node>::iterator emplace(
        OSSIA::Container<OSSIA::Node>::const_iterator it,
        const std::string& name,
        OSSIA::Value::Type t,
        OSSIA::AccessMode am = {},
        const std::shared_ptr<OSSIA::Domain>& d = {},
        OSSIA::BoundingMode bm = {},
        bool repetitionFilter = {}) override
    {
      return ossia_wrapper::emplace(*this, it, name, t, am, d, bm, repetitionFilter);
    }

    OSSIA::Container<OSSIA::Node>::iterator erase(
        OSSIA::Container<OSSIA::Node>::const_iterator it) override
    {
      if(it != m_children.end())
      {
        auto res_it = m_children.erase(it);
        return res_it;
      }
      throw std::runtime_error("Node::erase : Invalid iterator");
    }

    void register_children(const std::shared_ptr<OSSIA::Node>& node)
    {
      auto ptr = dynamic_cast<Node*>(node.get());
      m_children.insert(ptr->destination(), node);
    }

};
}
}
