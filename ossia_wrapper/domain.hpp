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
      assert(false);
      return *this;
    }

    const OSSIA::Value* getMax() const override
    {
      return coppaToOSSIAValue(m_domain.max);
    }

    OSSIA::Domain& setMax(const OSSIA::Value*) override
    {
      assert(false);
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
}
}
