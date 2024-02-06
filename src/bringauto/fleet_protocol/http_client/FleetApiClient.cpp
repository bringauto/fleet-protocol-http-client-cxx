#include <bringauto/fleet_protocol/http_client/FleetApiClient.hpp>
#include <bringauto/fleet_protocol/http_client/settings/Constants.hpp>

#include <chrono>

using namespace org::openapitools::client;


namespace bringauto::fleet_protocol::http_client {


FleetApiClient::FleetApiClient(const std::string& apiUrl, const std::string& apiKey, const std::string& companyName, const std::string& carName,
                               int32_t maxRequestsThresholdCount, int32_t maxRequestsThresholdPeriodMs,
                               int32_t delayAfterThresholdReachedMs, int32_t retryRequestsDelayMs):
    companyName_(companyName),
    carName_(carName) {
    auto apiConfigPtr = std::make_shared<api::ApiConfiguration>();
    apiConfigPtr->setBaseUrl(apiUrl);
    apiConfigPtr->setApiKey(settings::Constants::API_KEY_HEADER_KEY, apiKey);
    apiClientPtr_ = std::make_shared<api::ApiClient>();    
    apiClientPtr_->setConfiguration(apiConfigPtr);

    carApi_ = std::make_shared<api::CarApi>(apiClientPtr_);
    deviceApi_ = std::make_shared<api::DeviceApi>(apiClientPtr_);
    moduleApi_ = std::make_shared<api::ModuleApi>(apiClientPtr_);

    deviceIdPtr_ = std::make_shared<model::DeviceId>();
    messagePtr_ = std::make_shared<model::Message>();
    payloadPtr_ = std::make_shared<model::Payload>();
    payloadDataPtr_ = std::make_shared<model::Payload_data>();

    setDeviceIdentification(0, 0, settings::Constants::DEFAULT_COMPANY_NAME, settings::Constants::DEFAULT_CAR_NAME);
    payloadPtr_->setEncoding(settings::Constants::PAYLOAD_ENCODING);

    payloadPtr_->setData(payloadDataPtr_);
    messagePtr_->setDeviceId(deviceIdPtr_);
    messagePtr_->setPayload(payloadPtr_);

    requestFrequencyGuard_ = std::make_shared<RequestFrequencyGuard>(maxRequestsThresholdCount, maxRequestsThresholdPeriodMs,
                                                                     delayAfterThresholdReachedMs, retryRequestsDelayMs);
}


void FleetApiClient::setDeviceIdentification(int32_t moduleId, int32_t deviceType, const std::string& deviceRole, const std::string& deviceName) {
    deviceIdPtr_->setModuleId(moduleId);
    deviceIdPtr_->setType(deviceType);
    deviceIdPtr_->setRole(deviceRole);
    deviceIdPtr_->setName(deviceName);
}


std::vector<std::shared_ptr<model::Car>> FleetApiClient::getCars() {
    auto carsRequest = carApi_->availableCars();
    return carsRequest.get();
}


std::vector<std::shared_ptr<model::Message>> FleetApiClient::getCommands(int64_t since, bool wait) {
    auto commandsRequest = deviceApi_->listCommands(companyName_, carName_, since, wait);
    std::vector<std::shared_ptr<model::Message>> commands;

    try {
        commands = commandsRequest.get();
    } catch (std::exception& e) {
    }

    if (wait) {
        requestFrequencyGuard_->handleDelays(
            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()
        );
    }
    return commands;
}


std::vector<std::shared_ptr<model::Message>> FleetApiClient::getStatuses(int64_t since, bool wait) {
    auto statusesRequest = deviceApi_->listStatuses(companyName_, carName_, since, wait);
    std::vector<std::shared_ptr<model::Message>> statuses;

    try {
        statuses = statusesRequest.get();
    } catch (std::exception& e) {
    }

    if (wait) {
        requestFrequencyGuard_->handleDelays(
            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()
        );
    }
    return statuses;
}


void FleetApiClient::sendCommand(const std::string& commandJson) {
    payloadPtr_->setMessageType(settings::Constants::COMMAND_MESSAGE_TYPE);
    payloadDataPtr_->setJson(web::json::value::parse(commandJson));
    messagePtr_->setTimestamp(utility::datetime::utc_timestamp());
    
    std::vector<std::shared_ptr<model::Message>> commands;
    commands.push_back(messagePtr_);

    auto commandsRequest = deviceApi_->sendCommands(companyName_, carName_, commands);
    commandsRequest.wait();
}


void FleetApiClient::sendStatus(const std::string& statusJson) {
    payloadPtr_->setMessageType(settings::Constants::STATUS_MESSAGE_TYPE);
    payloadDataPtr_->setJson(web::json::value::parse(statusJson));
    messagePtr_->setTimestamp(utility::datetime::utc_timestamp());
    
    std::vector<std::shared_ptr<model::Message>> statuses;
    statuses.push_back(messagePtr_);

    auto statusesRequest = deviceApi_->sendStatuses(companyName_, carName_, statuses);
    statusesRequest.wait();
}


std::shared_ptr<model::AvailableDevices> FleetApiClient::getAvailableDevices(std::optional<int32_t> moduleId) {
    boost::optional<int32_t> moduleIdBoost = boost::none;
    if (moduleId.has_value()) {
        moduleIdBoost = moduleId.value();
    }

    auto availableDevicesRequest = moduleApi_->availableDevices(companyName_, carName_, moduleIdBoost);
    return availableDevicesRequest.get();
}

}
