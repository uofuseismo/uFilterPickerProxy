#ifndef UFILTER_PICKER_PROXY_BACKEND_HPP
#define UFILTER_PICKER_PROXY_BACKEND_HPP
#include <spdlog/logger.h>
#include <memory>
#include <functional>
namespace UFilterPickerProxyAPI::V1
{
 class Pick;
}
namespace UFilterPickerProxy
{
 class Database;
 class BackendOptions;
 class PickStore;
}

namespace UFilterPickerProxy
{
/// @class Backend backend.hpp
/// @brief This is the backend from which picks are consumed.
/// @copyright Ben Baker (University of Utah) distributed under the
///            MIT NO AI license.
class Backend
{
public:
    /// @brief Constructor.
    /// @param[in] options   The backend's options.
    /// @param[in] store     The pick store for the backend.
    /// @param[in] logger    The logging utility.
    Backend(const BackendOptions &options,
            std::unique_ptr<PickStore> &&store,
            std::shared_ptr<spdlog::logger> logger);

    /// @brief Starts the backend's pick publishing.
    void start();

    /// @brief Enqueues a pick to be written to consumers.
    void enqueue(UFilterPickerProxyAPI::V1::Pick &&pick);

    /// @brief Stops the backend and prevents the propagation of picks.
    void stop();

    /// @brief Destructor.
    ~Backend();
    Backend() = delete;
    Backend(const Backend &) = delete;
    Backend(Backend &&) noexcept = delete;
    Backend &operator=(const Backend &) = delete;
    Backend &operator=(Backend &&) noexcept = delete;
private:
    class BackendImpl;
    std::unique_ptr<BackendImpl> pImpl;
};
}
#endif