#pragma once

#include <vector>


class RequestFrequencyGuard
{
public:
    RequestFrequencyGuard(int maxRequestsThresholdCount, int maxRequestsThresholdPeriodMs,
                          int delayAfterThresholdReachedMs, int retryRequestsDelayMs);
    ~RequestFrequencyGuard();

    void handleDelays(unsigned long currentTimestamp);

private:
    int maxRequestsThresholdCount;
    int maxRequestsThresholdPeriodMs;
    int delayAfterThresholdReachedMs;
    int retryRequestsDelayMs;

    bool thresholdReached = false;
    std::vector<long> msgTimestamps;

    bool isOverThreshold();
};