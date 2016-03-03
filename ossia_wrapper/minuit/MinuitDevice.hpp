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
#include "MinuitNode.hpp"
namespace coppa
{
namespace ossia_wrapper
{
namespace Minuit
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
    using node_type = coppa::ossia_wrapper::Minuit::Node<Device<Protocol_T>>;
    using protocol_type = typename Protocol_T::protocol_t;

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
      // Send a refresh request
      auto fut_refresh = dev().refresh();

      // Wait until we get a full namespace
      switch(fut_refresh.wait_for(std::chrono::seconds(5)))
      {
        case std::future_status::ready:
        {
          for(auto elt : dev().map())
          {
            std::cerr << elt.destination << "\n";
          }
          make_tree_rec(m_proto->dev());
        }
          return true;
        default:
          return false;
          break;
      }
    }

  private:
    void make_tree_rec(const protocol_type& remote_dev)
    {
      auto dev_ptr = std::dynamic_pointer_cast<Device>(this->shared_from_this());

      // TODO LOCKME
      // Print the real parameters in the tree
      for(const auto& elt : remote_dev.map().get_data_map())
      {
        node_type* node = this;
        // Go to the furthest known point in the node hierarchy
        const auto& addr = elt.destination;
        if(addr == "/")
          continue;

        std::vector<std::string> path;
        boost::split(path, addr, boost::lambda::_1 == '/');
        path.erase(path.begin());

        for(int i = 0; i < path.size(); i++)
        {
          const auto& children = node->children();
          auto it = boost::range::find_if(
                      children,
                      [&] (const auto& ossia_node) { return ossia_node->getName() == path[i]; });

          if(it == children.end())
          {
            // We have to start adding sub-nodes from here.
            node_type* parentnode = node;
            for(int k = i; k < path.size(); k++)
            {
              std::string new_addr;
              for(int str_idx = 0; str_idx <= k; str_idx++)
              {
                new_addr += "/" + path[str_idx];
              }
              auto new_node_it = parentnode->emplace(parentnode->children().end(), new_addr);
              auto n = dynamic_cast<node_type*>(new_node_it->get());
              n->setDevice(dev_ptr);
              if(remote_dev.map().has(new_addr))
                n->createAddress({});

              if(k == path.size() - 1)
              {
                node = n;
              }
              else
              {
                parentnode = n;
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
}
}
}
