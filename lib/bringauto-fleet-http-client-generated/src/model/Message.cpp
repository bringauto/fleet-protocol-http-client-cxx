/**
 * Fleet v2 HTTP API
 * HTTP-based API for Fleet Protocol v2 serving for communication between the External Server and the end users.
 *
 * The version of the OpenAPI document: 2.4.0
 * Contact: jiri.strouhal@bringauto.com
 *
 * NOTE: This class is auto generated by OpenAPI-Generator 7.2.0.
 * https://openapi-generator.tech
 * Do not edit the class manually.
 */



#include "bringauto-fleet-http-client-generated/model/Message.h"

namespace org {
namespace openapitools {
namespace client {
namespace model {



Message::Message()
{
    m_Timestamp = 0;
    m_TimestampIsSet = false;
    m_Device_idIsSet = false;
    m_PayloadIsSet = false;
}

Message::~Message()
{
}

void Message::validate()
{
    // TODO: implement validation
}

web::json::value Message::toJson() const
{

    web::json::value val = web::json::value::object();
    
    if(m_TimestampIsSet)
    {
        val[utility::conversions::to_string_t(U("timestamp"))] = ModelBase::toJson(m_Timestamp);
    }
    if(m_Device_idIsSet)
    {
        val[utility::conversions::to_string_t(U("device_id"))] = ModelBase::toJson(m_Device_id);
    }
    if(m_PayloadIsSet)
    {
        val[utility::conversions::to_string_t(U("payload"))] = ModelBase::toJson(m_Payload);
    }

    return val;
}

bool Message::fromJson(const web::json::value& val)
{
    bool ok = true;
    
    if(val.has_field(utility::conversions::to_string_t(U("timestamp"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("timestamp")));
        if(!fieldValue.is_null())
        {
            int64_t refVal_setTimestamp;
            ok &= ModelBase::fromJson(fieldValue, refVal_setTimestamp);
            setTimestamp(refVal_setTimestamp);
        }
    }
    if(val.has_field(utility::conversions::to_string_t(U("device_id"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("device_id")));
        if(!fieldValue.is_null())
        {
            std::shared_ptr<DeviceId> refVal_setDeviceId;
            ok &= ModelBase::fromJson(fieldValue, refVal_setDeviceId);
            setDeviceId(refVal_setDeviceId);
        }
    }
    if(val.has_field(utility::conversions::to_string_t(U("payload"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("payload")));
        if(!fieldValue.is_null())
        {
            std::shared_ptr<Payload> refVal_setPayload;
            ok &= ModelBase::fromJson(fieldValue, refVal_setPayload);
            setPayload(refVal_setPayload);
        }
    }
    return ok;
}

void Message::toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix) const
{
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }
    if(m_TimestampIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("timestamp")), m_Timestamp));
    }
    if(m_Device_idIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("device_id")), m_Device_id));
    }
    if(m_PayloadIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("payload")), m_Payload));
    }
}

bool Message::fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix)
{
    bool ok = true;
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }

    if(multipart->hasContent(utility::conversions::to_string_t(U("timestamp"))))
    {
        int64_t refVal_setTimestamp;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("timestamp"))), refVal_setTimestamp );
        setTimestamp(refVal_setTimestamp);
    }
    if(multipart->hasContent(utility::conversions::to_string_t(U("device_id"))))
    {
        std::shared_ptr<DeviceId> refVal_setDeviceId;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("device_id"))), refVal_setDeviceId );
        setDeviceId(refVal_setDeviceId);
    }
    if(multipart->hasContent(utility::conversions::to_string_t(U("payload"))))
    {
        std::shared_ptr<Payload> refVal_setPayload;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("payload"))), refVal_setPayload );
        setPayload(refVal_setPayload);
    }
    return ok;
}

int64_t Message::getTimestamp() const
{
    return m_Timestamp;
}

void Message::setTimestamp(int64_t value)
{
    m_Timestamp = value;
    m_TimestampIsSet = true;
}

bool Message::timestampIsSet() const
{
    return m_TimestampIsSet;
}

void Message::unsetTimestamp()
{
    m_TimestampIsSet = false;
}
std::shared_ptr<DeviceId> Message::getDeviceId() const
{
    return m_Device_id;
}

void Message::setDeviceId(const std::shared_ptr<DeviceId>& value)
{
    m_Device_id = value;
    m_Device_idIsSet = true;
}

bool Message::deviceIdIsSet() const
{
    return m_Device_idIsSet;
}

void Message::unsetDevice_id()
{
    m_Device_idIsSet = false;
}
std::shared_ptr<Payload> Message::getPayload() const
{
    return m_Payload;
}

void Message::setPayload(const std::shared_ptr<Payload>& value)
{
    m_Payload = value;
    m_PayloadIsSet = true;
}

bool Message::payloadIsSet() const
{
    return m_PayloadIsSet;
}

void Message::unsetPayload()
{
    m_PayloadIsSet = false;
}
}
}
}
}


