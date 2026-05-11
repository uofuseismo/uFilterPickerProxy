#ifndef UFILTER_PICKER_PICK_BROKER_EXCEPTION_HPP
#define UFILTER_PICKER_PICK_BROKER_EXCEPTION_HPP
#include <string>
#include <exception>
namespace UFilterPickerPickBroker
{
/// @class DuplicatePickException exception.hpp
/// @brief Use this error when a pick already exists in the database.
/// @copyright Ben Baker (University of Utah) distributed under the
///             MIT NO AI license.
class DuplicatePickException : public std::exception
{
public:
    explicit DuplicatePickException(const std::string &message);
    [[nodiscard]] const char *what() const noexcept;
private:
    std::string mMessage;
};
}
#endif
