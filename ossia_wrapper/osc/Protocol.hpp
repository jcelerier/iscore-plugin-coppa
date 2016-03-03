#pragma once
#include <Network/Protocol/OSC.h>
#include <coppa/minuit/device/osc_local_device.hpp>
namespace coppa
{
namespace ossia_wrapper
{
namespace OSC
{

class Client: public OSSIA::OSC
{
    using map_type = ParameterMapType<ossia::Parameter>;
        basic_map<map_type>
          m_base_map;

        locked_map<basic_map<map_type>>
          m_locked_map{m_base_map};

        ossia::osc_local_impl m_dev;
    public:
        using protocol_t = ossia::osc_local_impl;
        Client(
            const std::string& addr,
            unsigned int remote_in_p,
            unsigned int local_in_p):
            OSSIA::OSC{},
            m_dev{"remote", m_locked_map, local_in_p, addr, remote_in_p}
        {

        }

        virtual ~Client();

        bool getLearningStatus() const override
        {
          assert(false);
        }

        Protocol & setLearningStatus(OSSIA::Device& ossiaDevice, bool newLearn) override
        {
          assert(false);
        }


        bool pullAddressValue(OSSIA::Address& addr) const override
        {
            return false;
        }

        bool pushAddressValue(const OSSIA::Address& addr) const override
        {
            return false;
        }

        bool observeAddressValue(std::shared_ptr<OSSIA::Address>, bool) const override
        {
            return false;
        }

        bool updateChildren(OSSIA::Node& node) const override
        {
            return false;
        }

        auto& dev() { return m_dev; }
        const auto& dev() const { return m_dev; }

        std::string getIp() const override
        {
          return m_dev.get_remote_ip();
        }

        Protocol & setIp(std::string str) override
        {
          m_dev.set_remote_ip(str);
          return *this;
        }

        int getInPort() const override
        {
          return m_dev.get_remote_input_port();
        }

        Protocol & setInPort(int p) override
        {
          m_dev.set_remote_input_port(p);
          return *this;
        }

        int getOutPort() const override
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
