#include <utility>
#include <memory>
#include <stdexcept>
#include "uFilterPickerPickBroker/pickStoreOptions.hpp"

using namespace UFilterPickerPickBroker;

class PickStoreOptions::PickStoreOptionsImpl
{
public:
    int mMaximumQueueSize{2048};
};

/// Constructor
PickStoreOptions::PickStoreOptions() :
    pImpl(std::make_unique<PickStoreOptionsImpl> ())
{
}

/// Copy constructor
PickStoreOptions::PickStoreOptions(
    const PickStoreOptions &options)
{
    *this = options;
}

/// Move constructor
PickStoreOptions::PickStoreOptions(
    PickStoreOptions &&options) noexcept
{
    *this = std::move(options);
}

/// Copy assignment
PickStoreOptions &PickStoreOptions::operator=(
    const PickStoreOptions &options)
{
    if (&options == this){return *this;}
    pImpl = std::make_unique<PickStoreOptionsImpl> (*options.pImpl);
    return *this;
}

/// Move assignment
PickStoreOptions &PickStoreOptions::operator=(
    PickStoreOptions &&options) noexcept
{
    if (&options == this){return *this;}
    pImpl = std::move (options.pImpl);
    return *this;
}

/// Destructor
PickStoreOptions::~PickStoreOptions() = default;

void PickStoreOptions::setMaximumQueueSize(const int maxSize)
{
    if (maxSize < 1)
    {
        throw std::invalid_argument("Maximum queue size must be positive");
    }
    pImpl->mMaximumQueueSize = maxSize;
}

int PickStoreOptions::getMaximumQueueSize() const noexcept
{
    return pImpl->mMaximumQueueSize;
}
