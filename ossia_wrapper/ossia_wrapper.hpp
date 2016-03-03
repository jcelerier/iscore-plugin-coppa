#pragma once
#include <coppa/minuit/parameter.hpp>
#include <Editor/Value.h>

namespace coppa
{
namespace ossia_wrapper
{

template<typename Node_T>
auto parameter(const std::shared_ptr<Node_T>& node)
{
  return static_cast<typename Node_T::device_type*>(node->getDevice().get())->dev().map().get(node->destination());
}

inline OSSIA::Value* coppaToOSSIAValue(const coppa::ossia::Variant& val)
{
  using namespace oscpack;
  using namespace eggs::variants;
  const struct vis {
      using return_type = OSSIA::Value*;
      return_type operator()(const coppa::ossia::None& ) const {
        return new OSSIA::Impulse;
      }

      return_type operator()(const coppa::ossia::Impulse& val) const {
        return new OSSIA::Impulse;
      }

      return_type operator()(int32_t val) const {
        return new OSSIA::Int(val);
      }

      return_type operator()(float val) const {
        return new OSSIA::Float(val);
      }

      return_type operator()(bool val) const {
        return new OSSIA::Bool(val);
      }

      return_type operator()(char val) const {
        return new OSSIA::Char(val);
      }

      return_type operator()(const std::string& val) const {
        return new OSSIA::String(val);
      }

      return_type operator()(const coppa::ossia::Tuple& t) const {
        // We don't really know how to do this ?
        return nullptr;
      }

      return_type operator()(const coppa::ossia::Generic& val) const {
        return nullptr;
      }

  } visitor{};

  return eggs::variants::apply(visitor, val);
}

inline OSSIA::Value* coppaToOSSIAValue(const coppa::ossia::Values& val)
{
  if(val.variants.size() == 0)
  {
    return new OSSIA::Impulse;
  }

  if(val.variants.size() == 1)
  {
    return coppaToOSSIAValue(val.variants[0]);
  }

  auto tuple = new OSSIA::Tuple;
  tuple->value.reserve(val.variants.size());
  for(const auto& v : val.variants)
  {
    tuple->value.push_back(coppaToOSSIAValue(v));
  }
  return tuple;
}

inline coppa::ossia::Variant fromValue(const OSSIA::Value* v)
{
  switch(v->getType())
  {
    case OSSIA::Value::Type::IMPULSE:
      return coppa::ossia::Impulse{};
      break;
    case OSSIA::Value::Type::INT:
      return int32_t(static_cast<const OSSIA::Int*>(v)->value);
      break;
    case OSSIA::Value::Type::FLOAT:
      return float(static_cast<const OSSIA::Float*>(v)->value);
      break;
    case OSSIA::Value::Type::BOOL:
      return bool(static_cast<const OSSIA::Bool*>(v)->value);
      break;
    case OSSIA::Value::Type::CHAR:
      return char(static_cast<const OSSIA::Char*>(v)->value);
      break;
    case OSSIA::Value::Type::STRING:
      return static_cast<const OSSIA::String*>(v)->value;
      break;
    case OSSIA::Value::Type::TUPLE:
    {
      coppa::ossia::Tuple t;
      const auto& vec = static_cast<const OSSIA::Tuple*>(v)->value;
      for(auto elt : vec)
      {
        t.variants.push_back(fromValue(elt));
      }
      return t;
    }
    default:
      break;
  }
  return {};
}


inline coppa::ossia::Values fromTopValue(const OSSIA::Value* v)
{
  coppa::ossia::Values coppa_v;
  if(auto tpl = dynamic_cast<const OSSIA::Tuple*>(v))
  {
    for(auto val : tpl->value)
    {
      coppa_v.variants.push_back(fromValue(val));
    }
  }
  else
  {
    coppa_v.variants.push_back(fromValue(v));
  }
  return coppa_v;
}


}
}
