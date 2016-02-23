#include "coppaProtocol.hpp"

std::shared_ptr<OSSIA::OSCQueryClient> OSSIA::OSCQueryClient::create(std::string addr)
{
    return std::make_shared<coppa::ow::OSCQueryClient>(addr);
}

coppa::ow::OSCQueryClient::~OSCQueryClient()
{

}
