#include "coppaDevice.hpp"
#include "coppaProtocol.hpp"
#include "coppaNode.hpp"
#include "coppaAddress.hpp"


std::shared_ptr<OSSIA::Device> OSSIA::Device::create(
        std::shared_ptr<OSSIA::Protocol> p,
        std::string)
{
    if(auto clt = std::dynamic_pointer_cast<coppa::ow::OSCQueryClient>(p))
    {
        return std::make_shared<coppa::ow::Device<coppa::ow::OSCQueryClient, coppa::oscquery::remote_device>>(clt);
    }
    return {};
}


