#ifndef __utils_included_034967102602157602372031257032075301265983470946067051
#define __utils_included_034967102602157602372031257032075301265983470946067051

#include <cstdint>
#include <string>
#include <cstdio>

#include "immutable_array.hpp"

namespace utils {

//=======================================================================================================================================================================================================================
// file input/output and utility functions
//=======================================================================================================================================================================================================================
namespace fileio {

/* determine total file size */
size_t fsize(FILE* f);

/* reads file to std::string */
std::string freadtostr(const std::string& path);

/* reads the whole file to memory buffer, buffer starting address is always aligned by 4
   needed to avoid extra copies as Vulkan requires buffer data / shader code to be 4-aligned in some cases */
immutable_array_t<uint8_t> freadall(const char* path, bool null_terminate = false);
immutable_array_t<uint8_t> freadall(const std::string& path, bool null_terminate = false);

/* normalize path string by unifying separators to / */
/* replace consecutive / or \ slashes with a single / */
std::string normalize(const char* path);
std::string normalize(const std::string& path);

/* get filename without directory */
std::string fname(const std::string& path);

/* get filename without extension */
std::string fname_noext(const std::string& path);

/* get directory name, excluding slash / */
std::string fdir(const std::string& path);

/* get file extension */
std::string fext(const std::string& filename);

}

//=======================================================================================================================================================================================================================
// cross-platform high resolution timer function
//=======================================================================================================================================================================================================================
namespace timer {


uint64_t ns();

struct cumulative_timer
{
    uint32_t periods;
    uint64_t duration;
    uint64_t timestamp;                     /* last timestamp */

    cumulative_timer()
        { reset(); }

    void reset()
    {
        periods = 0;
        duration = 0;
    }

    void start()
    {
        ++periods;
        timestamp = ns();
    }

    void stop()
    {
        uint64_t timestamp_now = ns();
        duration += (timestamp_now - timestamp);
    }

    double total_duration()
        { return duration * 1e-9; }

    double average_duration()
        { return total_duration() / periods; }
};

} /* namespace timer */

} /* namespace utils */

#endif /* __utils_included_034967102602157602372031257032075301265983470946067051 */
