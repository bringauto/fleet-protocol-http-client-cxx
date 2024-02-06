#include <bringauto/fleet_protocol/http_client/FleetApiClient.hpp>



using namespace bringauto::fleet_protocol::http_client;

std::unique_ptr<FleetApiClient> fleetApiClient;

void createFleetApiClient() {
    fleetApiClient = std::make_unique<FleetApiClient>("http://localhost:8080",
                                                      "StaticAccessKeyToBeUsedByDevelopersOnEtna",
                                                      "bringauto", "virtual_vehicle",
                                                      3, 1000, 500, 220);
    fleetApiClient->setDeviceIdentification(1, 1, "autonomy", "virtual-vehicle");
}

void getCars() {
    try {
        auto cars = fleetApiClient->getCars();

        std::cout << "Cars:" << std::endl;
        for (auto &car : cars) {
            std::cout << "  " << car->getCompanyName() << " / " << car->getCarName() << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void getCommands() {
    auto commands = fleetApiClient->getCommands(0, false);

    std::cout << "Commands:" << std::endl;
    for (auto command : commands) {
        std::cout << "  Timestamp: " << command->getTimestamp() << std::endl;
        std::cout << "  Payload: " << command->getPayload()->getData()->getJson().serialize() << std::endl;
    }
}

void getStatuses() {
    auto statuses = fleetApiClient->getStatuses(0, false);

    std::cout << "Statuses:" << std::endl;
    for (auto status : statuses) {
        std::cout << "  Timestamp: " << status->getTimestamp() << std::endl;
        std::cout << "  Payload: " << status->getPayload()->getData()->getJson().serialize() << std::endl;
    }
}

void sendCommand() {
    try {
        fleetApiClient->sendCommand("{}");
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void sendStatus() {
    try {
        fleetApiClient->sendStatus("{}");
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

void getAvailableDevices() {
    try {
        auto availableDevices = fleetApiClient->getAvailableDevices(1);

        std::cout << "Module 1 devices:" << std::endl;
        for (auto device : availableDevices->getDeviceList()) {
            std::cout << "  " << device->getName() << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

int main(int argc, char** argv) {
    createFleetApiClient();
    getCars();
    getCommands();
    getStatuses();
    sendCommand();
    sendStatus();
    getAvailableDevices();
}