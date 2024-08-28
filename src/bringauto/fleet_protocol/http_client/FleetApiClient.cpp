#include <bringauto/fleet_protocol/http_client/FleetApiClient.hpp>
#include <bringauto/fleet_protocol/http_client/settings/Constants.hpp>

using namespace org::openapitools::client;


namespace bringauto::fleet_protocol::http_client {


FleetApiClient::FleetApiClient(
	const FleetApiClientConfig &facConfig, 
	const RequestFrequencyGuard::RequestFrequencyGuardConfig &rfgConfig):
	companyName_(facConfig.companyName),
	carName_(facConfig.carName)
{
	const auto apiConfigPtr = std::make_shared<api::ApiConfiguration>();
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

	setDeviceIdentification(cxx::DeviceID(0, 0, 0,
		settings::Constants::DEFAULT_DEVICE_ROLE, settings::Constants::DEFAULT_DEVICE_NAME
	));

	payloadPtr_->setEncoding(settings::Constants::PAYLOAD_ENCODING);

	payloadPtr_->setData(payloadDataPtr_);
	messagePtr_->setDeviceId(deviceIdPtr_);
	messagePtr_->setPayload(payloadPtr_);

	requestFrequencyGuard_ = std::make_unique<RequestFrequencyGuard>(rfgConfig);
}


void FleetApiClient::setDeviceIdentification(const cxx::DeviceID &deviceId) const {
	auto deviceIdentification = deviceId.getDeviceId();
	deviceIdPtr_->setModuleId(deviceIdentification.module);
	deviceIdPtr_->setType(deviceIdentification.device_type);
	deviceIdPtr_->setRole(std::string(static_cast<char*>(deviceIdentification.device_role.data)));
	deviceIdPtr_->setName(std::string(static_cast<char*>(deviceIdentification.device_name.data)));
}

std::pair<std::vector<std::shared_ptr<model::Car>>, FleetApiClient::ReturnCode> FleetApiClient::getCars(
	const std::optional<int64_t> since, const std::optional<bool> wait) const
{
	const auto carsRequest = carApi_->availableCars(wait.value_or(false), since.value_or(0));
	std::vector<std::shared_ptr<model::Car>> cars {};
	auto rc = ReturnCode::OK;

	try {
		cars = carsRequest.get();
	} catch(std::exception &) {
	}

	if(wait && requestFrequencyGuard_->handleDelays()) {
		rc = ReturnCode::DELAYED;
	}
	return {cars, rc};
}


std::pair<std::vector<std::shared_ptr<model::Message>>, FleetApiClient::ReturnCode> FleetApiClient::getCommands(
	const std::optional<int64_t> since, const std::optional<bool> wait) const
{
	const auto commandsRequest = deviceApi_->listCommands(companyName_, carName_, since.value_or(0), wait.value_or(false));
	std::vector<std::shared_ptr<model::Message>> commands {};
	auto rc = ReturnCode::OK;

	try {
		commands = commandsRequest.get();
	} catch(std::exception &) {
	}

	if(wait && requestFrequencyGuard_->handleDelays()) {
		rc = ReturnCode::DELAYED;
	}
	return {commands, rc};
}


std::pair<std::vector<std::shared_ptr<model::Message>>, FleetApiClient::ReturnCode> FleetApiClient::getStatuses(
	const std::optional<int64_t> since, const std::optional<bool>& wait) const
{
	const auto statusesRequest = deviceApi_->listStatuses(companyName_, carName_, since.value_or(0), wait.value_or(false));
	std::vector<std::shared_ptr<model::Message>> statuses {};
	auto rc = ReturnCode::OK;

	try {
		statuses = statusesRequest.get();
	} catch(std::exception &) {
	}

	if(wait && requestFrequencyGuard_->handleDelays()) {
		rc = ReturnCode::DELAYED;
	}
	return {statuses, rc};
}


void FleetApiClient::sendCommand(const std::string &commandJson) const {
	payloadPtr_->setMessageType(settings::Constants::COMMAND_MESSAGE_TYPE);
	payloadDataPtr_->setJson(web::json::value::parse(commandJson));
	messagePtr_->setTimestamp(utility::datetime::utc_timestamp());

	std::vector<std::shared_ptr<model::Message>> commands;
	commands.push_back(messagePtr_);

	const auto commandsRequest = deviceApi_->sendCommands(companyName_, carName_, commands);
	commandsRequest.wait();
}


void FleetApiClient::sendStatus(const std::string &statusJson, const StatusType statusType) const {
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

	const auto statusesRequest = deviceApi_->sendStatuses(companyName_, carName_, statuses);
	statusesRequest.wait();
}


std::shared_ptr<model::AvailableDevices> FleetApiClient::getAvailableDevices(std::optional<int32_t> moduleId) const {
	boost::optional<int32_t> moduleIdBoost = boost::none;
	if(moduleId.has_value()) {
		moduleIdBoost = moduleId.value();
	}

	const auto availableDevicesRequest = moduleApi_->availableDevices(companyName_, carName_, moduleIdBoost);
	return availableDevicesRequest.get();
}

}// namespace bringauto::fleet_protocol::http_client
