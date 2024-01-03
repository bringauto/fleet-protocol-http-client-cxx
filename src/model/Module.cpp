/**
 * Fleet v2 HTTP API
 * HTTP-based API for Fleet Protocol v2 serving for communication between the External Server and the end users.
 *
 * The version of the OpenAPI document: 2.0.0
 * Contact: jiri.strouhal@bringauto.com
 *
 * NOTE: This class is auto generated by OpenAPI-Generator 7.1.0.
 * https://openapi-generator.tech
 * Do not edit the class manually.
 */



#include "CppRestOpenAPIClient/model/Module.h"

namespace org {
namespace openapitools {
namespace client {
namespace model {



Module::Module()
{
    m_Module_id = 0;
    m_Module_idIsSet = false;
    m_Device_listIsSet = false;
}

Module::~Module()
{
}

void Module::validate()
{
    // TODO: implement validation
}

web::json::value Module::toJson() const
{

    web::json::value val = web::json::value::object();
    
    if(m_Module_idIsSet)
    {
        val[utility::conversions::to_string_t(U("module_id"))] = ModelBase::toJson(m_Module_id);
    }
    if(m_Device_listIsSet)
    {
        val[utility::conversions::to_string_t(U("device_list"))] = ModelBase::toJson(m_Device_list);
    }

    return val;
}

bool Module::fromJson(const web::json::value& val)
{
    bool ok = true;
    
    if(val.has_field(utility::conversions::to_string_t(U("module_id"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("module_id")));
        if(!fieldValue.is_null())
        {
            int32_t refVal_setModuleId;
            ok &= ModelBase::fromJson(fieldValue, refVal_setModuleId);
            setModuleId(refVal_setModuleId);
        }
    }
    if(val.has_field(utility::conversions::to_string_t(U("device_list"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("device_list")));
        if(!fieldValue.is_null())
        {
            std::vector<std::shared_ptr<DeviceId>> refVal_setDeviceList;
            ok &= ModelBase::fromJson(fieldValue, refVal_setDeviceList);
            setDeviceList(refVal_setDeviceList);
        }
    }
    return ok;
}

void Module::toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix) const
{
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }
    if(m_Module_idIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("module_id")), m_Module_id));
    }
    if(m_Device_listIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("device_list")), m_Device_list));
    }
}

bool Module::fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix)
{
    bool ok = true;
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }

    if(multipart->hasContent(utility::conversions::to_string_t(U("module_id"))))
    {
        int32_t refVal_setModuleId;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("module_id"))), refVal_setModuleId );
        setModuleId(refVal_setModuleId);
    }
    if(multipart->hasContent(utility::conversions::to_string_t(U("device_list"))))
    {
        std::vector<std::shared_ptr<DeviceId>> refVal_setDeviceList;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("device_list"))), refVal_setDeviceList );
        setDeviceList(refVal_setDeviceList);
    }
    return ok;
}

int32_t Module::getModuleId() const
{
    return m_Module_id;
}

void Module::setModuleId(int32_t value)
{
    m_Module_id = value;
    m_Module_idIsSet = true;
}

bool Module::moduleIdIsSet() const
{
    return m_Module_idIsSet;
}

void Module::unsetModule_id()
{
    m_Module_idIsSet = false;
}
std::vector<std::shared_ptr<DeviceId>>& Module::getDeviceList()
{
    return m_Device_list;
}

void Module::setDeviceList(const std::vector<std::shared_ptr<DeviceId>>& value)
{
    m_Device_list = value;
    m_Device_listIsSet = true;
}

bool Module::deviceListIsSet() const
{
    return m_Device_listIsSet;
}

void Module::unsetDevice_list()
{
    m_Device_listIsSet = false;
}
}
}
}
}


