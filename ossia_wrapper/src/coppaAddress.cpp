#include "coppaAddress.hpp"
#include "coppaNode.hpp"

coppa::ow::Address::Address(std::shared_ptr<coppa::ow::Node> parent):
    m_parent{parent}
{

}

const std::shared_ptr<OSSIA::Node> coppa::ow::Address::getNode() const
{
    return m_parent;
}

const OSSIA::Value*coppa::ow::Address::pullValue()
{
    return nullptr;
}

OSSIA::Address&coppa::ow::Address::pushValue(const OSSIA::Value* v)
{
    return *this;
}

const OSSIA::Value*coppa::ow::Address::getValue() const
{
    return nullptr;
}

OSSIA::Address&coppa::ow::Address::setValue(const OSSIA::Value* v)
{
    return *this;
}

OSSIA::Value::Type coppa::ow::Address::getValueType() const
{
    return OSSIA::Value::Type::IMPULSE;
}

OSSIA::Address::AccessMode coppa::ow::Address::getAccessMode() const
{
    return AccessMode::BI;
}

OSSIA::Address&coppa::ow::Address::setAccessMode(OSSIA::Address::AccessMode)
{
    return *this;
}

const std::shared_ptr<OSSIA::Domain>&coppa::ow::Address::getDomain() const
{
    return nullptr;
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
