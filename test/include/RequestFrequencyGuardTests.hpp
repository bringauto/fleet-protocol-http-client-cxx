#pragma once

#include <bringauto/fleet_protocol/http_client/RequestFrequencyGuard.hpp>

#include <gtest/gtest.h>


using namespace bringauto::fleet_protocol::http_client;


class RequestFrequencyGuardTests_F: public ::testing::Test {
protected:
	void SetUp() override {
		RequestFrequencyGuard::RequestFrequencyGuardConfig config = {
				5,   // maxRequestsThresholdCount
				10,  // maxRequestsThresholdPeriodMs
				5000,// delayAfterThresholdReachedMs
				200  // retryRequestsDelayMs
		};
		requestFrequencyGuard = std::make_unique<RequestFrequencyGuard>(config);
	}

	std::unique_ptr<RequestFrequencyGuard> requestFrequencyGuard;
};