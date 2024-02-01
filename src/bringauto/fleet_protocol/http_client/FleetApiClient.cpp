#include <bringauto/fleet_protocol/http_client/FleetApiClient.hpp>

#include <chrono>

using namespace org::openapitools::client;


namespace bringauto::fleet_protocol::http_client {


FleetApiClient::FleetApiClient(std::string apiUrl, std::string apiKey, std::string companyName, std::string carName,
                               int32_t maxRequestsThresholdCount, int32_t maxRequestsThresholdPeriodMs,
                               int32_t delayAfterThresholdReachedMs, int32_t retryRequestsDelayMs) {
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

    setDeviceIdentification(0, 0, "none", "none");
    payloadPtr->setEncoding("JSON");

    payloadPtr->setData(payloadDataPtr);
    messagePtr->setDeviceId(deviceIdPtr);
    messagePtr->setPayload(payloadPtr);

    requestFrequencyGuard = std::make_shared<RequestFrequencyGuard>(maxRequestsThresholdCount, maxRequestsThresholdPeriodMs,
                                                                    delayAfterThresholdReachedMs, retryRequestsDelayMs);
}

FleetApiClient::~FleetApiClient() {

}


void FleetApiClient::setDeviceIdentification(int32_t moduleId, int32_t deviceType, std::string deviceRole, std::string deviceName) {
    deviceIdPtr->setModuleId(moduleId);
    deviceIdPtr->setType(deviceType);
    deviceIdPtr->setRole(deviceRole);
    deviceIdPtr->setName(deviceName);
}


std::vector<std::shared_ptr<model::Car>> FleetApiClient::getCars() {
    auto carsRequest = carApi->availableCars();
    return carsRequest.get();
}


std::vector<std::shared_ptr<model::Message>> FleetApiClient::getCommands(int64_t since, bool wait) {
    auto commandsRequest = deviceApi->listCommands(companyName, carName, since, wait);
    std::vector<std::shared_ptr<model::Message>> commands;

    try {
        commands = commandsRequest.get();
    } catch (std::exception& e) {
    }

    if (wait) {
        requestFrequencyGuard->handleDelays(
            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()
        );
    }
    return commands;
}


std::vector<std::shared_ptr<model::Message>> FleetApiClient::getStatuses(int64_t since, bool wait) {
    auto statusesRequest = deviceApi->listStatuses(companyName, carName, since, wait);
    std::vector<std::shared_ptr<model::Message>> statuses;

    try {
        statuses = statusesRequest.get();
    } catch (std::exception& e) {
    }

    if (wait) {
        requestFrequencyGuard->handleDelays(
            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()
        );
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


std::shared_ptr<model::AvailableDevices> FleetApiClient::getAvailableDevices(std::optional<int32_t> moduleId) {
    boost::optional<int32_t> moduleIdBoost = boost::none;
    if (moduleId.has_value()) {
        moduleIdBoost = moduleId.value();
    }

    auto availableDevicesRequest = moduleApi->availableDevices(companyName, carName, moduleIdBoost);
    return availableDevicesRequest.get();
}

}
