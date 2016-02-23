#include "coppaAddress.hpp"
#include "coppaNode.hpp"
#include "coppaProtocol.hpp"
#include "coppaDevice.hpp"
namespace coppa
{
namespace ow
{
coppaDomain::~coppaDomain()
{

}

void __coppa__address__instantiate()
{
    using type =
    Address<
        coppa::ow::Node<
            coppa::ow::Device<
                coppa::ow::OSCQueryClient,
                coppa::oscquery::remote_device
            >
        >
    >;
    type* addr;
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
      coppa::ow::Device<
        coppa::ow::OSCQueryClient,
        coppa::oscquery::remote_device
    >
    ;
    type* addr;
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
      coppa::ow::Node<
        coppa::ow::Device<
            coppa::ow::OSCQueryClient,
            coppa::oscquery::remote_device
        >
    >
    ;
    type* addr;
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
