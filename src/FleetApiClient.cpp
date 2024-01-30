#include "include/FleetApiClient.hpp"

using namespace org::openapitools::client;


FleetApiClient::FleetApiClient(std::string apiUrl, std::string apiKey, std::string companyName, std::string carName,
                               int maxRequestsThresholdCount, int maxRequestsThresholdPeriodMs,
                               int delayAfterThresholdReachedMs, int retryRequestsDelayMs) {
    auto apiConfigPtr = std::make_shared<api::ApiConfiguration>();
    apiConfigPtr->setBaseUrl(apiUrl);
    apiConfigPtr->setApiKey("api_key", apiKey);
    apiClientPtr = std::make_shared<api::ApiClient>();    
    apiClientPtr->setConfiguration(apiConfigPtr);

    carApi = std::make_shared<api::CarApi>(apiClientPtr);
    deviceApi = std::make_shared<api::DeviceApi>(apiClientPtr);
    moduleApi = std::make_shared<api::ModuleApi>(apiClientPtr);

    this->companyName = companyName;
    this->carName = carName;

    deviceIdPtr = std::make_shared<model::DeviceId>();
    messagePtr = std::make_shared<model::Message>();
    payloadPtr = std::make_shared<model::Payload>();
    payloadDataPtr = std::make_shared<model::Payload_data>();

    setDeviceIdentification(0, 0, "None", "None");
    payloadPtr->setEncoding("JSON");

    payloadPtr->setData(payloadDataPtr);
    messagePtr->setDeviceId(deviceIdPtr);
    messagePtr->setPayload(payloadPtr);

    requestFrequencyGuard = std::make_shared<RequestFrequencyGuard>(maxRequestsThresholdCount, maxRequestsThresholdPeriodMs,
                                                                    delayAfterThresholdReachedMs, retryRequestsDelayMs);
}


void FleetApiClient::setDeviceIdentification(int moduleId, int deviceType, std::string deviceRole, std::string deviceName) {
    deviceIdPtr->setModuleId(moduleId);
    deviceIdPtr->setType(deviceType);
    deviceIdPtr->setRole(deviceRole);
    deviceIdPtr->setName(deviceName);
}


std::vector<std::shared_ptr<model::Car>> FleetApiClient::getCars() {
    auto carsRequest = carApi->availableCars();
    return carsRequest.get();
}


std::vector<std::shared_ptr<model::Message>> FleetApiClient::getCommands(long since, bool wait) {
    auto commandsRequest = deviceApi->listCommands(companyName, carName, since, wait);
    auto commands = commandsRequest.get();
    if (wait) {
        requestFrequencyGuard->handleDelays(utility::datetime::utc_now().to_interval());
    }
    return commands;
}


std::vector<std::shared_ptr<model::Message>> FleetApiClient::getStatuses(long since, bool wait) {
    auto statusesRequest = deviceApi->listStatuses(companyName, carName, since, wait);
    auto statuses = statusesRequest.get();
    if (wait) {
        requestFrequencyGuard->handleDelays(utility::datetime::utc_now().to_interval());
    }
    return statuses;
}


void FleetApiClient::sendCommand(std::string commandJson) {
    payloadPtr->setMessageType("COMMAND");
    payloadDataPtr->setJson(web::json::value::parse(commandJson));
    messagePtr->setTimestamp(utility::datetime::utc_timestamp());
    
    std::vector<std::shared_ptr<model::Message>> commands;
    commands.push_back(messagePtr);

    auto commandsRequest = deviceApi->sendCommands(companyName, carName, commands);
    commandsRequest.wait();
}


void FleetApiClient::sendStatus(std::string statusJson) {
    payloadPtr->setMessageType("STATUS");
    payloadDataPtr->setJson(web::json::value::parse(statusJson));
    messagePtr->setTimestamp(utility::datetime::utc_timestamp());
    
    std::vector<std::shared_ptr<model::Message>> statuses;
    statuses.push_back(messagePtr);

    auto statusesRequest = deviceApi->sendStatuses(companyName, carName, statuses);
    statusesRequest.wait();
}


std::shared_ptr<model::AvailableDevices> FleetApiClient::getAvailableDevices(std::optional<int> moduleId) {
    boost::optional<int32_t> moduleIdBoost = boost::none;
    if (moduleId.has_value()) {
        moduleIdBoost = moduleId.value();
    }

    auto availableDevicesRequest = moduleApi->availableDevices(companyName, carName, moduleIdBoost);
    return availableDevicesRequest.get();
}
