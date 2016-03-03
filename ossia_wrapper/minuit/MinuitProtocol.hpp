#pragma once
#include <Network/Protocol/Minuit.h>
#include <coppa/minuit/device/minuit_remote_future.hpp>
namespace coppa
{
namespace minuit_wrapper
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
        MinuitClient(std::string addr):
            OSSIA::Minuit{},
            m_dev{"rimoute", m_locked_map, 13579, addr, 9998}
        {

        }

        virtual ~MinuitClient();

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

        std::string getIp() override
        {
          return m_dev.get_remote_ip();
        }

        Protocol & setIp(std::string str) override
        {
          m_dev.set_remote_ip(str);
          return *this;
        }

        /*! get port where to send messages */
        int getInPort() override
        {
          return m_dev.get_remote_input_port();
        }

        Protocol & setInPort(int p) override
        {
          m_dev.set_remote_input_port(p);
          return *this;
        }

        /*! get port where messages are sent back */
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
