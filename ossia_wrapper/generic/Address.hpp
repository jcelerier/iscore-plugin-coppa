#pragma once

#include <Network/Address.h>
#include <coppa/ossia/parameter.hpp>
#include <ossia_wrapper/ossia_wrapper.hpp>
#include <ossia_wrapper/domain.hpp>
#include <future>
namespace coppa
{
namespace ossia_wrapper // ossia wrapper
{

inline OSSIA::AccessMode toOSSIAAccess(coppa::ossia::Access::Mode acc)
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

inline OSSIA::BoundingMode toOSSIABounding(coppa::ossia::Bounding::Mode bound)
{
  return static_cast<OSSIA::BoundingMode>(bound);
}

inline coppa::ossia::Bounding::Mode toCoppaBounding(OSSIA::BoundingMode bound)
{
  return static_cast<coppa::ossia::Bounding::Mode>(bound);
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
        addr.dev().set(addr.destination(), coppa::ossia::Value{fromValue(v)});
      }

      return addr;
    }

    template<typename Address_T>
    static OSSIA::Address& pushValue(Address_T& addr, const OSSIA::Value* v)
    {
      if(v)
      {
        addr.dev().push(addr.destination(), coppa::ossia::Value{fromValue(v)});
      }

      return addr;
    }

    template<typename Address_T>
    static OSSIA::Value::Type getValueType(Address_T& addr)
    {
      auto it = addr.dev().find(addr.destination());
      if(it != addr.dev().map().end())
        return coppaToOSSIAValueType(it->value);
      return OSSIA::Value::Type::IMPULSE;
    }

    template<typename Address_T>
    static OSSIA::AccessMode getAccessMode(Address_T& addr)
    {
      return toOSSIAAccess(addr.dev().map().get(addr.destination()).access);
    }

    template<typename Address_T>
    static OSSIA::Address& setAccessMode(Address_T& addr, OSSIA::AccessMode am)
    {
      addr.dev().set_access(addr.destination(), toCoppaAccess(am));
      return addr;
    }

    template<typename Address_T>
    static const std::shared_ptr<OSSIA::Domain>& getDomain(Address_T& addr)
    {
      if(!addr.domain)
      {
        coppa::ossia::Parameter p = addr.dev().map().get(addr.destination());
        addr.domain = std::make_shared<ossia_wrapper::Domain>(p);
      }

      return addr.domain;
    }

    template<typename Address_T>
    static OSSIA::Address& setDomain(Address_T& addr, const std::shared_ptr<OSSIA::Domain>& dom)
    {
      addr.dev().set_range(addr.destination(), fromOSSIADomain(dom.get()));
      return addr;
    }

    template<typename Address_T>
    static OSSIA::BoundingMode getBoundingMode(Address_T& addr)
    {
      return toOSSIABounding(addr.parameter().bounding);
    }

    template<typename Address_T>
    static OSSIA::Address& setBoundingMode(Address_T& addr, OSSIA::BoundingMode bm)
    {
      addr.dev().set_bounding(addr.destination(), toCoppaBounding(bm));
      return addr;
    }

    template<typename Address_T>
    static bool getRepetitionFilter(Address_T& addr)
    {
      return addr.parameter().repetitionFilter;
    }

    template<typename Address_T>
    static OSSIA::Address& setRepetitionFilter(Address_T& addr, bool b)
    {
      addr.dev().set_repetition(addr.destination(), b);
      return addr;
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

    template<typename Address_T, typename Fun>
    static const auto addCallback(Address_T& addr, Fun callback)
    {
      return addr.Address_T::callback_container_t::addCallback(callback);
    }

    template<typename Address_T, typename Iterator>
    static const void removeCallback(Address_T& addr, Iterator it)
    {
      addr.Address_T::callback_container_t::removeCallback(it);
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

    template<typename Address_T, typename Fun>
    static const auto addCallback(Address_T& addr, Fun callback)
    {
      auto it = addr.Address_T::callback_container_t::addCallback(callback);
      if(addr.callbacks().size() == 1)
      {
        addr.dev()
            .get_value_callback(addr.destination())
            .template connect<Address_T, &Address_T::value_callback>(addr);
        addr.dev().listen(addr.destination(), true);
      }
      return it;
    }

    template<typename Address_T, typename Iterator>
    static const void removeCallback(Address_T& addr, Iterator it)
    {
      addr.Address_T::callback_container_t::removeCallback(it);
      if(addr.callbacks().size() == 0)
      {
        addr.dev().listen(addr.destination(), false);
        addr.dev()
            .get_value_callback(addr.destination())
            .template disconnect<Address_T, &Address_T::value_callback>(addr);
        // TODO remove when no more callbacks
      }
    }
};

template<typename Node_T, typename AddressImpl>
class Address : public OSSIA::Address
{
  public:
    using callback_container_t = OSSIA::CallbackContainer<OSSIA::ValueCallback>;
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

    const coppa::ossia::Parameter& parameter() const
    { return *dev().find(this->destination()); }

    void value_callback(coppa::ossia::Value val)
    {
      auto ptr = coppaToOSSIAValue(val);
      for(auto cb : callbacks())
      {
        cb(ptr.get());
      }
    }

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

    Address::iterator addCallback(OSSIA::ValueCallback fun) override
    {
      return AddressImpl::addCallback(*this, fun);
    }

    void removeCallback(Address::iterator it) override
    {
      AddressImpl::removeCallback(*this, it);
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
