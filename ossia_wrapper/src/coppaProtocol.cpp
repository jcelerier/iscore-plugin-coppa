#include "coppaProtocol.hpp"

std::shared_ptr<OSSIA::OSCQueryClient> OSSIA::OSCQueryClient::create(std::string addr)
{
    return std::make_shared<coppa::minuit_wrapper::OSCQuery::OSCQueryClient>(addr);
}

coppa::minuit_wrapper::OSCQuery::OSCQueryClient::~OSCQueryClient()
{

}
