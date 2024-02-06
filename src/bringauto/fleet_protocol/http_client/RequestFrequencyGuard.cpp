#include <bringauto/fleet_protocol/http_client/RequestFrequencyGuard.hpp>

#include <unistd.h>
#include <chrono>
#include <thread>


RequestFrequencyGuard::RequestFrequencyGuard(int32_t maxRequestsThresholdCount, int32_t maxRequestsThresholdPeriodMs,
                                             int32_t delayAfterThresholdReachedMs, int32_t retryRequestsDelayMs) {
    maxRequestsThresholdCount_ = maxRequestsThresholdCount;
    maxRequestsThresholdPeriodMs_ = maxRequestsThresholdPeriodMs;
    delayAfterThresholdReachedMs_ = delayAfterThresholdReachedMs;
    retryRequestsDelayMs_ = retryRequestsDelayMs;
}


void RequestFrequencyGuard::handleDelays(int64_t currentTimestamp) {
    msgTimestamps_.insert(msgTimestamps_.begin(), currentTimestamp);

    if (!thresholdReached_ && isOverThreshold()) {
        thresholdReached_ = true;
        msgTimestamps_.clear();
        std::this_thread::sleep_for(std::chrono::milliseconds(delayAfterThresholdReachedMs_));
        return;
    }

    if (thresholdReached_) {
        std::this_thread::sleep_for(std::chrono::milliseconds(retryRequestsDelayMs_));

        if (msgTimestamps_.size() >= maxRequestsThresholdCount_) {
            thresholdReached_ = false;
            msgTimestamps_.pop_back();
        }
    }
}


bool RequestFrequencyGuard::isOverThreshold() {
    bool retVal = false;
    
    if (msgTimestamps_.size() >= maxRequestsThresholdCount_) {
        if ((msgTimestamps_.front() - msgTimestamps_.back()) < maxRequestsThresholdPeriodMs_)
            retVal = true;

        msgTimestamps_.pop_back();
    }

    return retVal;
}