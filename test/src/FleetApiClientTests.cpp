#define _TURN_OFF_PLATFORM_STRING

#include <bringauto/fleet_protocol/http_client/FleetApiClient.hpp>

#include <chrono>
#include <gtest/gtest.h>
#include <stdio.h>


using namespace bringauto::fleet_protocol::http_client;


/**
 * @brief Ensure getCommands and getStatuses functions trigger the delay mechanism; requests are sent to a non existent api 
 */
TEST(FleetApiClientTests, DelayRepeatedRequests) {
	FleetApiClient::FleetApiClientConfig facConfig {};
	facConfig.apiUrl = "http://localhost:8080";
	facConfig.apiKey = "test";
	facConfig.companyName = "test";
	facConfig.carName = "test";
	
	RequestFrequencyGuard::RequestFrequencyGuardConfig rfgConfig {};
	rfgConfig.maxRequestsThresholdCount = 5;
	rfgConfig.maxRequestsThresholdPeriodMs = std::chrono::milliseconds(10);
	rfgConfig.delayAfterThresholdReachedMs = std::chrono::milliseconds(5000);
	rfgConfig.retryRequestsDelayMs = std::chrono::milliseconds(200);

	auto fleetApiClient = std::make_unique<FleetApiClient>(facConfig, rfgConfig);
	auto timeBefore = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	// Do 5 requests with no delay which should trigger the threshold
	std::cout << "Expecting 5s delay" << std::endl;
	for(int i = 0; i < 5; i++) {
		fleetApiClient->getCommands(0, true);
	}

	auto timeAfter = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	ASSERT_GE(timeAfter - timeBefore, 5000);
	ASSERT_LT(timeAfter - timeBefore, 5100);

	timeBefore = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	// The next 5 requests should be delayed by 200ms
	for(int i = 0; i < 5; i++) {
		std::cout << "Expecting 200ms delay" << std::endl;
		fleetApiClient->getStatuses(0, true);
	}

	timeAfter = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	ASSERT_GE(timeAfter - timeBefore, 1000);
	ASSERT_LT(timeAfter - timeBefore, 1100);
}