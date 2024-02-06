#pragma once

#include <vector>


class RequestFrequencyGuard
{
public:
    /**
     * @brief Constructs the request frequency guard
     * @param maxRequestsThresholdCount max amount of allowed requests in the specified time period
     * @param maxRequestsThresholdPeriodMs time period in which the amount of requests is checked
     * @param delayAfterThresholdReachedMs time to sleep for in ms when threshold is initially reached
     * @param retryRequestsDelayMs delay in ms between requests until request rate is no longer over the threshold
     */
    RequestFrequencyGuard(int32_t maxRequestsThresholdCount, int32_t maxRequestsThresholdPeriodMs,
                          int32_t delayAfterThresholdReachedMs, int32_t retryRequestsDelayMs);

    ~RequestFrequencyGuard();

    /**
     * @brief Determines how long the next request should be delayed when the max request rate is reached
     * @param currentTimestamp timestamp that gets added to a container, used to calculate if the max request threshold is reached
     */
    void handleDelays(int64_t currentTimestamp);

private:
    int32_t maxRequestsThresholdCount_;
    int32_t maxRequestsThresholdPeriodMs_;
    int32_t delayAfterThresholdReachedMs_;
    int32_t retryRequestsDelayMs_;

    bool thresholdReached_ = false;
    std::vector<int64_t> msgTimestamps_;

    /**
     * @brief Checks if the current request rate is over the threshold
     * @return true if there are more requests than allowed within the set period
     */
    bool isOverThreshold();
};