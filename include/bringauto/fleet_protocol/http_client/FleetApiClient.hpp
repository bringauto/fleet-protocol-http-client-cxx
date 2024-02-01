#pragma once

#include "CppRestOpenAPIClient/api/CarApi.h"
#include "CppRestOpenAPIClient/api/DeviceApi.h"
#include "CppRestOpenAPIClient/api/ModuleApi.h"
#include "RequestFrequencyGuard.hpp"

#include <optional>


namespace bringauto::fleet_protocol::http_client {


class FleetApiClient
{
public:
    /**
     * @brief Constructs the fleet API client
     * @param apiUrl URL of Fleet v2 HTTP API
     * @param apiKey api key used for authorization
     * @param companyName company identifier in the API
     * @param carName car identifier in the API
     * @param maxRequestsThresholdCount max amount of allowed requests in the specified time period (only for getCommands/getStatuses)
     * @param maxRequestsThresholdPeriodMs time period in which the amount of requests is checked (only for getCommands/getStatuses)
     * @param delayAfterThresholdReachedMs time to sleep for in ms when threshold is initially reached (only for getCommands/getStatuses)
     * @param retryRequestsDelayMs delay in ms between requests until request rate is no longer over the threshold (only for getCommands/getStatuses)
     */
    FleetApiClient(std::string apiUrl, std::string apiKey, std::string companyName, std::string carName,
                   int32_t maxRequestsThresholdCount = 10, int32_t maxRequestsThresholdPeriodMs = 5000,
                   int32_t delayAfterThresholdReachedMs = 5000, int32_t retryRequestsDelayMs = 200);

    ~FleetApiClient();

    /**
     * @brief Sets the DeviceId parameters, required for sendCommand and sendStatuse functions
     * @param moduleId moduleId of device
     * @param deviceType type of device
     * @param deviceRole role of device
     * @param deviceName name of device
     */
    void setDeviceIdentification(int32_t moduleId, int32_t deviceType, std::string deviceRole, std::string deviceName);

    /**
     * @brief Calls the GET function on /cars of Fleet v2 HTTP API
     * @return Vector of shared pointers to the Car model 
     */
    std::vector<std::shared_ptr<org::openapitools::client::model::Car>> getCars();

    /**
     * @brief Calls the GET function on /command/{company_name}/{car_name} of Fleet v2 HTTP API
     * @param since optional, minimal timestamp of commands to look for
     * @param wait optional, if true, waits for a predefined period until any command is found, will check request rate and possibly delay them
     * @return Vector of shared pointers to the Message model containing commands in payload data
     */
    std::vector<std::shared_ptr<org::openapitools::client::model::Message>> getCommands(int64_t since = 0, bool wait = false);

    /**
     * @brief Calls the GET function on /status/{company_name}/{car_name} of Fleet v2 HTTP API
     * @param since optional, minimal timestamp of statuses to look for
     * @param wait optional, if true, waits for a predefined period until any status is found, will check request rate and possibly delay them
     * @return Vector of shared pointers to the Message model containing statuses in payload data
     */
    std::vector<std::shared_ptr<org::openapitools::client::model::Message>> getStatuses(int64_t since = 0, bool wait = false);

    /**
     * Calls the POST function on /command/{company_name}/{car_name} of Fleet v2 HTTP API.
     * setDeviceIdentification needs to be used beforehand to set DeviceId, otherwise placeholder values will be used.
     * @param commandJson payload data of a command represented by a json as a string
     */
    void sendCommand(std::string commandJson);

    /**
     * Calls the POST function on /status/{company_name}/{car_name} of Fleet v2 HTTP API.
     * setDeviceIdentification needs to be used beforehand to set DeviceId, otherwise placeholder values will be used.
     * @param statusJson payload data of a status represented by a json as a string
     */
    void sendStatus(std::string statusJson);

    /**
     * @brief Calls the GET function on /available-devices/{company_name}/{car_name} of Fleet v2 HTTP API
     * @param moduleId optional, filters returned devices to only those with matching module Id
     * @return Shared pointer to the AvailableDevices model
     */
    //TODO might not work properly with api because it probably returns an array
    std::shared_ptr<org::openapitools::client::model::AvailableDevices> getAvailableDevices(std::optional<int32_t> moduleId);

private:
    std::shared_ptr<RequestFrequencyGuard> requestFrequencyGuard;

    std::shared_ptr<org::openapitools::client::api::ApiClient> apiClientPtr;
    std::shared_ptr<org::openapitools::client::api::CarApi> carApi;
    std::shared_ptr<org::openapitools::client::api::DeviceApi> deviceApi;
    std::shared_ptr<org::openapitools::client::api::ModuleApi> moduleApi;
    std::string companyName;
    std::string carName;

    std::shared_ptr<org::openapitools::client::model::DeviceId> deviceIdPtr;
    std::shared_ptr<org::openapitools::client::model::Message> messagePtr;
    std::shared_ptr<org::openapitools::client::model::Payload> payloadPtr;
    std::shared_ptr<org::openapitools::client::model::Payload_data> payloadDataPtr;
};

}