#pragma once

#include <bringauto/fleet_protocol/http_client/RequestFrequencyGuard.hpp>

#include <gtest/gtest.h>


using namespace bringauto::fleet_protocol::http_client;


class RequestFrequencyGuardTests_F: public ::testing::Test {
protected:
	void SetUp() override {
		RequestFrequencyGuard::RequestFrequencyGuardConfig config {};
		config.maxRequestsThresholdCount = 5;
		config.maxRequestsThresholdPeriodMs = 10;
		config.delayAfterThresholdReachedMs = 5000;
		config.retryRequestsDelayMs = 200;
		requestFrequencyGuard = std::make_unique<RequestFrequencyGuard>(config);
	}

	std::unique_ptr<RequestFrequencyGuard> requestFrequencyGuard;
};