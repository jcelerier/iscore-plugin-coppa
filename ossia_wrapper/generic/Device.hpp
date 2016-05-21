#pragma once
#include <Network/Device.h>
#include <Network/Node.h>
#include <future>
#include <iscore/tools/std/Algorithms.hpp>
#include <chrono>
#include <coppa/ossia/parameter_ostream.hpp>
#include <coppa/nano-signal-slot/nano_observer.hpp>

namespace coppa
{
namespace ossia_wrapper // ossia wrapper
{
struct StandardDevice
{
    template<typename Device_T>
    static bool updateNamespace(const Device_T& dev)
    {
      return false;
    }
};

struct UpdatableDevice
{
    template<typename Device_T>
    static bool updateNamespace(Device_T& dev)
    {
      auto& dev_impl = dev.dev();
      // Send a refresh request
      auto fut_refresh = dev_impl.refresh();

      // Wait until we get a full namespace
      switch(fut_refresh.wait_for(std::chrono::seconds(5)))
      {
        case std::future_status::ready:
        {
          make_tree_rec(dev, dev_impl);
        }
          return true;
        default:
          return false;
          break;
      }
    }

  private:
    template<typename Device_T, typename ProtocolImpl_T>
    static void make_tree_rec(Device_T& dev, const ProtocolImpl_T& remote_dev)
    {
      using node_type = typename Device_T::node_type;
      auto dev_ptr = std::dynamic_pointer_cast<Device_T>(dev.shared_from_this());

      // TODO LOCKME
      // Print the real parameters in the tree
      for(const auto& elt : remote_dev.map().get_data_map())
      {
        node_type* node = &dev;
        // Go to the furthest known point in the node hierarchy
        const auto& addr = elt.destination;
        if(addr == "/")
          continue;

        std::vector<std::string> path;
        boost::split(path, addr, boost::lambda::_1 == '/');
        path.erase(path.begin());

        for(auto i = 0U; i < path.size(); i++)
        {
          const auto& children = node->children();
          auto it = find_if(
                      children,
                      [&] (const auto& ossia_node) { return ossia_node->getName() == path[i]; });

          if(it == children.end())
          {
            // We have to start adding sub-nodes from here.
            node_type* parentnode = node;
            for(auto k = i; k < path.size(); k++)
            {
              std::string new_addr;
              for(auto str_idx = 0U; str_idx <= k; str_idx++)
              {
                new_addr += "/" + path[str_idx];
              }

              // Creation of the node :
              auto n = std::make_shared<node_type>(
                            parentnode->shared_from_this(),
                            dev_ptr,
                            new_addr);

              n->setDevice(dev_ptr);

              // Creation of an address if it is an existing keyin the map.
              auto param_it = remote_dev.map().find(new_addr);
              if(param_it != remote_dev.map().end())
              {
                n->createAddress(ToOssiaType(*param_it));
              }

              // Insertion of the node
              parentnode->get_children().insert(parentnode->get_children().end(), n);

              // Loop
              if(k == path.size() - 1)
              {
                node = n.get();
              }
              else
              {
                parentnode = n.get();
              }
            }

            break;
          }
          else
          {
            node = dynamic_cast<node_type*>(it->get());
          }
        }
      }
    }
};

template<
    typename Protocol_T,
    typename Node_T,
    typename DeviceImpl_T,
    typename ConcreteDevice_T>
class GenericDeviceBase :
    public OSSIA::Device,
    public Node_T,
    public Nano::Observer
{
    std::shared_ptr<Protocol_T> m_proto;
    std::map<std::string, std::weak_ptr<OSSIA::Node>> m_children;

  public:
    using this_t = GenericDeviceBase<Protocol_T, Node_T, DeviceImpl_T, ConcreteDevice_T>;
    using node_type = Node_T;
    using protocol_type = typename Protocol_T::protocol_t;
    using ossia_protocol_t = Protocol_T;
    using concrete_device_t = ConcreteDevice_T;
    GenericDeviceBase(std::shared_ptr<Protocol_T> prot):
      m_proto{prot}
    {
    }

    virtual ~GenericDeviceBase()
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
      return DeviceImpl_T::updateNamespace(*dynamic_cast<concrete_device_t*>(this));
    }
};

}
}
