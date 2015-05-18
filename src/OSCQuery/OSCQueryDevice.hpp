#pragma once
#include <DeviceExplorer/Protocol/DeviceInterface.hpp>

namespace coppa
{
namespace oscquery
{
class RemoteDevice;
}
}

class OSCQueryDevice : public DeviceInterface
{
    public:
        OSCQueryDevice(const DeviceSettings& settings);
        bool canRefresh() const override;
        Node refresh() override;

        void addAddress(const FullAddressSettings& settings) override;
        void updateAddress(const FullAddressSettings& address) override;
        void removeAddress(const QString& path) override;

        void sendMessage(Message& mess) override;
        bool check(const QString& str) override;

    private:
        coppa::oscquery::RemoteDevice* m_dev{};

};
