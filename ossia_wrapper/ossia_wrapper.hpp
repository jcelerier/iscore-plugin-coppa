#pragma once
#include <coppa/minuit/parameter.hpp>
#include <Editor/Value.h>

namespace coppa
{
namespace minuit_wrapper
{

template<typename Node_T>
auto parameter(const std::shared_ptr<Node_T>& node)
{
  return static_cast<typename Node_T::device_type*>(node->getDevice().get())->dev().map().get(node->destination());
}

inline OSSIA::Value::Type ToOssiaType(const coppa::minuit::Values& v, bool& valid)
{
  valid = false;
  if(v.variants.size() > 1)
  {
    valid = true;
    return OSSIA::Value::Type::TUPLE;
  }
  else if(v.variants.size() == 1)
  {
    switch(coppa::minuit::which(v.variants[0]))
    {
      case coppa::minuit::Type::none_t:
        valid = false;
        return {};
      case coppa::minuit::Type::impulse_t:
        valid = true;
        return OSSIA::Value::Type::IMPULSE;
      case coppa::minuit::Type::bool_t:
        valid = true;
        return OSSIA::Value::Type::BOOL;
      case coppa::minuit::Type::int_t:
        valid = true;
        return OSSIA::Value::Type::INT;
      case coppa::minuit::Type::float_t:
        valid = true;
        return OSSIA::Value::Type::FLOAT;
      case coppa::minuit::Type::char_t:
        valid = true;
        return OSSIA::Value::Type::CHAR;
      case coppa::minuit::Type::string_t:
        valid = true;
        return OSSIA::Value::Type::STRING;
      case coppa::minuit::Type::tuple_t:
        valid = true;
        return OSSIA::Value::Type::TUPLE;
      case coppa::minuit::Type::generic_t:
        valid = true;
        return OSSIA::Value::Type::GENERIC;
    }
  }
  else
  {
    // no type
    valid = false;
    return {};
  }
}

inline OSSIA::Value::Type coppaToOSSIAValueType(const coppa::minuit::Variant& val)
{
  if(!val)
    return OSSIA::Value::Type::IMPULSE;

  using namespace oscpack;
  using namespace eggs::variants;
  const struct vis {
      using return_type = OSSIA::Value::Type;
      return_type operator()(const coppa::minuit::None& ) const {
        return OSSIA::Value::Type::IMPULSE;
      }

      return_type operator()(const coppa::minuit::Impulse&) const {
        return OSSIA::Value::Type::IMPULSE;
      }

      return_type operator()(int32_t) const {
        return OSSIA::Value::Type::INT;
      }

      return_type operator()(float) const {
        return OSSIA::Value::Type::FLOAT;
      }

      return_type operator()(bool) const {
        return OSSIA::Value::Type::BOOL;
      }

      return_type operator()(char) const {
        return OSSIA::Value::Type::CHAR;
      }

      return_type operator()(const std::string&) const {
        return OSSIA::Value::Type::STRING;
      }

      return_type operator()(const coppa::minuit::Tuple&) const {
        return OSSIA::Value::Type::TUPLE;
      }

      return_type operator()(const coppa::minuit::Generic&) const {
        return OSSIA::Value::Type::GENERIC;
      }

  } visitor{};

  return eggs::variants::apply(visitor, val);
}

inline OSSIA::Value::Type coppaToOSSIAValueType(const coppa::minuit::Values& val)
{
  if(val.variants.size() == 0)
  {
    return OSSIA::Value::Type::IMPULSE;
  }
  else if(val.variants.size() == 1)
  {
    return coppaToOSSIAValueType(val.variants[0]);
  }
  else
  {
    return OSSIA::Value::Type::TUPLE;
  }
}


inline coppa::minuit::Values OSSIAValueTypeToCoppa(OSSIA::Value::Type t)
{
  coppa::minuit::Values v;
  switch (t) {
    case OSSIA::Value::Type::IMPULSE:
      v.variants.push_back(coppa::minuit::Impulse{});
      break;
    case OSSIA::Value::Type::INT:
      v.variants.push_back(int32_t{});
      break;
    case OSSIA::Value::Type::FLOAT:
      v.variants.push_back(float{});
      break;
    case OSSIA::Value::Type::BOOL:
      v.variants.push_back(bool{});
      break;
    case OSSIA::Value::Type::CHAR:
      v.variants.push_back(char{});
      break;
    case OSSIA::Value::Type::STRING:
      v.variants.push_back(std::string{});
      break;
    case OSSIA::Value::Type::TUPLE:
      // TODO ??
      break;
    case OSSIA::Value::Type::GENERIC:
      // TODO ??
      break;
    default:
      break;
  }

  return v;
}

inline std::unique_ptr<OSSIA::Value> coppaToOSSIAValue(const coppa::minuit::Variant& val)
{
  if(!val)
    return nullptr;

  using namespace oscpack;
  using namespace eggs::variants;
  const struct vis {
      using return_type = OSSIA::Value*;
      return_type operator()(const coppa::minuit::None& ) const {
        return new OSSIA::Impulse;
      }

      return_type operator()(const coppa::minuit::Impulse& val) const {
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

      return_type operator()(const coppa::minuit::Tuple& t) const {
        // We don't really know how to do this ?
        return nullptr;
      }

      return_type operator()(const coppa::minuit::Generic& val) const {
        return nullptr;
      }

  } visitor{};

  return std::unique_ptr<OSSIA::Value>(eggs::variants::apply(visitor, val));
}

[[deprecated]] inline std::unique_ptr<OSSIA::Value> coppaToOSSIAValue(const coppa::minuit::Values& val) // leaks
{
  if(val.variants.size() == 0)
  {
    return std::make_unique<OSSIA::Impulse>();
  }
  else if(val.variants.size() == 1)
  {
    return coppaToOSSIAValue(val.variants[0]);
  }
  else
  {
    auto tuple = new OSSIA::Tuple;
    tuple->value.reserve(val.variants.size());
    for(const auto& v : val.variants)
    {
      tuple->value.push_back(coppaToOSSIAValue(v).release());
    }
    return std::unique_ptr<OSSIA::Value>(tuple);
  }
}

inline coppa::minuit::Variant fromValue(const OSSIA::Value* v)
{
  switch(v->getType())
  {
    case OSSIA::Value::Type::IMPULSE:
      return coppa::minuit::Impulse{};
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
      coppa::minuit::Tuple t;
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


inline coppa::minuit::Values fromTopValue(const OSSIA::Value* v)
{
  coppa::minuit::Values coppa_v;
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
