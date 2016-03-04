#pragma once
#include <Network/Device.h>
#include <Network/Protocol.h>
#include <Editor/Value.h>
#include <Editor/Domain.h>
#include <coppa/coppa.hpp>
#include <coppa/minuit/device/minuit_remote_future.hpp>
#include <Misc/Container.h>

#include <boost/algorithm/string/split.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/range/algorithm/find_if.hpp>
#include <shared_mutex>
#include "Node.hpp"
namespace coppa
{

namespace ossia_wrapper
{
namespace Local
{
class Protocol;

template<
    typename Protocol_T>
class Device :
    public OSSIA::Device,
    public Node<Device<Protocol_T>>
{
    std::shared_ptr<Protocol_T> m_proto;

  public:
    using node_type = coppa::ossia_wrapper::Local::Node<Device<Protocol_T>>;
    using protocol_type = typename Protocol_T::protocol_t;
    using ossia_protocol_t = Protocol_T;

    Device(std::shared_ptr<Protocol_T> prot):
      m_proto{prot}
    {
    }

    virtual ~Device()
    {

    }

    protocol_type& dev() const
    {
      return m_proto->dev();
    }

    std::string getName() const override
    {
      return dev().get_name();
    }

    OSSIA::Node& setName(std::string s) override
    {
      dev().set_name(s);
      return *this;
    }

    std::shared_ptr<OSSIA::Protocol> getProtocol() const override
    {
      return m_proto;
    }

    bool updateNamespace() override
    {
      return false;
    }
};
}
}
}
