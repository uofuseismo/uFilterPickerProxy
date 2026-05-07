#include <cstdint>
#include <cstddef>
#include <cctype>
#include <atomic>
#include <thread>
#include <utility>
#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <chrono>
#include <queue>
#include <mutex>
#include <algorithm>
#include <ranges>
#include <exception>
#include <stdexcept>
#include <google/protobuf/util/time_util.h>
#include <spdlog/spdlog.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h> //NOLINT
#include "uFilterPickerProxy/proxy.hpp"
#include "uFilterPickerProxy/proxyOptions.hpp"
#include "uFilterPickerProxy/database.hpp"
#include "uFilterPickerProxy/frontend.hpp"
#include "uFilterPickerProxy/frontendOptions.hpp"
#include "uFilterPickerProxy/backend.hpp"
#include "uFilterPickerProxy/backendOptions.hpp"
#include "uFilterPickerProxy/metricsSingleton.hpp"
#include "uFilterPickerProxyAPI/v1/pick.pb.h"
#include "uFilterPickerProxyAPI/v1/algorithm.pb.h"
#include "uFilterPickerProxyAPI/v1/stream_identifier.pb.h"

using namespace UFilterPickerProxy;

namespace
{

std::string capitalizeAndRemoveBlanks(const std::string &input)
{
    std::string result{input};
    const auto ret = std::ranges::remove_if(result, ::isspace);
    result.erase(ret.begin(), ret.end());
    std::ranges::transform(result, result.end(), ::toupper);
    return result;
}

UFilterPickerProxyAPI::V1::StreamIdentifier checkAndFixStreamIdentifier(
    UFilterPickerProxyAPI::V1::StreamIdentifier &&identifierIn)
{
    UFilterPickerProxyAPI::V1::StreamIdentifier identifier{std::move(identifierIn)};
    if (!identifier.has_network())
    {
        throw std::invalid_argument("Network not set");
    }
    auto network = ::capitalizeAndRemoveBlanks(identifier.network());
    if (network.empty()){throw std::invalid_argument("Network is empty");}
    identifier.set_network(network);

    if (!identifier.has_station())
    {
        throw std::invalid_argument("Station not set");
    }
    auto station = ::capitalizeAndRemoveBlanks(identifier.station());
    if (station.empty()){throw std::invalid_argument("Station is empty");}
    identifier.set_station(station);

    if (!identifier.has_channel())
    {
        throw std::invalid_argument("Channel not set");
    }
    auto channel = ::capitalizeAndRemoveBlanks(identifier.channel());
    if (channel.empty()){throw std::invalid_argument("Channel is empty");}
    identifier.set_channel(channel);

    if (!identifier.has_location_code())
    {
        identifier.set_location_code("--");
    }
    else
    {
        auto location = capitalizeAndRemoveBlanks(identifier.location_code());
        if (location.empty())
        {
            identifier.set_location_code("--");
        }
        else
        {
            identifier.set_location_code(location);
        }
    }
    return identifier;
}

UFilterPickerProxyAPI::V1::Algorithm checkAndFixAlgorithm(
    UFilterPickerProxyAPI::V1::Algorithm &&algorithmIn)
{
    UFilterPickerProxyAPI::V1::Algorithm algorithm{std::move(algorithmIn)};
    if (!algorithm.has_name())
    {
        algorithm.set_name("uFilterPicker");
    }
    if (!algorithm.has_version())
    {
        throw std::invalid_argument("Algorithm version not set");
    }
    return algorithm;  
}

}

class Proxy::ProxyImpl
{
public:
    ProxyImpl(const ProxyOptions &options,
              std::shared_ptr<spdlog::logger> logger) :
        mOptions(options),
        mLogger(std::move(logger))
    {
        if (!mOptions.hasFrontendOptions())
        {
            throw std::invalid_argument("Frontend options not set on proxy");
        }
        if (!mOptions.hasBackendOptions())
        {
            throw std::invalid_argument("Backend options not set on proxy");
        }
        if (mLogger == nullptr)
        {   
            // NOLINTBEGIN(misc-include-cleaner)
            auto classId
                = std::to_string (reinterpret_cast<std::uintptr_t> (this));
            mLogger = spdlog::stdout_color_mt("ProxyConsole-"
                                            + classId);
            // NOLINTEND(misc-include-cleaner)
        }
        mFrontend 
            = std::make_unique<Frontend> (mOptions.getFrontendOptions(), 
                                          mAddPickCallbackFunction, 
                                          mLogger);
                                        
        std::vector<UFilterPickerProxyAPI::V1::Pick> loadedPicks;
        mBackend 
            = std::make_unique<Backend> (mOptions.getBackendOptions(),
                                         loadedPicks,
                                         mLogger);
    }

    /// Stop the proxy
    void stop()
    {
        mKeepRunning.store(false);
        constexpr std::chrono::milliseconds pause{15};
        // Stop receiving picks first to give subscribers a chance
        if (mFrontend)
        {
            SPDLOG_LOGGER_DEBUG(mLogger, "Stopping frontend");
            mFrontend->stop();
            std::this_thread::sleep_for(pause);
        }
        if (mBackend)
        {
            SPDLOG_LOGGER_DEBUG(mLogger, "Stopping backend");
            mBackend->stop();
            std::this_thread::sleep_for(pause);
        }
        // Now shutdown the database

        mIsRunning.store(false);
    }

    void processPick()
    {
        while (mKeepRunning.load(std::memory_order_relaxed))
        {
            UFilterPickerProxyAPI::V1::Pick pick;
            bool gotPick{false};
            {
            const std::lock_guard lock(mMutex);
            if (!mInputQueue.empty())
            {
                pick = std::move(mInputQueue.front());
                mInputQueue.pop();
                gotPick = true;
            }
            }
            if (gotPick)
            {
                //mDatabase->add(pick);
                //mBackend->enqueue(pick);
            }
            else
            {
                constexpr std::chrono::milliseconds timeOut{25};
                std::this_thread::sleep_for(timeOut);
            }
        }
    }

    void cleanDatabase()
    {
        constexpr std::chrono::seconds cleanEvery{60};
        while (mKeepRunning.load(std::memory_order_relaxed))
        {
            // Set a condition variable
        }
    }

    /// @brief Defines the callback for getting/propagating a pick.
    void addPickCallback(UFilterPickerProxyAPI::V1::Pick &&pick)
    {
        // Check the pick
        if (!pick.has_stream_identifier())
        {
            throw std::invalid_argument("Stream identifier not set");
        }
        if (!pick.has_time())
        {
            throw std::invalid_argument("Pick time not set");
        }
        if (!pick.has_algorithm())
        {
            throw std::invalid_argument("Algorithm not set");
        }
        // Check (and fix capitalization) of our identifier
        auto identifier = pick.stream_identifier();
        auto newIdentifier = ::checkAndFixStreamIdentifier(std::move(identifier));
        *pick.mutable_stream_identifier() = std::move(newIdentifier);
        // Check (and fix name if not present) of algorithm
        auto algorithm = pick.algorithm();
        auto newAlgorithm = ::checkAndFixAlgorithm(std::move(algorithm));
        *pick.mutable_algorithm() = std::move(newAlgorithm);
        // Check that the pick time makes some sense
        const auto pickTime
            = google::protobuf::util::TimeUtil::TimestampToNanoseconds(
                 pick.time());
        const auto now 
            = std::chrono::duration_cast<std::chrono::microseconds>
              ((std::chrono::high_resolution_clock::now()).time_since_epoch());
        if (pickTime > now.count())
        {
            throw std::invalid_argument("Pick cannot be from future");
        }
        // Enqueue the pick
        {
        const std::lock_guard lock{mMutex};
        while (mInputQueue.size() >= mMaximumInputQueueSize)
        {
            SPDLOG_LOGGER_WARN(mLogger, "Popping pick from queue");
            //mMetrics.incrementOverflowPicksCounter();
            mInputQueue.pop();
        }
        mInputQueue.push(std::move(pick));
        }
    }

    /// @result True indicates the proxy is still running
    [[nodiscard]] bool isRunning() const noexcept
    {
        return mIsRunning.load();
    }
private:
//private:
    ProxyOptions mOptions;
    std::shared_ptr<spdlog::logger> mLogger{nullptr};
    std::mutex mMutex;
    std::function<void(UFilterPickerProxyAPI::V1::Pick &&)> 
        mAddPickCallbackFunction
    {    
        std::bind(&ProxyImpl::addPickCallback, this,
                  std::placeholders::_1)
    };   
    std::unique_ptr<Frontend> mFrontend{nullptr};
    std::unique_ptr<Backend> mBackend{nullptr};
    std::queue<UFilterPickerProxyAPI::V1::Pick> mInputQueue;
    size_t mMaximumInputQueueSize{4096};
    std::atomic<bool> mIsRunning{false};
    std::atomic<bool> mKeepRunning{true};
};

/// Constructor

/// Destructor
Proxy::~Proxy() = default;

/// Stop the proxy
void Proxy::stop()
{
    pImpl->stop();
}

/// Is the proxy running?
bool Proxy::isRunning() const noexcept
{
    return pImpl->isRunning();
}
