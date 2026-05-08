#include <string>
#include "uFilterPickerMessageStore/version.hpp"

using namespace UFilterPickerMessageStore;

int Version::getMajor() noexcept
{
    return uFilterPickerMessageStore_MAJOR;
}

int Version::getMinor() noexcept
{
    return uFilterPickerMessageStore_MINOR;
}

int Version::getPatch() noexcept
{
    return uFilterPickerMessageStore_PATCH;
}

//NOLINTBEGIN(bugprone-easily-swappable-parameters)
bool Version::isAtLeast(const int major, const int minor,
                        const int patch) noexcept
//NOLINTEND(bugprone-easily-swappable-parameters)
{
    if (uFilterPickerMessageStore_MAJOR < major){return false;}
    if (uFilterPickerMessageStore_MAJOR > major){return true;}
    if (uFilterPickerMessageStore_MINOR < minor){return false;}
    if (uFilterPickerMessageStore_MINOR > minor){return true;}
    if (uFilterPickerMessageStore_PATCH < patch){return false;}
    return true;
}

std::string Version::getVersion() noexcept
{
    std::string version{uFilterPickerMessageStore_VERSION};
    return version;
}

std::string Version::getTag() noexcept
{
    std::string tag{uFilterPickerMessageStore_GITTAG};
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
