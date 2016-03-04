#pragma once

#include <Network/Address.h>
#include <Editor/Value.h>
#include <Editor/Domain.h>

#include <coppa/minuit/parameter.hpp>
#include <ossia_wrapper/ossia_wrapper.hpp>
#include <ossia_wrapper/domain.hpp>
#include <future>

namespace coppa
{
namespace ossia_wrapper // ossia wrapper
{
namespace Minuit
{
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
      switch(res.wait_for(std::chrono::milliseconds(50)))
      {
        case std::future_status::ready:
          return coppaToOSSIAValue(res.get());
        default:
          return nullptr;
      }
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
      if(!m_domain)
      {
        coppa::ossia::Parameter p = dev().map().get(m_parent->destination());
        m_domain = std::make_shared<Domain>(p);
      }

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
}
