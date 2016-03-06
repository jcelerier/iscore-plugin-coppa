#pragma once
#include <Editor/Domain.h>
#include <coppa/minuit/parameter.hpp>
#include <ossia_wrapper/ossia_wrapper.hpp>
namespace coppa
{
namespace ossia_wrapper
{
class Domain final : public OSSIA::Domain
{
    coppa::ossia::Range m_domain;

  public:
    Domain(coppa::ossia::Range d):
      m_domain(d)
    {

    }

    Domain(const OSSIA::Domain* other)
    {
      if(!other)
        return;
      m_domain.min = coppa::ossia_wrapper::fromValue(other->getMin());
      m_domain.max = coppa::ossia_wrapper::fromValue(other->getMax());
      // TODO values
    }

    virtual ~Domain();

    std::shared_ptr<OSSIA::Domain> clone() const override
    {
      return std::make_shared<Domain>(m_domain);
    }

    const OSSIA::Value* getMin() const override
    {
      return coppaToOSSIAValue(m_domain.min);
    }

    OSSIA::Domain& setMin(const OSSIA::Value* val) override
    {
      if(val)
        m_domain.min = coppa::ossia_wrapper::fromValue(val);
      return *this;
    }

    const OSSIA::Value* getMax() const override
    {
      return coppaToOSSIAValue(m_domain.max);
    }

    OSSIA::Domain& setMax(const OSSIA::Value* val) override
    {
      if(val)
        m_domain.max = coppa::ossia_wrapper::fromValue(val);
      return *this;
    }

    std::vector<const OSSIA::Value*> getValues() const override
    {
      std::vector<const OSSIA::Value*> range;
      for(const auto& val : m_domain.range_values)
        range.push_back(coppaToOSSIAValue(val));
      return range;
    }

    OSSIA::Domain& setValues(std::vector<const OSSIA::Value*> values) override
    {
      assert(false);
      return *this;;
    }
};

inline coppa::ossia::Range fromOSSIADomain(OSSIA::Domain* dom)
{
  coppa::ossia::Range r;
  if(!dom)
    return r;

  if(auto min = dom->getMin())
    r.min = coppa::ossia_wrapper::fromValue(min);
  if(auto max = dom->getMax())
    r.max = coppa::ossia_wrapper::fromValue(max);

  // TODO values
  return r;
}
}
}
