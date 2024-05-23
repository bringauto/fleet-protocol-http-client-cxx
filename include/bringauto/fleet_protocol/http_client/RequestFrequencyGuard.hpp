#pragma once

#include <chrono>
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
		const int32_t maxRequestsThresholdCount {3};
		/// time period in which the amount of requests is checked
		const std::chrono::milliseconds maxRequestsThresholdPeriodMs {1000};
		/// time to sleep for in ms when threshold is initially reached
		const std::chrono::milliseconds delayAfterThresholdReachedMs {500};
		/// delay in ms between requests until request rate is no longer over the threshold
		const std::chrono::milliseconds retryRequestsDelayMs {220};
	};

	/**
	 * @brief Constructs the request frequency guard
	 * @param config struct containing the configuration for the request frequency guard
	 */
	RequestFrequencyGuard(const RequestFrequencyGuardConfig &config);

	~RequestFrequencyGuard() = default;

	/**
	 * @brief Determines how long the next request should be delayed when the max request rate is reached
	 * Saves the current timestamp and calculates the delay based on timestamps of previous requests
	 */
	void handleDelays();

private:
	uint32_t maxRequestsThresholdCount_ {};
	std::chrono::milliseconds maxRequestsThresholdPeriodMs_ {};
	std::chrono::milliseconds delayAfterThresholdReachedMs_ {};
	std::chrono::milliseconds retryRequestsDelayMs_ {};

	bool thresholdReached_ {false};
	std::vector<int64_t> msgTimestamps_ {};

	/**
	 * @brief Checks if the current request rate is over the threshold
	 * @return true if there are more requests than allowed within the set period
	 */
	bool isOverThreshold();
};

}// namespace bringauto::fleet_protocol::http_client