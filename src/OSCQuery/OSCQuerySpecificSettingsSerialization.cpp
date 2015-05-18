#include <iscore/serialization/DataStreamVisitor.hpp>
#include <iscore/serialization/JSONVisitor.hpp>
#include "OSCQuerySpecificSettings.hpp"


template<>
void Visitor<Reader<DataStream>>::readFrom(const OSCQuerySpecificSettings& n)
{
    m_stream << n.port << n.host;
    insertDelimiter();
}

template<>
void Visitor<Writer<DataStream>>::writeTo(OSCQuerySpecificSettings& n)
{
    m_stream >> n.port >> n.host;
    checkDelimiter();
}

template<>
void Visitor<Reader<JSONObject>>::readFrom(const OSCQuerySpecificSettings& n)
{
    m_obj["Port"] = n.port;
    m_obj["Host"] = n.host;
}

template<>
void Visitor<Writer<JSONObject>>::writeTo(OSCQuerySpecificSettings& n)
{
    n.port = m_obj["Port"].toInt();
    n.host = m_obj["Host"].toString();
}
