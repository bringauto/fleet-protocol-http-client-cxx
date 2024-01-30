#include "RequestFrequencyGuard.hpp"

#include <unistd.h>


RequestFrequencyGuard::RequestFrequencyGuard(int maxRequestsThresholdCount, int maxRequestsThresholdPeriodMs,
                                             int delayAfterThresholdReachedMs, int retryRequestsDelayMs) {
    this->maxRequestsThresholdCount = maxRequestsThresholdCount;
    this->maxRequestsThresholdPeriodMs = maxRequestsThresholdPeriodMs;
    this->delayAfterThresholdReachedMs = delayAfterThresholdReachedMs;
    this->retryRequestsDelayMs = retryRequestsDelayMs;
}


RequestFrequencyGuard::~RequestFrequencyGuard() {

}


void RequestFrequencyGuard::handleDelays(unsigned long currentTimestamp) {
    msgTimestamps.insert(msgTimestamps.begin(), currentTimestamp);

    if (!thresholdReached && isOverThreshold()) {
        thresholdReached = true;
        msgTimestamps.clear();
        usleep(delayAfterThresholdReachedMs * 1000);
        return;
    }

    if (thresholdReached) {
        usleep(retryRequestsDelayMs * 1000);

        if (msgTimestamps.size() >= maxRequestsThresholdCount) {
            thresholdReached = false;
            msgTimestamps.pop_back();
        }
    }
}


bool RequestFrequencyGuard::isOverThreshold() {
    bool retVal = false;
    
    if (msgTimestamps.size() >= maxRequestsThresholdCount) {
        if ((msgTimestamps.front() - msgTimestamps.back()) < maxRequestsThresholdPeriodMs)
            retVal = true;

        msgTimestamps.pop_back();
    }

    return retVal;
}