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



#include "bringauto-fleet-http-client-generated/model/Car.h"

namespace org {
namespace openapitools {
namespace client {
namespace model {



Car::Car()
{
    m_Company_name = utility::conversions::to_string_t("");
    m_Company_nameIsSet = false;
    m_Car_name = utility::conversions::to_string_t("");
    m_Car_nameIsSet = false;
}

Car::~Car()
{
}

void Car::validate()
{
    // TODO: implement validation
}

web::json::value Car::toJson() const
{

    web::json::value val = web::json::value::object();
    
    if(m_Company_nameIsSet)
    {
        val[utility::conversions::to_string_t(U("company_name"))] = ModelBase::toJson(m_Company_name);
    }
    if(m_Car_nameIsSet)
    {
        val[utility::conversions::to_string_t(U("car_name"))] = ModelBase::toJson(m_Car_name);
    }

    return val;
}

bool Car::fromJson(const web::json::value& val)
{
    bool ok = true;
    
    if(val.has_field(utility::conversions::to_string_t(U("company_name"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("company_name")));
        if(!fieldValue.is_null())
        {
            utility::string_t refVal_setCompanyName;
            ok &= ModelBase::fromJson(fieldValue, refVal_setCompanyName);
            setCompanyName(refVal_setCompanyName);
        }
    }
    if(val.has_field(utility::conversions::to_string_t(U("car_name"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("car_name")));
        if(!fieldValue.is_null())
        {
            utility::string_t refVal_setCarName;
            ok &= ModelBase::fromJson(fieldValue, refVal_setCarName);
            setCarName(refVal_setCarName);
        }
    }
    return ok;
}

void Car::toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix) const
{
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }
    if(m_Company_nameIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("company_name")), m_Company_name));
    }
    if(m_Car_nameIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("car_name")), m_Car_name));
    }
}

bool Car::fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix)
{
    bool ok = true;
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }

    if(multipart->hasContent(utility::conversions::to_string_t(U("company_name"))))
    {
        utility::string_t refVal_setCompanyName;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("company_name"))), refVal_setCompanyName );
        setCompanyName(refVal_setCompanyName);
    }
    if(multipart->hasContent(utility::conversions::to_string_t(U("car_name"))))
    {
        utility::string_t refVal_setCarName;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("car_name"))), refVal_setCarName );
        setCarName(refVal_setCarName);
    }
    return ok;
}

utility::string_t Car::getCompanyName() const
{
    return m_Company_name;
}

void Car::setCompanyName(const utility::string_t& value)
{
    m_Company_name = value;
    m_Company_nameIsSet = true;
}

bool Car::companyNameIsSet() const
{
    return m_Company_nameIsSet;
}

void Car::unsetCompany_name()
{
    m_Company_nameIsSet = false;
}
utility::string_t Car::getCarName() const
{
    return m_Car_name;
}

void Car::setCarName(const utility::string_t& value)
{
    m_Car_name = value;
    m_Car_nameIsSet = true;
}

bool Car::carNameIsSet() const
{
    return m_Car_nameIsSet;
}

void Car::unsetCar_name()
{
    m_Car_nameIsSet = false;
}
}
}
}
}


