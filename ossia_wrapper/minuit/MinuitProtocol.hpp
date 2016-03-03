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
/*
        remote_map_setter<
            locked_map<basic_map<map_type>>,
            osc::sender>
          m_setter{m_locked_map};
*/
        ossia::minuit_remote_impl_future m_dev;
    public:
        MinuitClient(std::string addr):
            OSSIA::Minuit{},
            m_dev{"rimoute", m_locked_map, 13579, addr, 9998}
        {

        }

        virtual ~MinuitClient();

        bool pullAddressValue(OSSIA::Address&) const override
        {
            return false;
        }

        bool pushAddressValue(const OSSIA::Address&) const override
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

        /*! get IP */
        virtual std::string getIp() {

        }

        virtual Protocol & setIp(std::string) {

        }

        /*! get port where to send messages */
        virtual int getInPort() {

        }

        virtual Protocol & setInPort(int) {

        }

        /*! get port where messages are sent back */
        virtual int getOutPort() {

        }

        virtual Protocol & setOutPort(int) {

        }
};
}
}
