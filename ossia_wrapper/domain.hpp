#pragma once
#include <Editor/Domain.h>
#include <coppa/minuit/parameter.hpp>
#include <ossia_wrapper/ossia_wrapper.hpp>
namespace coppa
{
namespace minuit_wrapper
{
class Domain final : public OSSIA::Domain
{
    coppa::minuit::Range m_domain;
    mutable std::unique_ptr<OSSIA::Value> m_minCache;
    mutable std::unique_ptr<OSSIA::Value> m_maxCache;
    mutable std::vector<std::unique_ptr<OSSIA::Value>> m_valuesCache;

  public:
    Domain(coppa::minuit::Range d):
      m_domain(d)
    {

    }

    Domain(const OSSIA::Domain* other)
    {
      if(!other)
        return;
      m_domain.min = coppa::minuit_wrapper::fromValue(other->getMin());
      m_domain.max = coppa::minuit_wrapper::fromValue(other->getMax());
      // TODO values
    }

    virtual ~Domain();

    std::shared_ptr<OSSIA::Domain> clone() const override
    {
      return std::make_shared<Domain>(m_domain);
    }

    const OSSIA::Value* getMin() const override
    {
      m_minCache = coppaToOSSIAValue(m_domain.min);
      return m_minCache.get();
    }

    OSSIA::Domain& setMin(const OSSIA::Value* val) override
    {
      if(val)
        m_domain.min = coppa::minuit_wrapper::fromValue(val);
      return *this;
    }

    const OSSIA::Value* getMax() const override
    {
      m_maxCache = coppaToOSSIAValue(m_domain.max);
      return m_maxCache.get();
    }

    OSSIA::Domain& setMax(const OSSIA::Value* val) override
    {
      if(val)
        m_domain.max = coppa::minuit_wrapper::fromValue(val);
      return *this;
    }

    std::vector<const OSSIA::Value*> getValues() const override
    {
      m_valuesCache.clear();

      std::vector<const OSSIA::Value*> range;
      for(const auto& val : m_domain.range_values)
      {
        m_valuesCache.push_back(coppaToOSSIAValue(val));
        range.push_back(m_valuesCache.back().get());
      }
      return range;
    }

    OSSIA::Domain& setValues(std::vector<const OSSIA::Value*> values) override
    {
      assert(false);
      return *this;;
    }
};

inline coppa::minuit::Range fromOSSIADomain(OSSIA::Domain* dom)
{
  coppa::minuit::Range r;
  if(!dom)
    return r;

  if(auto min = dom->getMin())
    r.min = coppa::minuit_wrapper::fromValue(min);
  if(auto max = dom->getMax())
    r.max = coppa::minuit_wrapper::fromValue(max);

  // TODO values
  return r;
}
}
}
