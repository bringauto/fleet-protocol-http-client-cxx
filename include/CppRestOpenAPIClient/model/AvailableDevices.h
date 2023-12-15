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

/*
 * AvailableDevices.h
 *
 * 
 */

#ifndef ORG_OPENAPITOOLS_CLIENT_MODEL_AvailableDevices_H_
#define ORG_OPENAPITOOLS_CLIENT_MODEL_AvailableDevices_H_


#include "CppRestOpenAPIClient/ModelBase.h"

#include "CppRestOpenAPIClient/model/Module.h"
#include "CppRestOpenAPIClient/model/DeviceId.h"
#include <vector>

namespace org {
namespace openapitools {
namespace client {
namespace model {

class DeviceId;

/// <summary>
/// 
/// </summary>
class  AvailableDevices
    : public ModelBase
{
public:
    AvailableDevices();
    virtual ~AvailableDevices();

    /////////////////////////////////////////////
    /// ModelBase overrides

    void validate() override;

    web::json::value toJson() const override;
    bool fromJson(const web::json::value& json) override;

    void toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) const override;
    bool fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& namePrefix) override;

    /////////////////////////////////////////////
    /// AvailableDevices members

    /// <summary>
    /// Id (unsigned integer) of the module.
    /// </summary>
    int32_t getModuleId() const;
    bool moduleIdIsSet() const;
    void unsetModule_id();

    void setModuleId(int32_t value);

    /// <summary>
    /// List of Ids of devices contained in the module.
    /// </summary>
    std::vector<std::shared_ptr<DeviceId>>& getDeviceList();
    bool deviceListIsSet() const;
    void unsetDevice_list();

    void setDeviceList(const std::vector<std::shared_ptr<DeviceId>>& value);


protected:
    int32_t m_Module_id;
    bool m_Module_idIsSet;
    std::vector<std::shared_ptr<DeviceId>> m_Device_list;
    bool m_Device_listIsSet;
};


}
}
}
}

#endif /* ORG_OPENAPITOOLS_CLIENT_MODEL_AvailableDevices_H_ */
