#include <string>
#include "uFilterPickerMessageStore/exception.hpp"

using namespace UFilterPickerProxy;

DuplicatePickException::DuplicatePickException(const std::string &message) :
    mMessage(message)
{   
}

const char *DuplicatePickException::what() const noexcept
{   
   return mMessage.c_str();
}   

