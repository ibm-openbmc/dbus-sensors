#pragma once

#include <Thresholds.hpp>
#include <boost/asio/streambuf.hpp>
#include <sdbusplus/asio/object_server.hpp>
#include <sensor.hpp>

#include <string>
#include <vector>

class HwmonTempSensor :
    public Sensor,
    public std::enable_shared_from_this<HwmonTempSensor>
{
  public:
    HwmonTempSensor(const std::string& path, const std::string& objectType,
                    sdbusplus::asio::object_server& objectServer,
                    std::shared_ptr<sdbusplus::asio::connection>& conn,
                    boost::asio::io_service& io, const std::string& fanName,
                    std::vector<thresholds::Threshold>&& thresholds,
                    const float pollRate,
                    const std::string& sensorConfiguration,
                    const PowerState powerState, size_t bus, size_t address);
    ~HwmonTempSensor() override;
    void setupRead(void);

    // Clears the history of failed reads
    static void clearFailedDevices();

    // Create an event log for a failed read
    void createEventLog();

  private:
    sdbusplus::asio::object_server& objServer;
    boost::asio::posix::stream_descriptor inputDev;
    boost::asio::deadline_timer waitTimer;
    boost::asio::streambuf readBuf;
    std::string path;
    unsigned int sensorPollMs;
    size_t bus;
    size_t address;

    // pair<bus, address> of devices with logged failed reads
    static std::vector<std::pair<size_t, size_t>> failedDevices;

    // The error code from the last HW access for the sensor.
    boost::system::error_code errorCode;

    void handleResponse(const boost::system::error_code& err);
    void checkThresholds(void) override;
};
