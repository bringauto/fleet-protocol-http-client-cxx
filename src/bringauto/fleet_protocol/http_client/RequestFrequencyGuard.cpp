#include <bringauto/fleet_protocol/http_client/RequestFrequencyGuard.hpp>
#include <bringauto/logging/Logger.hpp>

#include <chrono>
#include <thread>


namespace bringauto::fleet_protocol::http_client {

using namespace bringauto::logging;


RequestFrequencyGuard::RequestFrequencyGuard(const RequestFrequencyGuardConfig &config):
	maxRequestsThresholdCount_(config.maxRequestsThresholdCount),
	maxRequestsThresholdPeriodMs_(config.maxRequestsThresholdPeriodMs),
	delayAfterThresholdReachedMs_(config.delayAfterThresholdReachedMs),
	retryRequestsDelayMs_(config.retryRequestsDelayMs) {}


void RequestFrequencyGuard::handleDelays(int64_t currentTimestamp) {
	msgTimestamps_.insert(msgTimestamps_.begin(), currentTimestamp);

	if(!thresholdReached_ && isOverThreshold()) {
		thresholdReached_ = true;
		msgTimestamps_.clear();
		std::this_thread::sleep_for(delayAfterThresholdReachedMs_);
		return;
	}

	if(thresholdReached_) {
		std::this_thread::sleep_for(retryRequestsDelayMs_);

		if(msgTimestamps_.size() >= maxRequestsThresholdCount_) {
			thresholdReached_ = false;
			msgTimestamps_.pop_back();
		}
	}
}


bool RequestFrequencyGuard::isOverThreshold() {
	bool retVal = false;

	if(msgTimestamps_.size() >= maxRequestsThresholdCount_) {
		if((msgTimestamps_.front() - msgTimestamps_.back()) < maxRequestsThresholdPeriodMs_.count()) {
			retVal = true;
			Logger::logWarning("Http api request frequency threshold reached, delaying requests");
		}

		msgTimestamps_.pop_back();
	}

	return retVal;
}

}// namespace bringauto::fleet_protocol::http_client