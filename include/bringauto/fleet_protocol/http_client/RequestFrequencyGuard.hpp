#pragma once

#include <cstdint>
#include <vector>


namespace bringauto::fleet_protocol::http_client {


class RequestFrequencyGuard {
public:
	/**
	 * @brief Configuration struct for the RequestFrequencyGuard
	 */
	struct RequestFrequencyGuardConfig {
		/// max amount of allowed requests in the specified time period
		int32_t maxRequestsThresholdCount {3};
		/// time period in which the amount of requests is checked
		int32_t maxRequestsThresholdPeriodMs {1000};
		/// time to sleep for in ms when threshold is initially reached
		int32_t delayAfterThresholdReachedMs {500};
		/// delay in ms between requests until request rate is no longer over the threshold
		int32_t retryRequestsDelayMs {220};
	};

	/**
     * @brief Constructs the request frequency guard
	 * @param config struct containing the configuration for the request frequency guard
     */
	RequestFrequencyGuard(const RequestFrequencyGuardConfig &config);

	~RequestFrequencyGuard() = default;

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

	bool thresholdReached_ {false};
	std::vector<int64_t> msgTimestamps_;

	/**
     * @brief Checks if the current request rate is over the threshold
     * @return true if there are more requests than allowed within the set period
     */
	bool isOverThreshold();
};

}// namespace bringauto::fleet_protocol::http_client