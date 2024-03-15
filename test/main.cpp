#include <bringauto/logging/ConsoleSink.hpp>
#include <bringauto/logging/Logger.hpp>

#include <gtest/gtest.h>


using namespace bringauto::logging;

int main(int argc, char **argv) {
	Logger::addSink<ConsoleSink>();
	Logger::LoggerSettings params {
		"FleetHttpClientTests",
		Logger::Verbosity::Debug
	};
	Logger::init(params);

	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}