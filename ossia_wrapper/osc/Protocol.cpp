#include "Protocol.hpp"

std::shared_ptr<OSSIA::OSC> OSSIA::OSC::create(
    std::string ip,
    int remote_in,
    int local_in)
{
  return std::make_shared<coppa::minuit_wrapper::OSC::Client>(ip, remote_in, local_in);
}

coppa::minuit_wrapper::OSC::Client::~Client()
{

}
