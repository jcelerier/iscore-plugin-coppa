#pragma once
#include <Network/Protocol/Local.h>
#include <coppa/minuit/device/osc_local_device.hpp>
#include <ossia_wrapper/generic/Address.hpp>
namespace coppa
{
namespace ossia_wrapper
{
namespace Local
{
class Client: public OSSIA::Local
{
    using map_type = ParameterMapType<ossia::Parameter>;
        basic_map<map_type>
          m_base_map;

        locked_map<basic_map<map_type>>
          m_locked_map{m_base_map};

        ossia::osc_local_impl m_dev;
    public:
        using protocol_t = ossia::osc_local_impl;
        using address_impl_t = StandardAddress;
        Client():
            OSSIA::Local{},
            m_dev{"local", m_locked_map, 0, "", 0}
        {

        }

        virtual ~Client();

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

};

}
}
}
