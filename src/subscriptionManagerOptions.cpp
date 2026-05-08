#include <utility>
#include <memory>
#include <stdexcept>
#include "uFilterPickerProxy/subscriptionManagerOptions.hpp"

using namespace UFilterPickerProxy;

class SubscriptionManagerOptions::SubscriptionManagerOptionsImpl
{
public:
    int mMaximumQueueSize{2048};
};

/// Constructor
SubscriptionManagerOptions::SubscriptionManagerOptions() :
    pImpl(std::make_unique<SubscriptionManagerOptionsImpl> ()) 
{
}

/// Copy constructor
SubscriptionManagerOptions::SubscriptionManagerOptions(
    const SubscriptionManagerOptions &options)
{
    *this = options;
}

/// Copy constructor
SubscriptionManagerOptions::SubscriptionManagerOptions(
    SubscriptionManagerOptions &&options) noexcept
{
    *this = std::move(options);
}

/// Copy assignment
SubscriptionManagerOptions &SubscriptionManagerOptions::operator=(
    const SubscriptionManagerOptions &options)
{
    if (&options == this){return *this;}
    pImpl = std::make_unique<SubscriptionManagerOptionsImpl> (*options.pImpl);
    return *this;
}

/// Move assignment
SubscriptionManagerOptions &SubscriptionManagerOptions::operator=(
    SubscriptionManagerOptions &&options) noexcept
{
    if (&options == this){return *this;}
    pImpl = std::move (options.pImpl);
    return *this;
}

/// Destructor
SubscriptionManagerOptions::~SubscriptionManagerOptions() = default;

/// Maximum queue size
void SubscriptionManagerOptions::setMaximumQueueSize(const int maxSize)
{
    if (maxSize < 1)
    {
        throw std::invalid_argument("Maximum queue size must be positive");
    }
    pImpl->mMaximumQueueSize = maxSize;
}

int SubscriptionManagerOptions::getMaximumQueueSize() const noexcept
{
    return pImpl->mMaximumQueueSize;
}