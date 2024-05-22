#include <bringauto/fleet_protocol/http_client/FleetApiClient.hpp>
#include <bringauto/fleet_protocol/http_client/settings/Constants.hpp>

#include <chrono>

using namespace org::openapitools::client;


namespace bringauto::fleet_protocol::http_client {


FleetApiClient::FleetApiClient(
	const FleetApiClientConfig &facConfig, 
	const RequestFrequencyGuard::RequestFrequencyGuardConfig &rfgConfig):
	companyName_(facConfig.companyName),
	carName_(facConfig.carName)
{
	auto apiConfigPtr = std::make_shared<api::ApiConfiguration>();
	apiConfigPtr->setBaseUrl(facConfig.apiUrl);
	apiConfigPtr->setApiKey(settings::Constants::API_KEY_HEADER_KEY, facConfig.apiKey);
	apiClientPtr_ = std::make_shared<api::ApiClient>();
	apiClientPtr_->setConfiguration(apiConfigPtr);

	carApi_ = std::make_unique<api::CarApi>(apiClientPtr_);
	deviceApi_ = std::make_unique<api::DeviceApi>(apiClientPtr_);
	moduleApi_ = std::make_unique<api::ModuleApi>(apiClientPtr_);

	deviceIdPtr_ = std::make_shared<model::DeviceId>();
	messagePtr_ = std::make_shared<model::Message>();
	payloadPtr_ = std::make_shared<model::Payload>();
	payloadDataPtr_ = std::make_shared<model::Payload_data>();

	setDeviceIdentification(fleet_protocol::cxx::DeviceID(0, 0, 0,
		settings::Constants::DEFAULT_DEVICE_ROLE, settings::Constants::DEFAULT_DEVICE_NAME
	));

	payloadPtr_->setEncoding(settings::Constants::PAYLOAD_ENCODING);

	payloadPtr_->setData(payloadDataPtr_);
	messagePtr_->setDeviceId(deviceIdPtr_);
	messagePtr_->setPayload(payloadPtr_);

	requestFrequencyGuard_ = std::make_unique<RequestFrequencyGuard>(rfgConfig);
}


void FleetApiClient::setDeviceIdentification(const fleet_protocol::cxx::DeviceID &deviceId) {
	auto deviceIdentification = deviceId.getDeviceId();
	deviceIdPtr_->setModuleId(deviceIdentification.module);
	deviceIdPtr_->setType(deviceIdentification.device_type);
	deviceIdPtr_->setRole(std::string(static_cast<char*>(deviceIdentification.device_role.data)));
	deviceIdPtr_->setName(std::string(static_cast<char*>(deviceIdentification.device_name.data)));
}


std::vector<std::shared_ptr<model::Car>> FleetApiClient::getCars(std::optional<int64_t> since, std::optional<bool> wait) {
	auto carsRequest = carApi_->availableCars(wait.value_or(false), since.value_or(0));
	std::vector<std::shared_ptr<model::Car>> cars;

	try {
		cars = carsRequest.get();
	} catch(std::exception &e) {
	}

	if(wait) {
		requestFrequencyGuard_->handleDelays(
			std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
	}
	return cars;
}


std::vector<std::shared_ptr<model::Message>> FleetApiClient::getCommands(std::optional<int64_t> since, std::optional<bool> wait) {
	auto commandsRequest = deviceApi_->listCommands(companyName_, carName_, since.value_or(0), wait.value_or(false));
	std::vector<std::shared_ptr<model::Message>> commands;

	try {
		commands = commandsRequest.get();
	} catch(std::exception &e) {
	}

	if(wait) {
		requestFrequencyGuard_->handleDelays(
			std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
	}
	return commands;
}


std::vector<std::shared_ptr<model::Message>> FleetApiClient::getStatuses(std::optional<int64_t> since, std::optional<bool> wait) {
	auto statusesRequest = deviceApi_->listStatuses(companyName_, carName_, since.value_or(0), wait.value_or(false));
	std::vector<std::shared_ptr<model::Message>> statuses;

	try {
		statuses = statusesRequest.get();
	} catch(std::exception &e) {
	}

	if(wait) {
		requestFrequencyGuard_->handleDelays(
			std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
	}
	return statuses;
}


void FleetApiClient::sendCommand(const std::string &commandJson) {
	payloadPtr_->setMessageType(settings::Constants::COMMAND_MESSAGE_TYPE);
	payloadDataPtr_->setJson(web::json::value::parse(commandJson));
	messagePtr_->setTimestamp(utility::datetime::utc_timestamp());

	std::vector<std::shared_ptr<model::Message>> commands;
	commands.push_back(messagePtr_);

	auto commandsRequest = deviceApi_->sendCommands(companyName_, carName_, commands);
	commandsRequest.wait();
}


void FleetApiClient::sendStatus(const std::string &statusJson, StatusType statusType) {
	switch(statusType) {
		case StatusType::STATUS:
			payloadPtr_->setMessageType(settings::Constants::STATUS_MESSAGE_TYPE);
			break;
		case StatusType::STATUS_ERROR:
			payloadPtr_->setMessageType(settings::Constants::STATUS_ERROR_MESSAGE_TYPE);
			break;
		default:
			throw std::invalid_argument("Invalid status type");
	}
	payloadDataPtr_->setJson(web::json::value::parse(statusJson));
	messagePtr_->setTimestamp(utility::datetime::utc_timestamp());

	std::vector<std::shared_ptr<model::Message>> statuses;
	statuses.push_back(messagePtr_);

	auto statusesRequest = deviceApi_->sendStatuses(companyName_, carName_, statuses);
	statusesRequest.wait();
}


std::shared_ptr<model::AvailableDevices> FleetApiClient::getAvailableDevices(std::optional<int32_t> moduleId) {
	boost::optional<int32_t> moduleIdBoost = boost::none;
	if(moduleId.has_value()) {
		moduleIdBoost = moduleId.value();
	}

	auto availableDevicesRequest = moduleApi_->availableDevices(companyName_, carName_, moduleIdBoost);
	return availableDevicesRequest.get();
}

}// namespace bringauto::fleet_protocol::http_client
