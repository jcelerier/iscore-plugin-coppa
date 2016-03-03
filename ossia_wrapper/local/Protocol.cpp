#include "Protocol.hpp"

std::shared_ptr<OSSIA::Local> OSSIA::Local::create()
{
  return std::make_shared<coppa::ossia_wrapper::Local::Client>();
}

coppa::ossia_wrapper::Local::Client::~Client()
{

}
