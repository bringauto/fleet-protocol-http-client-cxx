#pragma once

#include "CppRestOpenAPIClient/api/CarApi.h"
#include "CppRestOpenAPIClient/api/DeviceApi.h"
#include "CppRestOpenAPIClient/api/ModuleApi.h"
#include "RequestFrequencyGuard.hpp"

#include <optional>


using namespace org::openapitools::client;

class FleetApiClient
{
public:
    FleetApiClient(std::string apiUrl, std::string apiKey, std::string companyName, std::string carName,
                   int maxRequestsThresholdCount = 10, int maxRequestsThresholdPeriodMs = 5000,
                   int delayAfterThresholdReachedMs = 5000, int retryRequestsDelayMs = 200);
    ~FleetApiClient();

    void setDeviceIdentification(int moduleId, int deviceType, std::string deviceRole, std::string deviceName);

    std::vector<std::shared_ptr<model::Car>> getCars();
    std::vector<std::shared_ptr<model::Message>> getCommands(long since = 0, bool wait = false);
    std::vector<std::shared_ptr<model::Message>> getStatuses(long since = 0, bool wait = false);
    void sendCommand(std::string commandJson);
    void sendStatus(std::string statusJson);
    std::shared_ptr<model::AvailableDevices> getAvailableDevices(std::optional<int> moduleId);

private:
    std::shared_ptr<RequestFrequencyGuard> requestFrequencyGuard;

    std::shared_ptr<api::ApiClient> apiClientPtr;
    std::shared_ptr<api::CarApi> carApi;
    std::shared_ptr<api::DeviceApi> deviceApi;
    std::shared_ptr<api::ModuleApi> moduleApi;
    std::string companyName;
    std::string carName;

    std::shared_ptr<model::DeviceId> deviceIdPtr;
    std::shared_ptr<model::Message> messagePtr;
    std::shared_ptr<model::Payload> payloadPtr;
    std::shared_ptr<model::Payload_data> payloadDataPtr;
};