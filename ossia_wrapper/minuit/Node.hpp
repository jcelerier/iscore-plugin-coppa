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
namespace ossia_wrapper // ossia wrapper
{
template<typename Node_T, typename Impl>
class Address;
namespace Minuit
{
class Protocol;

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

    auto& get_children()
    { return m_children; }
    const auto& device() const
    { return m_device; }

    Node(const std::shared_ptr<Node>& parent,
         const std::shared_ptr<Device_T>& dev,
         const std::string& dest):
      m_parent{parent},
      m_device{dev},
      m_fullDestination{dest}
    {

      if(dest.find("//") != std::string::npos)
      {
        assert(false);
      }
      std::vector<std::string> vec;
      boost::split(vec, m_fullDestination, [] (char c) { return c == '/';});

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

    std::shared_ptr<OSSIA::Address> createAddress(OSSIA::Value::Type t) override
    {
      using address_impl_t = typename device_type::ossia_protocol_t::address_impl_t;
      // We don't change the address for now.
      if(!m_address)
      {
        m_address = std::make_shared<
                    Address<node_type, address_impl_t>>(this->shared_from_this());
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
        OSSIA::Container<OSSIA::Node>::const_iterator,
        std::shared_ptr<OSSIA::Node>,
        std::string) override
    {
      assert(false);
      return {};
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
        OSSIA::Container<OSSIA::Node>::const_iterator) override
    {
      assert(false);
      return {};
    }
};
}
}
}
