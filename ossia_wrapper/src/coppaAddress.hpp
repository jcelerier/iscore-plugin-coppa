#pragma once

#include <Network/Address.h>
#include <Editor/Value.h>
#include <Editor/Domain.h>

namespace coppa
{
namespace oscquery
{
class remote_device;
}
namespace ow // ossia wrapper
{

class Node;
class Address : public OSSIA::Address
{
        std::shared_ptr<coppa::ow::Node> m_parent;

        mutable std::shared_ptr<OSSIA::Domain> m_domain;

        coppa::oscquery::remote_device& dev() const;
    public:
        Address(std::shared_ptr<coppa::ow::Node> parent);
        const std::shared_ptr<OSSIA::Node> getNode() const override;

        const OSSIA::Value* pullValue() override;

        OSSIA::Address& pushValue(const OSSIA::Value* v) override;

        const OSSIA::Value* getValue() const override;

        const OSSIA::Value* cloneValue(std::vector<char> = {}) const override;

        Address & setValueType(OSSIA::Value::Type) override;

        OSSIA::Address& setValue(const OSSIA::Value* v) override;

        OSSIA::Value::Type getValueType() const override;

        OSSIA::AccessMode getAccessMode() const override;

        OSSIA::Address& setAccessMode(OSSIA::AccessMode) override;

        const std::shared_ptr<OSSIA::Domain>& getDomain() const override;

        OSSIA::Address& setDomain(std::shared_ptr<OSSIA::Domain>) override;

        OSSIA::BoundingMode getBoundingMode() const override;

        OSSIA::Address& setBoundingMode(OSSIA::BoundingMode) override;

        bool getRepetitionFilter() const override;

        OSSIA::Address& setRepetitionFilter(bool) override;
};

}
}
