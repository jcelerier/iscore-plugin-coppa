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

template<typename DeviceToCheck>
bool atomic_connect_wrapper(const DeviceToCheck& dev, bool& b) // b is for connection failure
{
    std::chrono::milliseconds to(3000);
    static const std::chrono::milliseconds frame(10);

    while(!dev.query_is_connected())
    {
        if(to < std::chrono::milliseconds(0) || !b)
        {
            return false;
        }
        else
        {
            std::this_thread::sleep_for(frame);
            to -= frame;
        }
    }

    return true;
}



template<typename Fun, typename CallbackToCheck>
void atomic_update_wrapper(Fun f, CallbackToCheck& c)
{
    bool b = false;
    c = [&] () {
        b = true;
    };

    f();

    std::chrono::milliseconds to(3000);
    static const std::chrono::milliseconds frame(10);
    while(!b && to > std::chrono::milliseconds(0))
    {
        std::this_thread::sleep_for(frame);
        to -= frame;
    }
}


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
        OSCQueryClient(std::string addr);

        bool pullAddressValue(OSSIA::Address&) const override;
        bool pushAddressValue(const OSSIA::Address&) const override;
        bool observeAddressValue(std::shared_ptr<OSSIA::Address>, bool) const override;
        bool updateChildren(OSSIA::Node& node) const override;

        auto& dev() { return m_dev; }
        const auto& dev() const { return m_dev; }
};
}
}
