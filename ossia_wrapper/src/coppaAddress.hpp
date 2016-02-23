#pragma once

#include <Network/Address.h>
#include <Editor/Value.h>
#include <Editor/Domain.h>

#include "coppaNode.hpp"
#include "coppaDevice.hpp"
#include <coppa/oscquery/device/remote.hpp>
#include <coppa/oscquery/parameter.hpp>

template<typename Node_T>
auto parameter(const std::shared_ptr<Node_T>& node)
{
    return static_cast<typename Node_T::device_type*>(node->getDevice().get())->dev().map().get(node->destination());
}


namespace coppa
{
namespace oscquery
{
class remote_device;
}
namespace ow // ossia wrapper
{

inline OSSIA::Value* coppaToOSSIAValue(coppa::oscquery::Variant val)
{
    using namespace eggs::variants;
    switch(val.which())
    {
        case 0: // int
            return new OSSIA::Int(get<int32_t>(val));
            break;
        case 1: // float
            return new OSSIA::Float(get<float>(val));
            break;
        case 2: // bool
            return new OSSIA::Bool(get<bool>(val));
            break;
        case 3: // string
            return new OSSIA::String(get<std::string>(val));
            break;
        case 4: // generic
            return nullptr;
            break;
        default:
            return nullptr;
            break;
    }
}

class coppaDomain : public OSSIA::Domain
{
        coppa::oscquery::Range m_domain;

    public:
        coppaDomain(coppa::oscquery::Range d):
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

template<typename Node_T> // coppa::ow::Node
class Address : public OSSIA::Address
{
        std::shared_ptr<Node_T> m_parent;

        mutable std::shared_ptr<OSSIA::Domain> m_domain;

        coppa::oscquery::remote_device& dev() const
        {
            return static_cast<typename Node_T::device_type*>(m_parent->getDevice().get())->dev();
        }
    public:
        Address(std::shared_ptr<Node_T> parent):
            m_parent{parent}
        {

        }

        virtual ~Address()
        {

        }

        const std::shared_ptr<OSSIA::Node> getNode() const override
        {
            return m_parent;
        }

        const OSSIA::Value* pullValue() override
        {
            return getValue();
        }

        OSSIA::Address& pushValue(const OSSIA::Value* v) override
        {
            return setValue(v);
        }

        const OSSIA::Value* getValue() const override
        {
            coppa::oscquery::Parameter p = dev().map().get(m_parent->destination());

            if(p.values.size() == 0)
            {
                return new OSSIA::Impulse;
            }

            if(p.values.size() == 1)
            {
                return coppaToOSSIAValue(p.values[0]);
            }

            auto tuple = new OSSIA::Tuple;
            for(const auto& val : p.values)
            {
                tuple->value.push_back(coppaToOSSIAValue(val));
            }

            return tuple;
        }

        const OSSIA::Value* cloneValue(std::vector<char> = {}) const override
        {
            return nullptr;
        }

        Address & setValueType(OSSIA::Value::Type) override
        {
            return *this;
        }

        OSSIA::Address& setValue(const OSSIA::Value* v) override
        {
            if(!v)
                return *this;

            coppa::oscquery::Parameter p = dev().map().get(m_parent->destination());
            p.values.clear();
            switch(v->getType())
            {
                case OSSIA::Value::Type::INT:
                    p.values.push_back(static_cast<const OSSIA::Int*>(v)->value);
                    break;
                case OSSIA::Value::Type::FLOAT:
                    p.values.push_back(static_cast<const OSSIA::Float*>(v)->value);
                    break;
                case OSSIA::Value::Type::BOOL:
                    p.values.push_back(static_cast<const OSSIA::Bool*>(v)->value);
                    break;
                case OSSIA::Value::Type::STRING:
                    p.values.push_back(static_cast<const OSSIA::String*>(v)->value);
                    break;
                default:
                    break;
            }

            dev().setter().set(p.destination, p);

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
            coppa::oscquery::Parameter p = dev().map().get(m_parent->destination());
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
            coppa::oscquery::Parameter p = dev().map().get(m_parent->destination());

            if(p.ranges.size() > 0)
            {
                auto dom = std::make_shared<coppaDomain>(p.ranges[0]);
                m_domain = dom;
            }
            else
            {
                auto dom = std::make_shared<coppaDomain>(coppa::oscquery::Range{});
                m_domain = dom;
            }
            return m_domain;
        }

        OSSIA::Address& setDomain(std::shared_ptr<OSSIA::Domain>) override
        {
            return *this;
        }

        OSSIA::BoundingMode getBoundingMode() const override
        {
            return OSSIA::BoundingMode::FREE;
        }

        OSSIA::Address& setBoundingMode(OSSIA::BoundingMode) override
        {
            return *this;
        }

        bool getRepetitionFilter() const override
        {
            return false;
        }

        OSSIA::Address& setRepetitionFilter(bool) override
        {
            return *this;
        }
};


}
}
