#ifndef UFILTER_PICKER_PROXY_SUBSCRIPTION_MANAGER_HPP
#define UFILTER_PICKER_PROXY_SUBSCRIPTION_MANAGER_HPP
#include <memory>
#include <vector>
#include <optional>
#include <spdlog/logger.h>
namespace UFilterPickerProxy
{
 class SubscriptionManagerOptions;
}
namespace UFilterPickerProxyAPI::V1
{
 class Pick;
}
namespace UFilterPickerProxy
{
/// @class SubscriptionManager subscriptionManager.hpp
/// @brief Manages subscribers to the pick streams.
/// @copyright Ben Baker (University of Utah) distributed under the MIT
///            NO AI license.
class SubscriptionManager
{
public:
    /// @brief Constructor.
    /// @param[in] options  The options that define the subscription manager's
    ///                     behavior. 
    /// @param[in] logger   The logger.
    SubscriptionManager(const SubscriptionManagerOptions &options, 
                        std::shared_ptr<spdlog::logger> logger);

    /// @brief Enqueues the next pick for publication.
    /// @param[in,out] pick   The pick to send to subscribers.
    void enqueue(UFilterPickerProxyAPI::V1::Pick &&pick);

    /// @brief Subscribes the context to the pick stream.
    /// @param[in] contextAddress  The memory address of the context that is
    ///                            subscribing.
    void subscribe(uintptr_t contextAddress);
    /// @brief Unsubscribes the context from the pick stream.
    /// @param[in] contextAddress  The memory address of the context that is
    ///                            unsubscribing.
    void unsubscribe(uintptr_t contextAddress);

    /// @brief Fetches the latest pick to publish.
    /// @param contextAddress  The subscribing context address.
    /// @return The latest picks to send to the subscriber.
    [[nodiscard]] std::vector<UFilterPickerProxyAPI::V1::Pick> getPicks(uintptr_t contextAddress) const;
    /// @brief Allows the subscribing thread to get all picks since a certain time.
    /// @param contextAddress  The subscribing conext address.
    /// @param[in] time        Picks including and greater than this time will be
    ///                        returned to the subscriber.
    /// @return All picks that have been collected since the given time. 
    /// @note This is useful on a startup to accomplish backfill.
    [[nodiscard]] std::vector<UFilterPickerProxyAPI::V1::Pick> getPicksSince(uintptr_t contextAddress, const std::chrono::nanoseconds &time) const;

    /// @brief  Destructor.
    ~SubscriptionManager();
    SubscriptionManager(const SubscriptionManager &) = delete;
    SubscriptionManager(SubscriptionManager &&) noexcept = delete;
    SubscriptionManager& operator=(const SubscriptionManager &) = delete;
    SubscriptionManager& operator=(SubscriptionManager &&) noexcept = delete;
private:
    class SubscriptionManagerImpl;
    std::unique_ptr<SubscriptionManagerImpl> pImpl;
};
}
#endif