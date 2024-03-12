#pragma once

#include <bringauto/fleet_protocol/http_client/RequestFrequencyGuard.hpp>

#include <gtest/gtest.h>


using namespace bringauto::fleet_protocol::http_client;


class RequestFrequencyGuardTests_F: public ::testing::Test {
protected:
	void SetUp() override {
		RequestFrequencyGuard::RequestFrequencyGuardConfig config {};
		config.maxRequestsThresholdCount = 5;
		config.maxRequestsThresholdPeriodMs = std::chrono::milliseconds(10);
		config.delayAfterThresholdReachedMs = std::chrono::milliseconds(5000);
		config.retryRequestsDelayMs = std::chrono::milliseconds(200);
		requestFrequencyGuard = std::make_unique<RequestFrequencyGuard>(config);
	}

	std::unique_ptr<RequestFrequencyGuard> requestFrequencyGuard;
};