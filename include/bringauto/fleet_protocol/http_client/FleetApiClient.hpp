#pragma once

#include <bringauto-fleet-http-client-generated/api/CarApi.h>
#include <bringauto-fleet-http-client-generated/api/DeviceApi.h>
#include <bringauto-fleet-http-client-generated/api/ModuleApi.h>
#include <bringauto/fleet_protocol/http_client/RequestFrequencyGuard.hpp>
#include <bringauto/fleet_protocol/cxx/DeviceID.hpp>

#include <optional>


namespace bringauto::fleet_protocol::http_client {


class FleetApiClient {
public:
	/**
	 * @brief Enum class for status types
	 */
	enum class StatusType {
		STATUS,
		STATUS_ERROR
	};

	/**
	 * @brief Enum class for return codes
	 */
	enum class ReturnCode {
		// request was successful without any delays
		OK,
		// request failed
		FAILED,
		// request was delayed due to request rate threshold being reached
		DELAYED,
		// invalid arguments were passed to the function
		INVALID_ARGUMENTS
	};

	/**
	 * @brief Configuration struct for the FleetApiClient
	 */
	struct FleetApiClientConfig {
		/// URL of Fleet v2 HTTP API
		const std::string apiUrl;
		/// api key used for authorization
		const std::string apiKey;
		/// company identifier in the API
		const std::string companyName;
		/// car identifier in the API
		const std::string carName;
	};

	/**
	 * @brief Constructs the fleet API client
	 * @param facConfig struct containing the configuration for the fleet api client
	 * @param rfgConfig struct containing the configuration for the request frequency guard (used in getCommands/getStatuses)
	 */
	FleetApiClient(const FleetApiClientConfig &facConfig, const RequestFrequencyGuard::RequestFrequencyGuardConfig &rfgConfig);

	~FleetApiClient() = default;

	/**
	 * @brief Sets the DeviceId parameters, required for sendCommand and sendStatus functions
	 * @param deviceId DeviceId struct containing the device identification parameters
	 */
	void setDeviceIdentification(const cxx::DeviceID &deviceId) const;

	/**
	 * @brief Calls the GET function on /cars of Fleet v2 HTTP API
	 * @param since optional, minimal timestamp of cars to look for
	 * @param wait optional, if true, waits for a predefined period until any car is found, will check request rate and possibly delay them
	 * @return Vector of shared pointers to the Car model and a return code
	 */
	[[nodiscard]] std::pair<std::vector<std::shared_ptr<org::openapitools::client::model::Car>>, ReturnCode> getCars(
		std::optional<int64_t> since = std::nullopt,
		std::optional<bool> wait = std::nullopt) const;

	/**
	 * @brief Calls the GET function on /command/{company_name}/{car_name} of Fleet v2 HTTP API
	 * @param since optional, minimal timestamp of commands to look for
	 * @param wait optional, if true, waits for a predefined period until any command is found, will check request rate and possibly delay them
	 * @return Vector of shared pointers to the Message model containing commands in payload data and a return code
	 */
	[[nodiscard]] std::pair<std::vector<std::shared_ptr<org::openapitools::client::model::Message>>, ReturnCode> getCommands(
		std::optional<int64_t> since = std::nullopt,
		std::optional<bool> wait = std::nullopt) const;

	/**
	 * @brief Calls the GET function on /status/{company_name}/{car_name} of Fleet v2 HTTP API
	 * @param since optional, minimal timestamp of statuses to look for
	 * @param wait optional, if true, waits for a predefined period until any status is found, will check request rate and possibly delay them
	 * @return Vector of shared pointers to the Message model containing statuses in payload data and a return code
	 */
	[[nodiscard]] std::pair<std::vector<std::shared_ptr<org::openapitools::client::model::Message>>, ReturnCode> getStatuses(
		std::optional<int64_t> since = std::nullopt,
		const std::optional<bool>& wait = std::nullopt) const;

	/**
	 * @brief Calls the POST function on /command/{company_name}/{car_name} of Fleet v2 HTTP API.
	 * setDeviceIdentification needs to be used beforehand to set DeviceId, otherwise placeholder values will be used.
	 * @param commandJson payload data of a command represented by a json as a string
	 * @return ReturnCode indicating the result of the operation
	 */
	ReturnCode sendCommand(const std::string &commandJson) const;

	/**
	 * @brief Calls the POST function on /status/{company_name}/{car_name} of Fleet v2 HTTP API.
	 * setDeviceIdentification needs to be used beforehand to set DeviceId, otherwise placeholder values will be used.
	 * @param statusJson payload data of a status represented by a json as a string
	 * @param statusType optional, type of the status, default is STATUS
	 * @return ReturnCode indicating the result of the operation
	 */
	ReturnCode sendStatus(const std::string &statusJson, StatusType statusType = StatusType::STATUS) const;

	/**
	 * @brief Calls the GET function on /available-devices/{company_name}/{car_name} of Fleet v2 HTTP API
	 * @param moduleId optional, filters returned devices to only those with matching module Id
	 * @return Shared pointer to the AvailableDevices model and a ReturnCode indicating the result
	 */
	[[nodiscard]] std::pair<std::shared_ptr<org::openapitools::client::model::AvailableDevices>, ReturnCode> getAvailableDevices(
		std::optional<int32_t> moduleId = std::nullopt) const;

private:
	std::unique_ptr<RequestFrequencyGuard> requestFrequencyGuard_ {};

	std::shared_ptr<org::openapitools::client::api::ApiClient> apiClientPtr_ {};
	std::unique_ptr<org::openapitools::client::api::CarApi> carApi_ {};
	std::unique_ptr<org::openapitools::client::api::DeviceApi> deviceApi_ {};
	std::unique_ptr<org::openapitools::client::api::ModuleApi> moduleApi_ {};
	std::string companyName_ {};
	std::string carName_ {};

	std::shared_ptr<org::openapitools::client::model::DeviceId> deviceIdPtr_ {};
	std::shared_ptr<org::openapitools::client::model::Message> messagePtr_ {};
	std::shared_ptr<org::openapitools::client::model::Payload> payloadPtr_ {};
	std::shared_ptr<org::openapitools::client::model::Payload_data> payloadDataPtr_ {};
};

}// namespace bringauto::fleet_protocol::http_client
