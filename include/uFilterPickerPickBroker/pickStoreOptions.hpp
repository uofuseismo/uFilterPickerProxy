#ifndef UFILTER_PICKER_PICK_BROKER_PICK_STORE_OPTIONS_HPP
#define UFILTER_PICKER_PICK_BROKER_PICK_STORE_OPTIONS_HPP
#include <memory>
namespace UFilterPickerPickBroker
{
/// @class PickStoreOptions pickStoreOptions.hpp
/// @brief Defines the behavior of the pick store.
/// @copyright Ben Baker (University of Utah) distributed under the
///            MIT NO AI license.
class PickStoreOptions
{
public:
    /// @brief Constructor.
    PickStoreOptions();
    /// @brief Copy constructor.
    PickStoreOptions(const PickStoreOptions &options);
    /// @brief Move constructor.
    PickStoreOptions(PickStoreOptions &&options) noexcept;

    /// @brief Sets the maximum message queue size.
    /// @param[in] maximumQueueSize  The maximum number of pick messages that
    ///                              can be stored in the queue.
    void setMaximumQueueSize(int maximumQueueSize);
    /// @return The maximum queue size.
    [[nodiscard]] int getMaximumQueueSize() const noexcept;

    /// @brief Destructor.
    ~PickStoreOptions();
    /// @brief Copy assignment.
    PickStoreOptions& operator=(const PickStoreOptions &options);
    /// @brief Move assignment.
    PickStoreOptions& operator=(PickStoreOptions &&options) noexcept;
private:
    class PickStoreOptionsImpl;
    std::unique_ptr<PickStoreOptionsImpl> pImpl;
};
}
#endif
