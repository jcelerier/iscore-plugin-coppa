#include "coppaAddress.hpp"
#include "coppaNode.hpp"
#include "coppaDevice.hpp"
#include <coppa/oscquery/device/remote.hpp>

auto parameter(const std::shared_ptr<coppa::ow::Node>& node)
{
    return static_cast<coppa::ow::Device*>(node->getDevice().get())->dev().safeMap().get(node->destination());
}

coppa::ow::Address::Address(std::shared_ptr<coppa::ow::Node> parent):
    m_parent{parent}
{

}

const std::shared_ptr<OSSIA::Node> coppa::ow::Address::getNode() const
{
    return m_parent;
}

const OSSIA::Value* coppa::ow::Address::pullValue()
{
    return getValue();
}

OSSIA::Address&coppa::ow::Address::pushValue(const OSSIA::Value* v)
{
    return setValue(v);
}

OSSIA::Value* coppaToOSSIAValue(coppa::Variant val)
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

const OSSIA::Value* coppa::ow::Address::getValue() const
{
    coppa::oscquery::Parameter p = static_cast<coppa::ow::Device*>(m_parent->getDevice().get())->dev().safeMap().get(m_parent->destination());

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

OSSIA::Address&coppa::ow::Address::setValue(
        const OSSIA::Value* v)
{
    if(!v)
        return *this;

    coppa::oscquery::Parameter p = static_cast<coppa::ow::Device*>(m_parent->getDevice().get())->dev().get(m_parent->destination());
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
    }

    static_cast<coppa::ow::Device*>(m_parent->getDevice().get())->dev().set(p.destination, p);

    return *this;
}

OSSIA::Value::Type coppa::ow::Address::getValueType() const
{
    auto val = getValue();
    if(val)
        return val->getType();
    else
        throw;
}

OSSIA::Address::AccessMode coppa::ow::Address::getAccessMode() const
{
    coppa::oscquery::Parameter p = static_cast<coppa::ow::Device*>(m_parent->getDevice().get())->dev().get(m_parent->destination());
    switch(p.accessmode)
    {
        case coppa::Access::Mode::None:
            return AccessMode::BI;
            break;
        case coppa::Access::Mode::Get:
            return AccessMode::BI;
            break;
        case coppa::Access::Mode::Set:
            return AccessMode::BI;
            break;
        case coppa::Access::Mode::Both:
            return AccessMode::BI;
    }

    return {};

}

OSSIA::Address&coppa::ow::Address::setAccessMode(OSSIA::Address::AccessMode)
{
    return *this;
}

class coppaDomain : public OSSIA::Domain
{
        coppa::oscquery::Range m_domain;

    public:
        coppaDomain(coppa::oscquery::Range d):
            m_domain{d}
        {

        }

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

const std::shared_ptr<OSSIA::Domain>& coppa::ow::Address::getDomain() const
{
    coppa::oscquery::Parameter p = static_cast<coppa::ow::Device*>(m_parent->getDevice().get())->dev().get(m_parent->destination());

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

OSSIA::Address&coppa::ow::Address::setDomain(std::shared_ptr<OSSIA::Domain>)
{
    return *this;
}

OSSIA::Address::BoundingMode coppa::ow::Address::getBoundingMode() const
{
    return BoundingMode::FREE;
}

OSSIA::Address&coppa::ow::Address::setBoundingMode(OSSIA::Address::BoundingMode)
{
    return *this;
}

bool coppa::ow::Address::getRepetitionFilter() const
{
    return false;
}

OSSIA::Address&coppa::ow::Address::setRepetitionFilter(bool)
{
    return *this;
}
