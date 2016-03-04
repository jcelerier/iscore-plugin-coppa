#pragma once
#include <Network/Node.h>
#include <coppa/minuit/parameter.hpp>
namespace coppa
{
namespace ossia_wrapper
{

template<typename Node_T>
auto emplace(
    Node_T& node,
    OSSIA::Container<OSSIA::Node>::const_iterator it,
    const std::string& name,
    OSSIA::Value::Type type,
    OSSIA::AccessMode access = {},
    const std::shared_ptr<OSSIA::Domain>& domain = {},
    OSSIA::BoundingMode bounding = {},
    bool repetitionFilter = {})
{
  coppa::ossia::Parameter p;
  p.destination = node.destination() + "/" + name;
  auto intance_dest = p.destination + ".";
  p.access = static_cast<coppa::ossia::Access::Mode>(access);
  p.bounding = static_cast<coppa::ossia::Bounding::Mode>(bounding);
  p.repetitionFilter = repetitionFilter;

  auto& map = node.device().lock()->dev().map();
  bool inserted = false;
  int num = 0;
  while(!inserted)
  {
    auto it = map.find(p.destination);
    if(it == map.end())
    {
      map.insert(p);
      inserted = true;
    }
    else
    {
      p.destination = intance_dest + std::to_string(num);
    }
  }
  auto child = std::make_shared<typename Node_T::node_type>(
                node.shared_from_this(),
                node.device().lock(),
                p.destination);
  auto res_it = node.get_children().insert(it, std::move(child));
  return res_it;
}

}
}
