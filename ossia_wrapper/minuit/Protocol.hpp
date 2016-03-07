#pragma once
#include <Network/Protocol/Minuit.h>
#include <coppa/ossia/device/minuit_remote_future.hpp>
#include <ossia_wrapper/generic/Address.hpp>
namespace coppa
{
namespace ossia_wrapper
{

namespace Minuit
{
class MinuitClient: public OSSIA::Minuit
{
    using map_type = ParameterMapType<ossia::Parameter>;
    basic_map<map_type>
    m_base_map;

    locked_map<basic_map<map_type>>
    m_locked_map{m_base_map};

    ossia::minuit_remote_impl_future m_dev;
  public:
    using protocol_t = ossia::minuit_remote_impl_future;
    using address_impl_t = PullableAddress;
    MinuitClient(
        const std::string& addr,
        unsigned int remote_in_p,
        unsigned int local_in_p):
      OSSIA::Minuit{},
      m_dev{"remote", m_locked_map, local_in_p, addr, remote_in_p}
    {

    }

    virtual ~MinuitClient();

    bool pullAddressValue(OSSIA::Address& addr) const override
    {
      assert(false);
      return false;
    }

    bool pushAddressValue(const OSSIA::Address& addr) const override
    {
      assert(false);
      return false;
    }

    bool observeAddressValue(std::shared_ptr<OSSIA::Address>, bool) const override
    {
      assert(false);
      return false;
    }

    bool updateChildren(OSSIA::Node& node) const override
    {
      assert(false);
      return false;
    }

    auto& dev() { return m_dev; }
    const auto& dev() const { return m_dev; }

    std::string getIp() override
    {
      return m_dev.get_remote_ip();
    }

    Protocol & setIp(std::string str) override
    {
      m_dev.set_remote_ip(str);
      return *this;
    }

    int getInPort() override
    {
      return m_dev.get_remote_input_port();
    }

    Protocol & setInPort(int p) override
    {
      m_dev.set_remote_input_port(p);
      return *this;
    }

    int getOutPort() override
    {
      return m_dev.get_local_input_port();
    }

    Protocol & setOutPort(int p) override
    {
      m_dev.set_local_input_port(p);
      return *this;
    }
};
}
}
}
