#pragma once

#include <Network/Node.h>
#include <Network/Protocol.h>
#include <coppa/oscquery/device/remote.hpp>

namespace OSSIA
{
class OSCQueryClient : public Protocol
{
        public:
          // address of the kind : http://the-server.org:9002
        static std::shared_ptr<OSCQueryClient> create(std::string);

          virtual ~OSCQueryClient() = default;

          OSSIA::Protocol::Type getType() const override final
          { return static_cast<OSSIA::Protocol::Type>(4); }

};
}

namespace coppa
{
namespace ow
{

class OSCQueryClient: public OSSIA::OSCQueryClient
{
        basic_map<oscquery::ParameterMap>
          m_base_map;

        locked_map<basic_map<oscquery::ParameterMap>>
          m_locked_map{m_base_map};

        remote_map_setter<
            locked_map<basic_map<oscquery::ParameterMap>>,
            osc::sender>
          m_setter{m_locked_map};

        oscquery::remote_device m_dev;
    public:
        OSCQueryClient(std::string addr):
            OSSIA::OSCQueryClient{},
            m_dev{m_setter, addr}
        {

        }

        virtual ~OSCQueryClient();

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
};
}
}
