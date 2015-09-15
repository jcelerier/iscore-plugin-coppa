#include "coppaProtocol.hpp"

coppa::ow::OSCQueryClient::OSCQueryClient(std::string addr):
    OSSIA::OSCQueryClient{},
    m_dev{addr}
{

}

std::shared_ptr<OSSIA::OSCQueryClient> OSSIA::OSCQueryClient::create(std::string addr)
{
    return std::make_shared<coppa::ow::OSCQueryClient>(addr);
}

bool coppa::ow::OSCQueryClient::pullAddressValue(std::shared_ptr<OSSIA::Address>) const
{
    return false;
}

bool coppa::ow::OSCQueryClient::pushAddressValue(std::shared_ptr<OSSIA::Address>) const
{
    return false;
}

bool coppa::ow::OSCQueryClient::observeAddressValue(std::shared_ptr<OSSIA::Address>, bool) const
{
    return false;
}
