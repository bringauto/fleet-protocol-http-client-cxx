#include <bringauto/fleet_protocol/http_client/FleetApiClient.hpp>
#include <bringauto/fleet_protocol/cxx/DeviceID.hpp>


using namespace bringauto::fleet_protocol::http_client;

std::unique_ptr<FleetApiClient> fleetApiClient;

void createFleetApiClient() {
	FleetApiClient::FleetApiClientConfig facConfig {
		.apiUrl = "http://localhost:8080/v2/protocol",
		.apiKey = "ProtocolStaticAccessKey",
		.companyName = "bringauto",
		.carName = "virtual_vehicle"
	};

	RequestFrequencyGuard::RequestFrequencyGuardConfig rfgConfig {
		.maxRequestsThresholdCount = 3,
		.maxRequestsThresholdPeriodMs = std::chrono::milliseconds(1000),
		.delayAfterThresholdReachedMs = std::chrono::milliseconds(500),
		.retryRequestsDelayMs = std::chrono::milliseconds(220)
	};

	fleetApiClient = std::make_unique<FleetApiClient>(facConfig, rfgConfig);
	fleetApiClient->setDeviceIdentification(bringauto::fleet_protocol::cxx::DeviceID(1, 1, 1, "autonomy", "virtual-vehicle"));
}

void getCars() {
	auto [cars, rc] = fleetApiClient->getCars();
	if (rc != FleetApiClient::ReturnCode::OK) {
		std::cerr << "getCars() function failed" << std::endl;
		return;
	}

	std::cout << "Cars:" << std::endl;
	for(const auto &car: cars) {
		std::cout << "  " << car->getCompanyName() << " / " << car->getCarName() << std::endl;
	}
}

void getCommands() {
	auto [commands, rc] = fleetApiClient->getCommands();
	if (rc != FleetApiClient::ReturnCode::OK) {
		std::cerr << "getCommands() function failed" << std::endl;
		return;
	}

	std::cout << "Commands:" << std::endl;
	for(const auto& command: commands) {
		std::cout << "  Timestamp: " << command->getTimestamp() << std::endl;
		std::cout << "  Payload: " << command->getPayload()->getData()->getJson().serialize() << std::endl;
	}
}

void getStatuses() {
	auto [statuses, rc] = fleetApiClient->getStatuses();
	if (rc != FleetApiClient::ReturnCode::OK) {
		std::cerr << "getStatuses() function failed" << std::endl;
		return;
	}

	std::cout << "Statuses:" << std::endl;
	for(const auto& status: statuses) {
		std::cout << "  Timestamp: " << status->getTimestamp() << std::endl;
		std::cout << "  Payload: " << status->getPayload()->getData()->getJson().serialize() << std::endl;
	}
}

void sendCommand() {
	if (fleetApiClient->sendCommand("{}") != FleetApiClient::ReturnCode::OK) {
		std::cerr << "sendCommand() function failed" << std::endl;
	} else {
		std::cout << "Command sent successfuly" << std::endl;
	}
}

void sendStatus() {
	if (fleetApiClient->sendStatus("{}") != FleetApiClient::ReturnCode::OK) {
		std::cerr << "sendStatus() function failed" << std::endl;
	} else {
		std::cout << "Status sent successfuly" << std::endl;
	}
}

void getAvailableDevices() {
		auto [availableDevices, rc] = fleetApiClient->getAvailableDevices(1);
		if (rc != FleetApiClient::ReturnCode::OK) {
			std::cerr << "getAvailableDevices() function failed" << std::endl;
			return;
		}

		std::cout << "All devices of module 1:" << std::endl;
		for(const auto& device: availableDevices->getDeviceList()) {
			std::cout << "  " << device->getName() << std::endl;
		}
}

int main(int argc, char **argv) {
	createFleetApiClient();
	getCars();
	getCommands();
	getStatuses();
	sendCommand();
	sendStatus();
	getAvailableDevices();
}
