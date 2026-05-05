#include <memory>
#include <functional>
#include <spdlog/spdlog.h>
#include <spdlog/logger.h>
#include "uFilterPickerProxy/frontend.hpp"
#include "uFilterPickerProxy/frontendOptions.hpp"
#include "uFilterPickerProxy/grpcServerOptions.hpp"

using namespace UFilterPickerProxy;

class Frontend::FrontendImpl
{
public:
    FrontendOptions mOptions;
    std::function<void (UFilterPickerProxyAPI::V1::Pick &&)> mCallback;
    std::shared_ptr<spdlog::logger> mLogger{nullptr};
};

/// Destructor
Frontend::~Frontend() = default;
