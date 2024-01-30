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
    RequestFrequencyGuard(int maxRequestsThresholdCount, int maxRequestsThresholdPeriodMs,
                          int delayAfterThresholdReachedMs, int retryRequestsDelayMs);

    ~RequestFrequencyGuard();

    /**
     * @brief Determines how long the next request should be delayed when the max request rate is reached
     * @param currentTimestamp timestamp that gets added to a container, used to calculate if the max request threshold is reached
     */
    void handleDelays(unsigned long currentTimestamp);

private:
    int maxRequestsThresholdCount;
    int maxRequestsThresholdPeriodMs;
    int delayAfterThresholdReachedMs;
    int retryRequestsDelayMs;

    bool thresholdReached = false;
    std::vector<long> msgTimestamps;

    /**
     * @brief Checks if the current request rate is over the threshold
     * @return true if there are more requests than allowed within the set period
     */
    bool isOverThreshold();
};