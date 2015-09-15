#pragma once
#include <Network/Device.h>
#include <Network/Protocol.h>
#include <Editor/Value.h>
#include <Editor/Domain.h>

namespace coppa
{
namespace oscquery
{
class remote_device;
}
namespace ow // ossia wrapper
{
class Protocol;
class OSCQueryClient;
class Device : public OSSIA::Device, public std::enable_shared_from_this<coppa::ow::Device>
{
        std::shared_ptr<coppa::ow::OSCQueryClient> m_proto;

    public:
        coppa::oscquery::remote_device& dev();

        std::shared_ptr<OSSIA::Device> getDevice() const override;

        std::shared_ptr<OSSIA::Node> getParent() const override;

        std::string getName() const override;

        OSSIA::Node&setName(std::string) override;

        const std::shared_ptr<OSSIA::Address>& getAddress() const override;

        std::shared_ptr<OSSIA::Address> createAddress(OSSIA::Value::Type) override;

        bool removeAddress() override;

        OSSIA::Container<OSSIA::Node>::iterator emplace(
                OSSIA::Container<OSSIA::Node>::const_iterator,
                std::string) override;



        Device(std::shared_ptr<coppa::ow::OSCQueryClient> prot);

        std::shared_ptr<OSSIA::Protocol> getProtocol() const override;
        bool updateNamespace() override;
};

}
}
