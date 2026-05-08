#include <cstdint>
#include <queue>
#include <deque>
#include <utility>
#include <map>
#include <algorithm>
#include <chrono>
#include <stdexcept>
#include <spdlog/spdlog.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/stdout_color_sinks.h> //NOLINT
#include <mutex>
#include "uFilterPickerMessageStore/pickStore.hpp"
#include "uFilterPickerMessageStore/pickStoreOptions.hpp"
#include "uFilterPickerProxyAPI/v1/pick.pb.h"

using namespace UFilterPickerProxy;

class PickStore::PickStoreImpl
{
public:
    PickStoreImpl(const PickStoreOptions &options,
                  std::shared_ptr<spdlog::logger> logger) :
        mOptions(options),
        mLogger(std::move(logger))
    {
        if (mLogger == nullptr)
        {
            // NOLINTBEGIN(misc-include-cleaner)
            auto classId
                = std::to_string (reinterpret_cast<std::uintptr_t> (this));
            mLogger = spdlog::stdout_color_mt("PickStoreConsole-"
                                            + classId);
            // NOLINTEND(misc-include-cleaner)
        }
    }
    ~PickStoreImpl()
    {
        unsubscribeAll();
    }
    /// Purge all subscribers
    void unsubscribeAll()
    {

    }

    /// Add a subscriber (if it doesn't exist)
    void subscribe(const uintptr_t contextAddress)
    {
        std::lock_guard lock(mMutex);
        if (!mSubscriberPickMap.contains(contextAddress))
        {
            auto newQueue = std::queue<UFilterPickerProxyAPI::V1::Pick> ();
            auto newElement = std::make_pair(contextAddress, std::move(newQueue));
            mSubscriberPickMap.insert( std::move(newElement) );
        }
    }

    /// Clean the pick deque
    void purgeOldPicks()
    {
        const auto now
            = std::chrono::high_resolution_clock::now().time_since_epoch();
        const std::chrono::nanoseconds oldestTime = now - mMaxHistory;
        std::ranges::remove_if(mPickDeque, [&](const auto &p)
                               {
                                   return p.first < oldestTime;
                               });
    }

    /// Add a pick
    void enqueue(const std::chrono::nanoseconds &timeReceived,
                 UFilterPickerProxyAPI::V1::Pick &&pick)
    {
        const auto now
            = std::chrono::high_resolution_clock::now().time_since_epoch();
        if (timeReceived > now)
        {
            throw std::invalid_argument("Cannot enqueue pick from future");
        }
        const std::chrono::nanoseconds oldestTime = now - mMaxHistory;
        if (timeReceived < oldestTime)
        {
            SPDLOG_LOGGER_DEBUG(mLogger, "Cannot enqueue expired pick");
            return;
        }
        {
        const std::lock_guard lock(mMutex);
        // A newly received pick is most common case (and it is sorted)
        if (timeReceived > mPickDeque.back().first)
        {
            mPickDeque.push_front( std::pair{timeReceived, std::move(pick)} );
            return;
        }
        // Weird but easy
        if (timeReceived < mPickDeque.front().first)
        {
            mPickDeque.push_front( std::pair{timeReceived, std::move(pick)} );
            return;
        }
        // General case is bad news
        mPickDeque.push_front( std::pair{timeReceived, std::move(pick)} );
        std::ranges::sort(mPickDeque, [](const auto &lhs, const auto &rhs)
                          {
                              return lhs.first < rhs.first;
                          });
        } // End lock
    }

//private:
    PickStoreOptions mOptions;
    std::shared_ptr<spdlog::logger> mLogger{nullptr};
    std::map<uintptr_t, std::queue<UFilterPickerProxyAPI::V1::Pick>> mSubscriberPickMap;
    std::deque
    <
        std::pair
        <
            std::chrono::nanoseconds,
            UFilterPickerProxyAPI::V1::Pick
        >
    > mPickDeque;
    std::mutex mMutex;
    std::chrono::nanoseconds mMaxHistory{std::chrono::minutes {15}};
};

/// @brief Destructor.
PickStore::~PickStore() = default;
