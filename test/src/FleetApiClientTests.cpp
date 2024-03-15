#define _TURN_OFF_PLATFORM_STRING

#include <bringauto/fleet_protocol/http_client/FleetApiClient.hpp>
#include <TestConstants.hpp>

#include <chrono>
#include <gtest/gtest.h>
#include <stdio.h>


using namespace bringauto::fleet_protocol::http_client;


/**
 * @brief Ensure getCommands and getStatuses functions trigger the delay mechanism; requests are sent to a non existent api 
 */
TEST(FleetApiClientTests, DelayRepeatedRequests) {
	FleetApiClient::FleetApiClientConfig facConfig {
		.apiUrl = "http://localhost:8080",
		.apiKey = "test",
		.companyName = "test",
		.carName = "test"
	};
	
	RequestFrequencyGuard::RequestFrequencyGuardConfig rfgConfig {
		.maxRequestsThresholdCount = MAX_REQUEST_THRESHOLD_COUNT,
		.maxRequestsThresholdPeriodMs = std::chrono::milliseconds(MAX_REQUEST_THRESHOLD_PERIOD_MS),
		.delayAfterThresholdReachedMs = std::chrono::milliseconds(DELAY_AFTER_THRESHOLD_REACHED_MS),
		.retryRequestsDelayMs = std::chrono::milliseconds(RETRY_REQUESTS_DELAY_MS)
	};

	auto fleetApiClient = std::make_unique<FleetApiClient>(facConfig, rfgConfig);
	auto timeBefore = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	// Do MAX_REQUEST_THRESHOLD_COUNT requests with no delay which should trigger the threshold
	std::cout << "Expecting " << DELAY_AFTER_THRESHOLD_REACHED_MS << "ms delay" << std::endl;
	for(int i = 0; i < MAX_REQUEST_THRESHOLD_COUNT; i++) {
		fleetApiClient->getCommands(0, true);
	}

	auto timeAfter = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	ASSERT_GE(timeAfter - timeBefore, DELAY_AFTER_THRESHOLD_REACHED_MS);
	ASSERT_LT(timeAfter - timeBefore, MAX_DELAY_AFTER_THRESHOLD_REACHED_MS);

	timeBefore = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	// The next MAX_REQUEST_THRESHOLD_COUNT requests should be delayed by RETRY_REQUESTS_DELAY_MS
	for(int i = 0; i < MAX_REQUEST_THRESHOLD_COUNT; i++) {
		std::cout << "Expecting " << RETRY_REQUESTS_DELAY_MS << "ms delay" << std::endl;
		fleetApiClient->getStatuses(0, true);
	}

	timeAfter = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	ASSERT_GE(timeAfter - timeBefore, MAX_REQUEST_THRESHOLD_COUNT * RETRY_REQUESTS_DELAY_MS);
	ASSERT_LT(timeAfter - timeBefore, MAX_REQUEST_THRESHOLD_COUNT * MAX_RETRY_REQUESTS_DELAY_MS);
}