#include "RequestFrequencyGuardTests.hpp"

#include <stdio.h>
#include <chrono>


TEST_F(RequestFrequencyGuardTests_F, ThresholdNotReached) {
    int64_t currentTimestamp = 0;
    auto timeBefore = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    std::cout << "Expecting no delay" << std::endl;
    for (int i = 0; i < 20; i++) {
        requestFrequencyGuard->handleDelays(currentTimestamp);
        currentTimestamp += 5;
    }

    auto timeAfter = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    ASSERT_LT(timeAfter - timeBefore, 100);
}


TEST_F(RequestFrequencyGuardTests_F, ThresholdReached) {
    int64_t currentTimestamp = 0;
    auto timeBefore = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    std::cout << "Expecting 5s delay" << std::endl;
    for (int i = 0; i < 5; i++) {
        requestFrequencyGuard->handleDelays(currentTimestamp);
        currentTimestamp += 1;
    }

    auto timeAfter = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    ASSERT_GE(timeAfter - timeBefore, 5000);
    ASSERT_LT(timeAfter - timeBefore, 5100);

    timeBefore = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    std::cout << "Expecting 200ms delay" << std::endl;
    requestFrequencyGuard->handleDelays(currentTimestamp);
    
    timeAfter = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    ASSERT_GE(timeAfter - timeBefore, 200);
    ASSERT_LT(timeAfter - timeBefore, 300);
}


TEST_F(RequestFrequencyGuardTests_F, DelayResetAfterThresholdReached) {
    int64_t currentTimestamp = 0;
    auto timeBefore = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    std::cout << "Expecting 5s delay" << std::endl;
    for (int i = 0; i < 5; i++) {
        requestFrequencyGuard->handleDelays(currentTimestamp);
        currentTimestamp += 1;
    }

    auto timeAfter = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    ASSERT_GE(timeAfter - timeBefore, 5000);
    ASSERT_LT(timeAfter - timeBefore, 5100);

    timeBefore = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    for (int i = 0; i < 5; i++) {
        std::cout << "Expecting 200ms delay" << std::endl;
        requestFrequencyGuard->handleDelays(currentTimestamp);
        currentTimestamp += 5;
    }

    timeAfter = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    ASSERT_GE(timeAfter - timeBefore, 1000);
    ASSERT_LT(timeAfter - timeBefore, 1100);

    timeBefore = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    std::cout << "Expecting no delay" << std::endl;
    for (int i = 0; i < 5; i++) {
        requestFrequencyGuard->handleDelays(currentTimestamp);
        currentTimestamp += 5;
    }

    timeAfter = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    ASSERT_LT(timeAfter - timeBefore, 100);
}


TEST_F(RequestFrequencyGuardTests_F, DelayNotResetAfterThresholdReached) {
    int64_t currentTimestamp = 0;
    auto timeBefore = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    std::cout << "Expecting 5s delay" << std::endl;
    for (int i = 0; i < 5; i++) {
        requestFrequencyGuard->handleDelays(currentTimestamp);
        currentTimestamp += 1;
    }

    auto timeAfter = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    ASSERT_GE(timeAfter - timeBefore, 5000);
    ASSERT_LT(timeAfter - timeBefore, 5100);

    timeBefore = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    for (int i = 0; i < 5; i++) {
        std::cout << "Expecting 200ms delay" << std::endl;
        requestFrequencyGuard->handleDelays(currentTimestamp);
        currentTimestamp += 1;
    }

    timeAfter = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    ASSERT_GE(timeAfter - timeBefore, 1000);
    ASSERT_LT(timeAfter - timeBefore, 1100);

    timeBefore = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    std::cout << "Expecting 5s delay" << std::endl;
    requestFrequencyGuard->handleDelays(currentTimestamp);
    
    timeAfter = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    ASSERT_GE(timeAfter - timeBefore, 5000);
    ASSERT_LT(timeAfter - timeBefore, 5100);
}