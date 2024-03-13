#pragma once

#include <bringauto/fleet_protocol/http_client/RequestFrequencyGuard.hpp>

#include <gtest/gtest.h>


using namespace bringauto::fleet_protocol::http_client;


class RequestFrequencyGuardTests_F: public ::testing::Test {
protected:
	void SetUp() override {
		RequestFrequencyGuard::RequestFrequencyGuardConfig config {
			.maxRequestsThresholdCount = 5,
			.maxRequestsThresholdPeriodMs = std::chrono::milliseconds(10),
			.delayAfterThresholdReachedMs = std::chrono::milliseconds(5000),
			.retryRequestsDelayMs = std::chrono::milliseconds(200)
		};
		requestFrequencyGuard = std::make_unique<RequestFrequencyGuard>(config);
	}

	std::unique_ptr<RequestFrequencyGuard> requestFrequencyGuard {};
};