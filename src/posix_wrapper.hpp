#ifndef POSIX_WRAPPER
#define POSIX_WRAPPER

#include <string>
#include <system_error>
#include <net/if.h>

namespace PosixWrapper
{
    unsigned int if_nametoindex(const std::string& ifname);
}

#endif
