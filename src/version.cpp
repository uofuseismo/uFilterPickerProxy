#include <string>
#include "uFilterPickerPickBroker/version.hpp"

using namespace UFilterPickerPickBroker;

int Version::getMajor() noexcept
{
    return uFilterPickerPickBroker_MAJOR;
}

int Version::getMinor() noexcept
{
    return uFilterPickerPickBroker_MINOR;
}

int Version::getPatch() noexcept
{
    return uFilterPickerPickBroker_PATCH;
}

//NOLINTBEGIN(bugprone-easily-swappable-parameters)
bool Version::isAtLeast(const int major, const int minor,
                        const int patch) noexcept
//NOLINTEND(bugprone-easily-swappable-parameters)
{
    if (uFilterPickerPickBroker_MAJOR < major){return false;}
    if (uFilterPickerPickBroker_MAJOR > major){return true;}
    if (uFilterPickerPickBroker_MINOR < minor){return false;}
    if (uFilterPickerPickBroker_MINOR > minor){return true;}
    if (uFilterPickerPickBroker_PATCH < patch){return false;}
    return true;
}

std::string Version::getVersion() noexcept
{
    std::string version{uFilterPickerPickBroker_VERSION};
    return version;
}

std::string Version::getTag() noexcept
{
    std::string tag{uFilterPickerPickBroker_GITTAG};
    return tag;
}

std::string Version::getVersionWithTag() noexcept
{
    auto tag = Version::getTag();
    if (tag.empty())
    {
        return Version::getVersion();
    }
    else
    {
        return Version::getVersion() + "-" + tag;
    }
}
