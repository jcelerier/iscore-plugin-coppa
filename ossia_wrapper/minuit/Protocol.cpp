#include "Protocol.hpp"

std::shared_ptr<OSSIA::Minuit> OSSIA::Minuit::create(
    std::string ip,
    int remote_in,
    int local_in)
{
  return std::make_shared<coppa::ossia_wrapper::Minuit::MinuitClient>(ip, remote_in, local_in);
}

coppa::ossia_wrapper::Minuit::MinuitClient::~MinuitClient()
{

}
