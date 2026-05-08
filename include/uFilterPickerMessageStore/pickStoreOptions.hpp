#ifndef UFILTER_PICKER_PROXY_SUBSCRIPTION_MANAGER_OPTIONS_HPP
#define UFILTER_PICKER_PROXY_SUBSCRIPTION_MANAGER_OPTIONS_HPP
#include <memory>
namespace UFilterPickerProxy
{
/// @class SubscriptionManagerOptions subscriptionManagerOptions.hpp
/// @brief Defines the behavior of the subscription manager.
/// @copyright Ben Baker (University of Utah) distributed under the
///            MIT NO AI license. 
class SubscriptionManagerOptions
{
public:
    /// @brief Constructor.
    SubscriptionManagerOptions();
    /// @brief Copy constructor.
    SubscriptionManagerOptions(const SubscriptionManagerOptions &options);
    /// @brief Move constructor.
    SubscriptionManagerOptions(SubscriptionManagerOptions &&options) noexcept;

    /// @brief Sets the maximum message queue size.
    /// @param[in] maximumQueueSize  The maximum number of pick messages that
    ///                              can be stored in the queue.
    void setMaximumQueueSize(int maximumQueueSize);
    /// @return The maximum queue size.  
    [[nodiscard]] int getMaximumQueueSize() const noexcept;

    /// @brief Destructor.
    ~SubscriptionManagerOptions();
    /// @brief Copy assignment.
    SubscriptionManagerOptions& operator=(const SubscriptionManagerOptions &options);
    /// @brief Move assignment.
    SubscriptionManagerOptions& operator=(SubscriptionManagerOptions &&options) noexcept;
private:
    class SubscriptionManagerOptionsImpl;
    std::unique_ptr<SubscriptionManagerOptionsImpl> pImpl;
};
}
#endif