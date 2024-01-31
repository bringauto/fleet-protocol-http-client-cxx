#define _TURN_OFF_PLATFORM_STRING

#include "FleetApiClient.hpp"

#include <gtest/gtest.h>
#include <stdio.h>
#include <chrono>


TEST(FleetApiClientTests, DelayRepeatedRequests) {
    auto fleetApiClient = std::make_shared<FleetApiClient>("http://localhost:8080", "test", "test", "test", 5, 10, 5000, 200);
    auto timeBefore = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    
    std::cout << "Expecting 5s delay" << std::endl;
    for (int i = 0; i < 5; i++) {
        fleetApiClient->getCommands(0, true);
    }

    auto timeAfter = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    ASSERT_GE(timeAfter - timeBefore, 5000);
    ASSERT_LT(timeAfter - timeBefore, 5100);

    timeBefore = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    
    for (int i = 0; i < 5; i++) {
        std::cout << "Expecting 200ms delay" << std::endl;
        fleetApiClient->getStatuses(0, true);
    }

    timeAfter = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    ASSERT_GE(timeAfter - timeBefore, 1000);
    ASSERT_LT(timeAfter - timeBefore, 1100);
}