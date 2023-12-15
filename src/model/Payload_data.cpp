/**
 * Fleet v2 HTTP API
 * HTTP-based API for Fleet Protocol v2 serving for communication between the External Server and the end users.
 *
 * The version of the OpenAPI document: 1.0.0
 * Contact: jiri.strouhal@bringauto.com
 *
 * NOTE: This class is auto generated by OpenAPI-Generator 7.1.0.
 * https://openapi-generator.tech
 * Do not edit the class manually.
 */



#include "CppRestOpenAPIClient/model/Payload_data.h"

namespace org {
namespace openapitools {
namespace client {
namespace model {



Payload_data::Payload_data()
{
}

Payload_data::~Payload_data()
{
}

void Payload_data::validate()
{
    // TODO: implement validation
}

web::json::value Payload_data::toJson() const
{

    web::json::value val = web::json::value::object();
    

    return val;
}

bool Payload_data::fromJson(const web::json::value& val)
{
    bool ok = true;
    
    return ok;
}

void Payload_data::toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix) const
{
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }
}

bool Payload_data::fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix)
{
    bool ok = true;
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }

    return ok;
}

}
}
}
}


