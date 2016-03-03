#include "coppaProtocol.hpp"

std::shared_ptr<OSSIA::OSCQueryClient> OSSIA::OSCQueryClient::create(std::string addr)
{
    return std::make_shared<coppa::ossia_wrapper::OSCQuery::OSCQueryClient>(addr);
}

coppa::ossia_wrapper::OSCQuery::OSCQueryClient::~OSCQueryClient()
{

}
