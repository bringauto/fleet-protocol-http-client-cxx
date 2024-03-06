#include <bringauto/fleet_protocol/http_client/FleetApiClient.hpp>
#include <bringauto/logging/ConsoleSink.hpp>
#include <bringauto/logging/Logger.hpp>
#include <bringauto/fleet_protocol/cxx/DeviceID.hpp>


using namespace bringauto::fleet_protocol::http_client;
using namespace bringauto::logging;

std::unique_ptr<FleetApiClient> fleetApiClient;

void createFleetApiClient() {
	FleetApiClient::FleetApiClientConfig facConfig = {"http://localhost:8080",
													  "StaticAccessKeyToBeUsedByDevelopersOnEtna",
													  "bringauto", "virtual_vehicle"};
	RequestFrequencyGuard::RequestFrequencyGuardConfig rfgConfig = {3, 1000, 500, 220};
	fleetApiClient = std::make_unique<FleetApiClient>(facConfig, rfgConfig);
	fleetApiClient->setDeviceIdentification(bringauto::fleet_protocol::cxx::DeviceID(1, 1, 1, "autonomy", "virtual-vehicle"));
}

void getCars() {
	try {
		auto cars = fleetApiClient->getCars();

		std::cout << "Cars:" << std::endl;
		for(auto &car: cars) {
			std::cout << "  " << car->getCompanyName() << " / " << car->getCarName() << std::endl;
		}
	} catch(const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}

void getCommands() {
	auto commands = fleetApiClient->getCommands();

	std::cout << "Commands:" << std::endl;
	for(auto command: commands) {
		std::cout << "  Timestamp: " << command->getTimestamp() << std::endl;
		std::cout << "  Payload: " << command->getPayload()->getData()->getJson().serialize() << std::endl;
	}
}

void getStatuses() {
	auto statuses = fleetApiClient->getStatuses();

	std::cout << "Statuses:" << std::endl;
	for(auto status: statuses) {
		std::cout << "  Timestamp: " << status->getTimestamp() << std::endl;
		std::cout << "  Payload: " << status->getPayload()->getData()->getJson().serialize() << std::endl;
	}
}

void sendCommand() {
	try {
		fleetApiClient->sendCommand("{}");
	} catch(const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}

void sendStatus() {
	try {
		fleetApiClient->sendStatus("{}");
	} catch(const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}

void getAvailableDevices() {
	try {
		auto availableDevices = fleetApiClient->getAvailableDevices(1);

		std::cout << "All module's devices:" << std::endl;
		for(auto device: availableDevices->getDeviceList()) {
			std::cout << "  " << device->getName() << std::endl;
		}
	} catch(const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}

int main(int argc, char **argv) {
	Logger::addSink<ConsoleSink>();
	Logger::LoggerSettings params {"FleetHttpClientExample",
								   Logger::Verbosity::Debug};
	Logger::init(params);

	createFleetApiClient();
	getCars();
	getCommands();
	getStatuses();
	sendCommand();
	sendStatus();
	getAvailableDevices();
}