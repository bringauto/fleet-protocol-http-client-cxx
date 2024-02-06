#include <RequestFrequencyGuardTests.hpp>

#include <chrono>
#include <stdio.h>


int64_t getCurrentTime() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}


TEST_F(RequestFrequencyGuardTests_F, ThresholdNotReached) {
	int64_t currentTimestamp = 0;
	auto timeBefore = getCurrentTime();

	// Do 20 calls that should not trigger the threshold
	std::cout << "Expecting no delay" << std::endl;
	for(int i = 0; i < 20; i++) {
		requestFrequencyGuard->handleDelays(currentTimestamp);
		currentTimestamp += 5;
	}

	auto timeAfter = getCurrentTime();
	ASSERT_LT(timeAfter - timeBefore, 100);
}


TEST_F(RequestFrequencyGuardTests_F, ThresholdReached) {
	int64_t currentTimestamp = 0;
	auto timeBefore = getCurrentTime();

	// Do 5 calls with a small timestamp difference that should trigger the threshold
	std::cout << "Expecting 5s delay" << std::endl;
	for(int i = 0; i < 5; i++) {
		requestFrequencyGuard->handleDelays(currentTimestamp);
		currentTimestamp += 1;
	}

	auto timeAfter = getCurrentTime();
	ASSERT_GE(timeAfter - timeBefore, 5000);
	ASSERT_LT(timeAfter - timeBefore, 5100);

	timeBefore = getCurrentTime();

	// The next call should be delayed by 200ms
	std::cout << "Expecting 200ms delay" << std::endl;
	requestFrequencyGuard->handleDelays(currentTimestamp);

	timeAfter = getCurrentTime();
	ASSERT_GE(timeAfter - timeBefore, 200);
	ASSERT_LT(timeAfter - timeBefore, 300);
}


TEST_F(RequestFrequencyGuardTests_F, DelayResetAfterThresholdReached) {
	int64_t currentTimestamp = 0;
	auto timeBefore = getCurrentTime();

	// Do 5 calls with a small timestamp difference that should trigger the threshold
	std::cout << "Expecting 5s delay" << std::endl;
	for(int i = 0; i < 5; i++) {
		requestFrequencyGuard->handleDelays(currentTimestamp);
		currentTimestamp += 1;
	}

	auto timeAfter = getCurrentTime();
	ASSERT_GE(timeAfter - timeBefore, 5000);
	ASSERT_LT(timeAfter - timeBefore, 5100);

	timeBefore = getCurrentTime();

	// The next 5 calls should be delayed by 200ms
	for(int i = 0; i < 5; i++) {
		std::cout << "Expecting 200ms delay" << std::endl;
		requestFrequencyGuard->handleDelays(currentTimestamp);
		currentTimestamp += 5;
	}

	timeAfter = getCurrentTime();
	ASSERT_GE(timeAfter - timeBefore, 1000);
	ASSERT_LT(timeAfter - timeBefore, 1100);

	timeBefore = getCurrentTime();

	// Subsequent calls should not be delayed since the threshold was reset
	std::cout << "Expecting no delay" << std::endl;
	for(int i = 0; i < 5; i++) {
		requestFrequencyGuard->handleDelays(currentTimestamp);
		currentTimestamp += 5;
	}

	timeAfter = getCurrentTime();
	ASSERT_LT(timeAfter - timeBefore, 100);
}


TEST_F(RequestFrequencyGuardTests_F, DelayNotResetAfterThresholdReached) {
	int64_t currentTimestamp = 0;
	auto timeBefore = getCurrentTime();

	// Do 5 calls with a small timestamp difference that should trigger the threshold
	std::cout << "Expecting 5s delay" << std::endl;
	for(int i = 0; i < 5; i++) {
		requestFrequencyGuard->handleDelays(currentTimestamp);
		currentTimestamp += 1;
	}

	auto timeAfter = getCurrentTime();
	ASSERT_GE(timeAfter - timeBefore, 5000);
	ASSERT_LT(timeAfter - timeBefore, 5100);

	timeBefore = getCurrentTime();

	// The next 5 calls should be delayed by 200ms
	for(int i = 0; i < 5; i++) {
		std::cout << "Expecting 200ms delay" << std::endl;
		requestFrequencyGuard->handleDelays(currentTimestamp);
		currentTimestamp += 1;
	}

	timeAfter = getCurrentTime();
	ASSERT_GE(timeAfter - timeBefore, 1000);
	ASSERT_LT(timeAfter - timeBefore, 1100);

	timeBefore = getCurrentTime();

	// The next call should be delayed by 5s since the threshold was not reset
	std::cout << "Expecting 5s delay" << std::endl;
	requestFrequencyGuard->handleDelays(currentTimestamp);

	timeAfter = getCurrentTime();
	ASSERT_GE(timeAfter - timeBefore, 5000);
	ASSERT_LT(timeAfter - timeBefore, 5100);
}