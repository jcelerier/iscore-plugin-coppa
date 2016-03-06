#include "coppaAddress.hpp"
#include "coppaNode.hpp"
#include "coppaProtocol.hpp"
#include "coppaDevice.hpp"

using namespace coppa::minuit_wrapper::OSCQuery;
namespace coppa
{
namespace minuit_wrapper
{
coppaDomain::~coppaDomain()
{

}

void __coppa__address__instantiate()
{
    using type =
    Address<
        Node<
            Device<
                OSCQueryClient
            >
        >
    >;
    type* addr{};
    delete addr;
    type& v = *addr;

    v.getNode();
    v.pullValue();
    v.pushValue((OSSIA::Value*)nullptr);
    v.cloneValue();
    v.setValueType({});
    v.setValue((OSSIA::Value*)nullptr);
}


void __coppa__device__instantiate()
{
    using type =
      Device<
        OSCQueryClient
    >
    ;
    type* addr{};
    delete addr;
    type& d = *addr;


    d.getDevice();
    d.getParent();
    d.getName();
    d.setName(std::string{});

    d.getAddress();
    d.createAddress(OSSIA::Value::Type{});

    d.removeAddress();

    d.emplace(
            OSSIA::Container<OSSIA::Node>::const_iterator{},
                std::string{});
    d.insert(
            OSSIA::Container<OSSIA::Node>::const_iterator{},
            std::shared_ptr<OSSIA::Node>{},
                std::string{});

    d.getProtocol();
    d.updateNamespace();
}


void __coppa__node_instantiate()
{
    using type =
      Node<
        Device<
            OSCQueryClient
        >
    >
    ;
    type* addr{};
    type& d = *addr;
    delete addr;
    d.getParent();
    d.getDevice();
    d.getName();
    d.setName("dfgfg");
    d.getAddress();
    d.createAddress({});
    d.removeAddress();

    d.emplace(
            OSSIA::Container<OSSIA::Node>::const_iterator{},
                std::string{});
    d.insert(
            OSSIA::Container<OSSIA::Node>::const_iterator{},
            std::shared_ptr<OSSIA::Node>{},
                std::string{});
    d.emplace(
        OSSIA::Container<OSSIA::Node>::const_iterator{},
        "dsfdfsf",
        OSSIA::Value::Type{},
        OSSIA::AccessMode{},
        {},
        {},
        {});

    d.erase(OSSIA::Container<OSSIA::Node>::const_iterator{});
}
}
}
