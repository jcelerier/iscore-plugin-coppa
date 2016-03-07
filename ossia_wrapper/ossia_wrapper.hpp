#pragma once
#include <coppa/ossia/parameter.hpp>
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

inline OSSIA::Value::Type ToOssiaType(const coppa::ossia::Value& v)
{
    switch(coppa::ossia::which(v.value))
    {
      case coppa::ossia::Type::none_t:
        return OSSIA::Value::Type::IMPULSE;
      case coppa::ossia::Type::impulse_t:
        return OSSIA::Value::Type::IMPULSE;
      case coppa::ossia::Type::bool_t:
        return OSSIA::Value::Type::BOOL;
      case coppa::ossia::Type::int_t:
        return OSSIA::Value::Type::INT;
      case coppa::ossia::Type::float_t:
        return OSSIA::Value::Type::FLOAT;
      case coppa::ossia::Type::char_t:
        return OSSIA::Value::Type::CHAR;
      case coppa::ossia::Type::string_t:
        return OSSIA::Value::Type::STRING;
      case coppa::ossia::Type::tuple_t:
        return OSSIA::Value::Type::TUPLE;
      case coppa::ossia::Type::generic_t:
        return OSSIA::Value::Type::GENERIC;
    }

    return OSSIA::Value::Type::IMPULSE;
}

inline OSSIA::Value::Type coppaToOSSIAValueType(const coppa::ossia::Variant& val)
{
  if(!val)
    return OSSIA::Value::Type::IMPULSE;

  using namespace oscpack;
  using namespace eggs::variants;
  const struct vis {
      using return_type = OSSIA::Value::Type;
      return_type operator()(const coppa::ossia::None& ) const {
        return OSSIA::Value::Type::IMPULSE;
      }

      return_type operator()(const coppa::ossia::Impulse&) const {
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

      return_type operator()(const coppa::ossia::Tuple&) const {
        return OSSIA::Value::Type::TUPLE;
      }

      return_type operator()(const coppa::ossia::Generic&) const {
        return OSSIA::Value::Type::GENERIC;
      }

  } visitor{};

  return eggs::variants::apply(visitor, val);
}

inline coppa::ossia::Value OSSIAValueTypeToCoppa(OSSIA::Value::Type t)
{
  switch (t) {
    case OSSIA::Value::Type::IMPULSE:
      return coppa::ossia::Value{coppa::ossia::Impulse{}};
      break;
    case OSSIA::Value::Type::INT:
      return coppa::ossia::Value{int32_t{}};
      break;
    case OSSIA::Value::Type::FLOAT:
      return coppa::ossia::Value{float{}};
      break;
    case OSSIA::Value::Type::BOOL:
      return coppa::ossia::Value{bool{}};
      break;
    case OSSIA::Value::Type::CHAR:
      return coppa::ossia::Value{char{}};
      break;
    case OSSIA::Value::Type::STRING:
      return coppa::ossia::Value{std::string{}};
      break;
    case OSSIA::Value::Type::TUPLE:
      return coppa::ossia::Value{coppa::ossia::Tuple{}};
      break;
    case OSSIA::Value::Type::GENERIC:
      return coppa::ossia::Value{coppa::ossia::Generic{}};
      break;
    default:
      return coppa::ossia::Value{coppa::ossia::Impulse{}};
      break;
  }
}

inline std::unique_ptr<OSSIA::Value> coppaToOSSIAValue(const coppa::ossia::Variant& val)
{
  if(!val)
    return nullptr;

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
        auto tuple = new OSSIA::Tuple;
        tuple->value.reserve(t.variants.size());
        for(const auto& v : t.variants)
        {
          tuple->value.push_back(coppaToOSSIAValue(v).release());
        }
        return tuple;
      }

      return_type operator()(const coppa::ossia::Generic& val) const {
        return nullptr;
      }

  } visitor{};

  return std::unique_ptr<OSSIA::Value>(eggs::variants::apply(visitor, val));
}

inline std::unique_ptr<OSSIA::Value> coppaToOSSIAValue(const coppa::ossia::Value& val) // leaks
{
  return coppaToOSSIAValue(val.value);
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
    case OSSIA::Value::Type::GENERIC:
    {
      // TODO;
      break;
    }
    default:
      break;
  }

  return coppa::ossia::Impulse{};
}

}
}
