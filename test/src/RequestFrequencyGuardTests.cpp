#include <RequestFrequencyGuardTests.hpp>

#include <chrono>
#include <cstdio>
#include <thread>


int64_t getCurrentTime() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}


/**
 * @brief Try sending MAX_REQUEST_THRESHOLD_COUNT * 2 requests with time difference not large enough to trigger the threshold
 */
TEST_F(RequestFrequencyGuardTests_F, ThresholdNotReached) {
	const auto timeBefore = getCurrentTime();

	// Do MAX_REQUEST_THRESHOLD_COUNT * 2 calls that should not trigger the threshold
	std::cout << "Expecting " << (MAX_REQUEST_THRESHOLD_PERIOD_MS / 2) * (MAX_REQUEST_THRESHOLD_COUNT * 2) << " ms delay" << std::endl;
	for(int i = 0; i < MAX_REQUEST_THRESHOLD_COUNT * 2; i++) {
		ASSERT_FALSE(requestFrequencyGuard->handleDelays());
		std::this_thread::sleep_for(std::chrono::milliseconds(MAX_REQUEST_THRESHOLD_PERIOD_MS / 2));
	}

	const auto timeAfter = getCurrentTime();
	ASSERT_LT(timeAfter - timeBefore,
		MAX_ALLOWED_NO_DELAY_TIME_DIFFERENCE_MS +
		(MAX_REQUEST_THRESHOLD_PERIOD_MS / 2) * (MAX_REQUEST_THRESHOLD_COUNT * 2));
}


/**
 * @brief Try sending MAX_REQUEST_THRESHOLD_COUNT requests with time difference small enough to trigger the threshold;
 * next request should be delayed by shorter time
 */
TEST_F(RequestFrequencyGuardTests_F, ThresholdReached) {
	auto timeBefore = getCurrentTime();

	// Do MAX_REQUEST_THRESHOLD_COUNT calls with a small timestamp difference that should trigger the threshold
	std::cout << "Expecting " << DELAY_AFTER_THRESHOLD_REACHED_MS << "ms delay" << std::endl;
	for(int i = 0; i < MAX_REQUEST_THRESHOLD_COUNT; i++) {
		requestFrequencyGuard->handleDelays();
	}

	auto timeAfter = getCurrentTime();
	ASSERT_GE(timeAfter - timeBefore, DELAY_AFTER_THRESHOLD_REACHED_MS);
	ASSERT_LT(timeAfter - timeBefore, MAX_DELAY_AFTER_THRESHOLD_REACHED_MS);

	timeBefore = getCurrentTime();

	// The next call should be delayed by RETRY_REQUESTS_DELAY_MS
	std::cout << "Expecting " << RETRY_REQUESTS_DELAY_MS << "ms delay" << std::endl;
	ASSERT_FALSE(requestFrequencyGuard->handleDelays());

	timeAfter = getCurrentTime();
	ASSERT_GE(timeAfter - timeBefore, RETRY_REQUESTS_DELAY_MS);
	ASSERT_LT(timeAfter - timeBefore, MAX_RETRY_REQUESTS_DELAY_MS);
}


/**
 * @brief Try sending MAX_REQUEST_THRESHOLD_COUNT requests with time difference small enough to trigger the threshold;
 * next MAX_REQUEST_THRESHOLD_COUNT requests should be delayed by shorter time;
 * their timestamp difference is large enough to not trigger the threshold afterward
 */
TEST_F(RequestFrequencyGuardTests_F, DelayResetAfterThresholdReached) {
	auto timeBefore = getCurrentTime();

	// Do MAX_REQUEST_THRESHOLD_COUNT calls with a small timestamp difference that should trigger the threshold
	std::cout << "Expecting " << DELAY_AFTER_THRESHOLD_REACHED_MS << "ms delay" << std::endl;
	for(int i = 0; i < MAX_REQUEST_THRESHOLD_COUNT; i++) {
		requestFrequencyGuard->handleDelays();
	}

	auto timeAfter = getCurrentTime();
	ASSERT_GE(timeAfter - timeBefore, DELAY_AFTER_THRESHOLD_REACHED_MS);
	ASSERT_LT(timeAfter - timeBefore, MAX_DELAY_AFTER_THRESHOLD_REACHED_MS);

	timeBefore = getCurrentTime();

	// The next MAX_REQUEST_THRESHOLD_COUNT calls should be delayed by RETRY_REQUESTS_DELAY_MS
	for(int i = 0; i < MAX_REQUEST_THRESHOLD_COUNT; i++) {
		std::cout << "Expecting " << RETRY_REQUESTS_DELAY_MS + (MAX_REQUEST_THRESHOLD_PERIOD_MS / 2) << "ms delay" << std::endl;
		ASSERT_FALSE(requestFrequencyGuard->handleDelays());
		std::this_thread::sleep_for(std::chrono::milliseconds(MAX_REQUEST_THRESHOLD_PERIOD_MS / 2));
	}

	timeAfter = getCurrentTime();
	ASSERT_GE(timeAfter - timeBefore,
		MAX_REQUEST_THRESHOLD_COUNT *
		(RETRY_REQUESTS_DELAY_MS + (MAX_REQUEST_THRESHOLD_PERIOD_MS / 2)));
	ASSERT_LT(timeAfter - timeBefore,
		MAX_REQUEST_THRESHOLD_COUNT *
		(MAX_RETRY_REQUESTS_DELAY_MS + (MAX_REQUEST_THRESHOLD_PERIOD_MS / 2)));

	timeBefore = getCurrentTime();

	// Subsequent calls should not be delayed since the threshold was reset
	std::cout << "Expecting " << (MAX_REQUEST_THRESHOLD_PERIOD_MS / 2) * MAX_REQUEST_THRESHOLD_COUNT << " ms delay" << std::endl;
	for(int i = 0; i < MAX_REQUEST_THRESHOLD_COUNT; i++) {
		ASSERT_FALSE(requestFrequencyGuard->handleDelays());
		std::this_thread::sleep_for(std::chrono::milliseconds(MAX_REQUEST_THRESHOLD_PERIOD_MS / 2));
	}

	timeAfter = getCurrentTime();
	ASSERT_LT(timeAfter - timeBefore,
		MAX_ALLOWED_NO_DELAY_TIME_DIFFERENCE_MS +
		(MAX_REQUEST_THRESHOLD_PERIOD_MS / 2) * MAX_REQUEST_THRESHOLD_COUNT);
}


/**
 * @brief Try sending MAX_REQUEST_THRESHOLD_COUNT requests with time difference small enough to trigger the threshold;
 * next MAX_REQUEST_THRESHOLD_COUNT requests should be delayed by shorter time;
 * their timestamp difference is small enough to trigger the threshold afterward
 */
TEST_F(RequestFrequencyGuardTests_F, DelayNotResetAfterThresholdReached) {
	auto timeBefore = getCurrentTime();

	// Do MAX_REQUEST_THRESHOLD_COUNT calls with a small timestamp difference that should trigger the threshold
	std::cout << "Expecting " << DELAY_AFTER_THRESHOLD_REACHED_MS << "ms delay" << std::endl;
	for(int i = 0; i < MAX_REQUEST_THRESHOLD_COUNT; i++) {
		requestFrequencyGuard->handleDelays();
	}

	auto timeAfter = getCurrentTime();
	ASSERT_GE(timeAfter - timeBefore, DELAY_AFTER_THRESHOLD_REACHED_MS);
	ASSERT_LT(timeAfter - timeBefore, MAX_DELAY_AFTER_THRESHOLD_REACHED_MS);

	timeBefore = getCurrentTime();

	// The next MAX_REQUEST_THRESHOLD_COUNT calls should be delayed by short delay
	for(int i = 0; i < MAX_REQUEST_THRESHOLD_COUNT; i++) {
		std::cout << "Expecting " << RETRY_REQUESTS_DELAY_MS << "ms delay" << std::endl;
		ASSERT_FALSE(requestFrequencyGuard->handleDelays());
	}

	timeAfter = getCurrentTime();
	ASSERT_GE(timeAfter - timeBefore, MAX_REQUEST_THRESHOLD_COUNT * RETRY_REQUESTS_DELAY_MS);
	ASSERT_LT(timeAfter - timeBefore, MAX_REQUEST_THRESHOLD_COUNT * MAX_RETRY_REQUESTS_DELAY_MS);

	timeBefore = getCurrentTime();

	// The next call should be delayed by long delay since the threshold was not reset
	std::cout << "Expecting " << DELAY_AFTER_THRESHOLD_REACHED_MS << "ms delay" << std::endl;
	ASSERT_TRUE(requestFrequencyGuard->handleDelays());

	timeAfter = getCurrentTime();
	ASSERT_GE(timeAfter - timeBefore, DELAY_AFTER_THRESHOLD_REACHED_MS);
	ASSERT_LT(timeAfter - timeBefore, MAX_DELAY_AFTER_THRESHOLD_REACHED_MS);
}
