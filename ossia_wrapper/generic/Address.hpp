#pragma once

#include <Network/Address.h>
#include <coppa/minuit/parameter.hpp>
#include <ossia_wrapper/ossia_wrapper.hpp>
#include <ossia_wrapper/domain.hpp>
#include <future>
namespace coppa
{
namespace minuit_wrapper // ossia wrapper
{

inline OSSIA::AccessMode toOSSIAAccess(coppa::minuit::Access::Mode acc)
{
  switch(acc)
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
    default:
      throw;
  }
}

inline coppa::Access::Mode toCoppaAccess(OSSIA::AccessMode acc)
{
  switch(acc)
  {
    case OSSIA::AccessMode::GET:
      return coppa::Access::Mode::Get;
      break;
    case OSSIA::AccessMode::SET:
      return coppa::Access::Mode::Set;
      break;
    case OSSIA::AccessMode::BI:
      return coppa::Access::Mode::Both;
      break;
    default:
      throw;
  }
}

inline OSSIA::BoundingMode toOSSIABounding(coppa::minuit::Bounding::Mode bound)
{
  return static_cast<OSSIA::BoundingMode>(bound);
}

inline coppa::minuit::Bounding::Mode toCoppaBounding(OSSIA::BoundingMode bound)
{
  return static_cast<coppa::minuit::Bounding::Mode>(bound);
}


struct GenericAddress
{
    template<typename Address_T>
    static OSSIA::Value* getValue(Address_T& addr)
    {
      auto it = addr.dev().find(addr.destination());
      if(it != addr.dev().map().end())
        return coppaToOSSIAValue(*it).release();
      return nullptr;
    }

    template<typename Address_T>
    static OSSIA::Value* cloneValue(Address_T& addr, std::vector<char> = {})
    {
      return getValue(addr);
    }

    template<typename Address_T>
    static Address_T & setValueType(Address_T& addr, OSSIA::Value::Type)
    {
      assert(false);
      return addr;
    }


    template<typename Address_T>
    static OSSIA::Address& setValue(Address_T& addr, const OSSIA::Value* v)
    {
      if(v)
      {
        addr.dev().set(addr.destination(), fromTopValue(v));
      }

      return addr;
    }

    template<typename Address_T>
    static OSSIA::Address& pushValue(Address_T& addr, const OSSIA::Value* v)
    {
      if(v)
      {
        addr.dev().push(addr.destination(), fromTopValue(v));
      }

      return addr;
    }

    template<typename Address_T>
    static OSSIA::Value::Type getValueType(Address_T& addr)
    {
      auto it = addr.dev().find(addr.destination());
      if(it != addr.dev().map().end())
        return coppaToOSSIAValueType(*it);
      return OSSIA::Value::Type::IMPULSE;
    }

    template<typename Address_T>
    static OSSIA::AccessMode getAccessMode(Address_T& addr)
    {
      return toOSSIAAccess(addr.dev().map().get(addr.destination()).access);
    }

    template<typename Address_T>
    static OSSIA::Address& setAccessMode(Address_T& addr, OSSIA::AccessMode)
    {
      assert(false);
      return addr;
    }

    template<typename Address_T>
    static const std::shared_ptr<OSSIA::Domain>& getDomain(Address_T& addr)
    {
      if(!addr.domain)
      {
        coppa::minuit::Parameter p = addr.dev().map().get(addr.destination());
        addr.domain = std::make_shared<minuit_wrapper::Domain>(p);
      }

      return addr.domain;
    }

    template<typename Address_T>
    static OSSIA::Address& setDomain(Address_T& addr, std::shared_ptr<OSSIA::Domain>)
    {
      assert(false);
      return addr;
    }

    template<typename Address_T>
    static OSSIA::BoundingMode getBoundingMode(Address_T& addr)
    {
      return toOSSIABounding(addr.parameter().bounding);
    }

    template<typename Address_T>
    static OSSIA::Address& setBoundingMode(Address_T& addr, OSSIA::BoundingMode)
    {
      assert(false);
      return addr;
    }

    template<typename Address_T>
    static bool getRepetitionFilter(Address_T& addr)
    {
      return addr.parameter().repetitionFilter;
    }

    template<typename Address_T>
    static OSSIA::Address& setRepetitionFilter(Address_T& addr, bool)
    {
      assert(false);
      return addr;
    }
};

/**
 * @brief The PullableAddress struct
 *
 * For Minuit-like protocols able to pull a value
 */
struct PullableAddress : public GenericAddress
{
    template<typename Address_T>
    static const OSSIA::Value* pullValue(Address_T& addr)
    {
      auto res = addr.dev().pull(addr.destination());
      switch(res.wait_for(std::chrono::milliseconds(50)))
      {
        case std::future_status::ready:
          return coppaToOSSIAValue(res.get()).release();
        default:
          return nullptr;
      }
    }
};

/**
 * @brief The PullableAddress struct
 *
 * For protocols unable to pull a value
 */
struct StandardAddress : public GenericAddress
{
    template<typename Address_T>
    static const OSSIA::Value* pullValue(Address_T& addr)
    {
      return getValue(addr);
    }
};

template<typename Node_T, typename AddressImpl>
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

    mutable std::shared_ptr<OSSIA::Domain> domain;

    const auto& destination() const
    { return m_parent.lock()->destination(); }

    auto& dev() const
    {
      return static_cast<typename Node_T::device_type*>(m_parent.lock()->getDevice().get())->dev();
    }

    const coppa::minuit::Parameter& parameter() const
    { return *dev().find(this->destination()); }

  private:
    using device_type = typename Node_T::device_type;
    std::weak_ptr<Node_T> m_parent;
    mutable std::unique_ptr<const OSSIA::Value> m_value;

    const std::shared_ptr<OSSIA::Node> getNode() const override
    {
      return m_parent.lock();
    }

    const OSSIA::Value* pullValue() override
    {
      m_value.reset(AddressImpl::pullValue(*this));
      return m_value.get();
    }

    OSSIA::Address& pushValue(const OSSIA::Value* v) override
    {
      return AddressImpl::pushValue(*this, v);
    }

    const OSSIA::Value* getValue() const override
    {
      m_value.reset(AddressImpl::getValue(*this));
      return m_value.get();
    }

    const OSSIA::Value* cloneValue(std::vector<char> = {}) const override
    {
      return AddressImpl::cloneValue(*this);
    }

    Address & setValueType(OSSIA::Value::Type t) override
    {
      return AddressImpl::setValueType(*this, t);
    }

    OSSIA::Address& setValue(const OSSIA::Value* v) override
    {
      return AddressImpl::setValue(*this, v);
    }

    OSSIA::Value::Type getValueType() const override
    {
      return AddressImpl::getValueType(*this);
    }

    OSSIA::AccessMode getAccessMode() const override
    {
      return AddressImpl::getAccessMode(*this);
    }

    OSSIA::Address& setAccessMode(OSSIA::AccessMode am) override
    {
      return AddressImpl::setAccessMode(*this, am);
    }

    const std::shared_ptr<OSSIA::Domain>& getDomain() const override
    {
      return AddressImpl::getDomain(*this);
    }

    OSSIA::Address& setDomain(std::shared_ptr<OSSIA::Domain> dom) override
    {
      return AddressImpl::setDomain(*this, dom);
    }

    OSSIA::BoundingMode getBoundingMode() const override
    {
      return AddressImpl::getBoundingMode(*this);
    }

    OSSIA::Address& setBoundingMode(OSSIA::BoundingMode bm) override
    {
      return AddressImpl::setBoundingMode(*this, bm);
    }

    bool getRepetitionFilter() const override
    {
      return AddressImpl::getRepetitionFilter(*this);
    }

    OSSIA::Address& setRepetitionFilter(bool b) override
    {
      return AddressImpl::setRepetitionFilter(*this, b);
    }
};
}
}
