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
        coppa::oscquery::remote_device m_dev;
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
