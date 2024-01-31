#pragma once

#include "RequestFrequencyGuard.hpp"

#include <gtest/gtest.h>


class RequestFrequencyGuardTests_F : public ::testing::Test
{
protected:
    void SetUp() override
    {
        requestFrequencyGuard = std::make_shared<RequestFrequencyGuard>(
            5, // maxRequestsThresholdCount
            10, // maxRequestsThresholdPeriodMs
            5000, // delayAfterThresholdReachedMs
            200); // retryRequestsDelayMs
    }

    std::shared_ptr<RequestFrequencyGuard> requestFrequencyGuard;
};