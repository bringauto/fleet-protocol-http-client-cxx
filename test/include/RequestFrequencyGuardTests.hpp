#pragma once

#include <bringauto/fleet_protocol/http_client/RequestFrequencyGuard.hpp>
#include <TestConstants.hpp>

#include <gtest/gtest.h>


using namespace bringauto::fleet_protocol::http_client;


class RequestFrequencyGuardTests_F: public ::testing::Test {
protected:
	void SetUp() override {
		RequestFrequencyGuard::RequestFrequencyGuardConfig config {
			.maxRequestsThresholdCount = MAX_REQUEST_THRESHOLD_COUNT,
			.maxRequestsThresholdPeriodMs = std::chrono::milliseconds(MAX_REQUEST_THRESHOLD_PERIOD_MS),
			.delayAfterThresholdReachedMs = std::chrono::milliseconds(DELAY_AFTER_THRESHOLD_REACHED_MS),
			.retryRequestsDelayMs = std::chrono::milliseconds(RETRY_REQUESTS_DELAY_MS)
		};
		requestFrequencyGuard = std::make_unique<RequestFrequencyGuard>(config);
	}

	std::unique_ptr<RequestFrequencyGuard> requestFrequencyGuard {};
};