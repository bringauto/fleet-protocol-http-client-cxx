#pragma once

#include <string>


namespace bringauto::fleet_protocol::http_client::settings {

/**
 * @brief String constants used in the FleetApiClient 
 */
class Constants {
public:
	inline static const std::string API_KEY_HEADER_KEY {"api_key"};
	inline static const std::string DEFAULT_DEVICE_ROLE {"none"};
	inline static const std::string DEFAULT_DEVICE_NAME {"none"};
	inline static const std::string PAYLOAD_ENCODING {"JSON"};
	inline static const std::string COMMAND_MESSAGE_TYPE {"COMMAND"};
	inline static const std::string STATUS_MESSAGE_TYPE {"STATUS"};
};

}// namespace bringauto::fleet_protocol::http_client::settings