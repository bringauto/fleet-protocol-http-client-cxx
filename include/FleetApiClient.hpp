class FleetApiClient
{
public:
    FleetApiClient(std::string apiUrl, std::string apiKey);
    ~FleetApiClient();

    std::vector<std::shared_ptr<Car>> getCars();
    std::vector<std::shared_ptr<model::Message>> getCommands(std::string companyName, std::string carName, long since = 0, bool wait = false);
    std::vector<std::shared_ptr<model::Message>> getStatuses(std::string companyName, std::string carName, long since = 0, bool wait = false);
    //TODO decide how to input json
    void sendCommands(std::string companyName,
                      std::string carName,
                      int moduleId,
                      int deviceType,
                      std::string deviceRole,
                      std::string deviceName,
                      std::string commandJson);
    void sendStatuses(std::string companyName,
                      std::string carName,
                      int moduleId,
                      int deviceType,
                      std::string deviceRole,
                      std::string deviceName,
                      std::string statusJson);
    std::shared_ptr<AvailableDevices> getAvailableDevices(std::string companyName, std::string carName, std::optional<int> moduleId);

private:
    void handleRequestFrequency();

    std::shared_ptr<api::ApiClient> apiClientPtr;
    api::CarApi carApi;
    api::DeviceApi deviceApi;
    api::ModuleApi moduleApi;
}