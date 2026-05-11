#include <string>
#include "uFilterPickerPickBroker/exception.hpp"

using namespace UFilterPickerPickBroker;

DuplicatePickException::DuplicatePickException(const std::string &message) :
    mMessage(message)
{
}

const char *DuplicatePickException::what() const noexcept
{
   return mMessage.c_str();
}
