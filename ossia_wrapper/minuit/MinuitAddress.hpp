#pragma once

#include <Network/Address.h>
#include <Editor/Value.h>
#include <Editor/Domain.h>

#include <coppa/minuit/parameter.hpp>

template<typename Node_T>
auto parameter(const std::shared_ptr<Node_T>& node)
{
  return static_cast<typename Node_T::device_type*>(node->getDevice().get())->dev().map().get(node->destination());
}

namespace coppa
{
namespace ossia
{
class remote_device;
}
namespace minuit_wrapper // ossia wrapper
{
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


class coppaDomain : public OSSIA::Domain
{
    coppa::ossia::Range m_domain;

  public:
    coppaDomain(coppa::ossia::Range d):
      m_domain(d)
    {

    }

    virtual ~coppaDomain();

    std::shared_ptr<OSSIA::Domain> clone() const override
    {
      return std::make_shared<coppaDomain>(m_domain);
    }
    const OSSIA::Value* getMin() const override
    {
      return coppaToOSSIAValue(m_domain.min);
    }
    OSSIA::Domain& setMin(const OSSIA::Value*) override
    {
      return *this;
    }
    const OSSIA::Value* getMax() const override
    {
      return coppaToOSSIAValue(m_domain.max);
    }
    OSSIA::Domain& setMax(const OSSIA::Value*) override
    {
      return *this;
    }
    std::vector<const OSSIA::Value*> getValues() const override
    {
      return {};
    }
    OSSIA::Domain& setValues(std::vector<const OSSIA::Value*> values) override
    {
      return *this;;
    }
};

template<typename Node_T>
class Address : public OSSIA::Address
{
  public:
    Address(std::shared_ptr<Node_T> parent):
      m_parent{parent}
    {

    }

    virtual ~Address()
    {

    }

  private:
    using device_type = typename Node_T::device_type;
    std::shared_ptr<Node_T> m_parent;

    mutable std::shared_ptr<OSSIA::Domain> m_domain;

    auto& dev() const
    {
      return static_cast<typename Node_T::device_type*>(m_parent->getDevice().get())->dev();
    }

    const coppa::ossia::Parameter& parameter() const
    {
      return *dev().find(m_parent->destination());
    }

    const std::shared_ptr<OSSIA::Node> getNode() const override
    {
      return m_parent;
    }

    const OSSIA::Value* pullValue() override
    {
      auto res = dev().pull(m_parent->destination());
      res.wait();
      auto param = res.get();
      return coppaToOSSIAValue(param);
    }

    OSSIA::Address& pushValue(const OSSIA::Value* v) override
    {
      if(v)
      {
        dev().push(m_parent->destination(), fromTopValue(v));
      }

      return *this;
    }

    const OSSIA::Value* getValue() const override
    {
      coppa::ossia::Parameter p = dev().map().get(m_parent->destination());

      return coppaToOSSIAValue(p);
    }

    const OSSIA::Value* cloneValue(std::vector<char> = {}) const override
    {
      return getValue();
    }

    Address & setValueType(OSSIA::Value::Type) override
    {
      return *this;
    }

    coppa::ossia::Variant fromValue(const OSSIA::Value* v)
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
    }


    coppa::ossia::Values fromTopValue(const OSSIA::Value* v)
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

    OSSIA::Address& setValue(const OSSIA::Value* v) override
    {
      if(v)
      {
        dev().template update<std::string>(m_parent->destination(), [=] (auto& p) {
          static_cast<coppa::ossia::Values&>(p) = fromTopValue(v);
        });
      }

      return *this;
    }

    OSSIA::Value::Type getValueType() const override
    {
      auto val = getValue();
      if(val)
        return val->getType();
      else
        throw;
    }

    OSSIA::AccessMode getAccessMode() const override
    {
      coppa::ossia::Parameter p = dev().map().get(m_parent->destination());
      switch(p.access)
      {
        case coppa::Access::Mode::None:
          return OSSIA::AccessMode::BI;
          break;
        case coppa::Access::Mode::Get:
          return OSSIA::AccessMode::GET;
          break;
        case coppa::Access::Mode::Set:
          return OSSIA::AccessMode::SET;
          break;
        case coppa::Access::Mode::Both:
          return OSSIA::AccessMode::BI;
      }

      return {};

    }

    OSSIA::Address& setAccessMode(OSSIA::AccessMode) override
    {
      return *this;
    }

    const std::shared_ptr<OSSIA::Domain>& getDomain() const override
    {
      coppa::ossia::Parameter p = dev().map().get(m_parent->destination());

      auto dom = std::make_shared<coppaDomain>(p);
      m_domain = dom;
      return m_domain;
    }

    OSSIA::Address& setDomain(std::shared_ptr<OSSIA::Domain>) override
    {
      return *this;
    }

    OSSIA::BoundingMode getBoundingMode() const override
    {
      return static_cast<OSSIA::BoundingMode>(parameter().bounding);
    }

    OSSIA::Address& setBoundingMode(OSSIA::BoundingMode) override
    {
      return *this;
    }

    bool getRepetitionFilter() const override
    {
      return parameter().repetitionFilter;
    }

    OSSIA::Address& setRepetitionFilter(bool) override
    {
      return *this;
    }
};


}
}
