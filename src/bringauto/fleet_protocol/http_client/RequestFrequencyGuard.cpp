#include <bringauto/fleet_protocol/http_client/RequestFrequencyGuard.hpp>

#include <chrono>
#include <thread>


namespace bringauto::fleet_protocol::http_client {


RequestFrequencyGuard::RequestFrequencyGuard(const RequestFrequencyGuardConfig &config):
	maxRequestsThresholdCount_(config.maxRequestsThresholdCount),
	maxRequestsThresholdPeriodMs_(config.maxRequestsThresholdPeriodMs),
	delayAfterThresholdReachedMs_(config.delayAfterThresholdReachedMs),
	retryRequestsDelayMs_(config.retryRequestsDelayMs) {}


bool RequestFrequencyGuard::handleDelays() {
	auto currentTimestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch()).count();
	msgTimestamps_.insert(msgTimestamps_.begin(), currentTimestamp);

	if(!thresholdReached_ && isOverThreshold()) {
		thresholdReached_ = true;
		msgTimestamps_.clear();
		std::this_thread::sleep_for(delayAfterThresholdReachedMs_);
		return true;
	}

	if(thresholdReached_) {
		std::this_thread::sleep_for(retryRequestsDelayMs_);

		if(msgTimestamps_.size() >= maxRequestsThresholdCount_) {
			thresholdReached_ = false;
			msgTimestamps_.pop_back();
		}
	}
	return false;
}


bool RequestFrequencyGuard::isOverThreshold() {
	bool retVal = false;

	if(msgTimestamps_.size() >= maxRequestsThresholdCount_) {
		if((msgTimestamps_.front() - msgTimestamps_.back()) < maxRequestsThresholdPeriodMs_.count()) {
			retVal = true;
		}

		msgTimestamps_.pop_back();
	}

	return retVal;
}

}// namespace bringauto::fleet_protocol::http_client
