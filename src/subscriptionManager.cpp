#include <mutex>
#include "uFilterPickerProxy/subscriptionManager.hpp"
#include "uFilterPickerProxy/subscriptionManagerOptions.hpp"

using namespace UFilterPickerProxy;

class SubscriptionManager::SubscriptionManagerImpl
{
public:
    ~SubscriptionManagerImpl()
    {
        unsubscribeAll();
    }
    /// Purge all subscribers
    void unsubscribeAll()
    {

    }
//private:
    std::mutex mMutex;
    SubscriptionManagerOptions mOptions;
};

/// @brief Destructor.
SubscriptionManager::~SubscriptionManager() = default;