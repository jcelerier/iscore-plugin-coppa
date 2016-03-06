#include "Protocol.hpp"

std::shared_ptr<OSSIA::Local> OSSIA::Local::create()
{
  return std::make_shared<coppa::minuit_wrapper::Local::Client>();
}

coppa::minuit_wrapper::Local::Client::~Client()
{

}
