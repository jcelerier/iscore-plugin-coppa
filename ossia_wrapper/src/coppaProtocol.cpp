#include "coppaProtocol.hpp"

coppa::ow::OSCQueryClient::OSCQueryClient(std::string addr):
    OSSIA::OSCQueryClient{},
    m_dev{addr}
{

}

bool coppa::ow::OSCQueryClient::pullAddressValue(OSSIA::Address&) const
{
    return false;

}

bool coppa::ow::OSCQueryClient::pushAddressValue(const OSSIA::Address&) const
{
    return false;

}

bool coppa::ow::OSCQueryClient::observeAddressValue(std::shared_ptr<OSSIA::Address>, bool) const
{
    return false;

}

std::shared_ptr<OSSIA::OSCQueryClient> OSSIA::OSCQueryClient::create(std::string addr)
{
    return std::make_shared<coppa::ow::OSCQueryClient>(addr);
}

bool coppa::ow::OSCQueryClient::updateChildren(OSSIA::Node& node) const
{
    return false;

}
