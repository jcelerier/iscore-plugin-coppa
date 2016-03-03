#include <Network/Node.h>
#include <Network/Device.h>
#include <Network/Protocol.h>
#include <Network/Protocol/Local.h>
#include <Network/Protocol/Minuit.h>
#include <Network/Protocol/MIDI.h>
#include <Network/Protocol/OSC.h>
#include "src/coppaAddress.hpp"
#include "src/coppaNode.hpp"
#include "src/coppaProtocol.hpp"
#include "src/coppaDevice.hpp"
#include "minuit/MinuitAddress.hpp"
#include "minuit/MinuitNode.hpp"
#include "minuit/MinuitProtocol.hpp"
#include "minuit/MinuitDevice.hpp"
#include "osc/Address.hpp"
#include "osc/Node.hpp"
#include "osc/Protocol.hpp"
#include "osc/Device.hpp"
#include "local/Address.hpp"
#include "local/Node.hpp"
#include "local/Protocol.hpp"
#include "local/Device.hpp"

namespace OSSIA
{

std::shared_ptr<MIDI> MIDI::create()
{
  return {};
}

Address::~Address()
{

}

Local::~Local()
{

}
Minuit::~Minuit()
{

}
OSC::~OSC()
{

}

Protocol::~Protocol()
{

}

Node::~Node()
{

}

Device::~Device()
{

}

void CleanupProtocols()
{

}


std::shared_ptr<OSSIA::Device> OSSIA::Device::create(
    std::shared_ptr<OSSIA::Protocol> p,
    std::string)
{
  using namespace coppa;
  using namespace ossia_wrapper;
  if(auto clt = std::dynamic_pointer_cast<ossia_wrapper::OSCQuery::OSCQueryClient>(p))
  {
    auto dev = std::make_shared<ossia_wrapper::OSCQuery::Device<ossia_wrapper::OSCQuery::OSCQueryClient>>(clt);
    dev->setDevice(dev);
    return dev;
  }
  else if(auto clt = std::dynamic_pointer_cast<ossia_wrapper::Local::Client>(p))
  {
    auto dev = std::make_shared<ossia_wrapper::Local::Device<ossia_wrapper::Local::Client>>(clt);
    dev->setDevice(dev);
    return dev;
  }
  else if(auto clt = std::dynamic_pointer_cast<ossia_wrapper::OSC::Client>(p))
  {
    auto dev = std::make_shared<ossia_wrapper::OSC::Device<ossia_wrapper::OSC::Client>>(clt);
    dev->setDevice(dev);
    return dev;
  }
  else if(auto clt = std::dynamic_pointer_cast<ossia_wrapper::Minuit::MinuitClient>(p))
  {
    auto dev = std::make_shared<ossia_wrapper::Minuit::Device<ossia_wrapper::Minuit::MinuitClient>>(clt);
    dev->setDevice(dev);
    return dev;
  }
  return {};
}

}
