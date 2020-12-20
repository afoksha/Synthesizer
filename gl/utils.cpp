#include <cstdio>
#include <fstream>

#include "GL/glew.h"

#include "utils.hpp"
#include "log.hpp"

namespace utils {

namespace fileio {

/* determine total file size */
size_t fsize(FILE* f)
{
    fseek(f, 0, SEEK_END);
    size_t filesize = ftell(f);
    fseek(f, 0, SEEK_SET);
    return filesize;
}

/* reads file to std::string */
std::string freadtostr(const std::string& path)
{
    std::ifstream file(path.c_str());
    std::string result((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    return result;
}

/* reads the whole file to memory buffer */
immutable_array_t<uint8_t> freadall(const char* path, bool null_terminate)
{
    immutable_array_t<uint8_t> buffer;

    FILE* f = fopen(path, "rb");
    if (!f)
    {
        printf("File %s failed to open", path);
        return buffer;
    }

    size_t size = fsize(f);
    if (size == -1)
    {
        printf("File %s reading error. Cannot reach end of file or file is empty.", path);
        return buffer;
    }

    size_t esize = null_terminate ? size + 1 : size;
    uint8_t* data = (uint8_t*) malloc(esize);
    if (!data)
    {
        printf("Could not allocate memory to read the file %s. Size = %u.", path, (unsigned int) size);
        return buffer;
    }

    size_t s = fread(data, 1, size, f);
    fclose(f);
    if (s != size)
    {
        free(data);
        return buffer;
    }

    if (null_terminate)
        data[size] = '\0';

    buffer.size = esize;
    buffer.data = data;
    return buffer;
}

immutable_array_t<uint8_t> freadall(const std::string& path, bool null_terminate)
    { return freadall(path.c_str(), null_terminate); }

std::string normalize(const char* path)
{
    std::string normalized_path;
    bool last_is_slash = false;

    for (const char* c = path; *c; ++c)
    {
        if ((*c == '/') || (*c == '\\'))
        {
            if (!last_is_slash)
                normalized_path.push_back('/');
            last_is_slash = true;
        }
        else
        {
            last_is_slash = false;
            normalized_path.push_back(*c);
        }
    }
    return normalized_path;
}

/* normalize path by unifying separators to / */
/* replace consecutive / or \ slashes with a single / */
std::string normalize(const std::string& path)
{
    std::string normalized_path;
    bool last_is_slash = false;

    for (size_t i = 0; i < path.size(); ++i)
    {
        char c = path[i];
        if ((c == '/') || (c == '\\'))
        {
            if (!last_is_slash)
                normalized_path.push_back('/');
            last_is_slash = true;
        }
        else
        {
            last_is_slash = false;
            normalized_path.push_back(c);
        }
    }
    return normalized_path;
}

/* get filename without directory */
std::string fname(const std::string& path)
{
    std::string npath = normalize(path);
    return npath.substr(npath.rfind('/') + 1);
}

/* get filename without extension */
std::string fname_noext(const std::string& path)
{
    std::string fname = normalize(path);
    size_t dotp = fname.rfind('.');                                 /* position of the last dot character in the string */
    if (dotp != std::string::npos)
        fname = fname.substr(0, dotp);
    return fname;
}

/* get directory name, excluding slash / */
std::string fdir(const std::string& path)
{
    std::string filedir;
    std::string npath = normalize(path);
    size_t slashp = npath.rfind('/');                               /* position of the last slash character in the string */
    if (slashp != std::string::npos)
        filedir = npath.substr(0, slashp);
    return filedir;
}

/* get file extension */
std::string fext(const std::string& filename)
{
    std::string ext;
    std::string nfilename = normalize(filename);
    size_t dotp = nfilename.rfind('.');                             /* position of the last dot character in the string */
    if (dotp != std::string::npos)
        ext = nfilename.substr(dotp + 1);
    return ext;
}

} /* namespace fileio */

void printErrorsGL(const char *func, int line)
{
    GLenum glErr = glGetError();
    while (glErr != GL_NO_ERROR)
    {
        printf("%s:%i :\n",func,line);
        switch(glErr)
        {

            case GL_INVALID_ENUM: debug_msg("glError: GL_INVALID_OPERATION \n"); break;
            case GL_INVALID_VALUE: printf("glError: GL_INVALID_VALUE\n"); break;
            case GL_INVALID_OPERATION: printf("glError: Invalid operation \n"); break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                printf("glError: Invalid framebuffer operation \n");
                break;

            case GL_OUT_OF_MEMORY:
                printf("glError: Out of memory \n");
                break;

            #ifdef GL_STACK_UNDERFLOW
            case GL_STACK_UNDERFLOW:
                printf("glError: Stack underflow \n");
                break;
            #endif

            #ifdef GL_STACK_OVERFLOW
            case GL_STACK_OVERFLOW:
                printf("glError: Stack underflow \n");
                break;
            #endif

            default:
                printf("glError: unrecognized error \n");
        }
        glErr = glGetError();
    }
}

namespace timer {

#if defined(__linux)
    #define HAVE_POSIX_TIMER
    #include <time.h>
    #ifdef CLOCK_MONOTONIC
        #define CLOCKID CLOCK_MONOTONIC
    #else
        #define CLOCKID CLOCK_REALTIME
    #endif
#elif defined(__APPLE__)
    #define HAVE_MACH_TIMER
    #include <mach/mach_time.h>
#elif defined(_WIN32)
    #define WIN32_LEAN_AND_MEAN
    # include <windows.h>
#endif

uint64_t ns()
{
    static bool initialized = false;
#if defined(__APPLE__)
    static mach_timebase_info_data_t info;
    if (!initialized)
    {
        mach_timebase_info(&info);
        initialized = true;
    }
    uint64_t now;
    now = mach_absolute_time();
    now *= info.numer;
    now /= info.denom;
    return now;
#elif defined(__linux)
    static struct timespec linux_rate;
    if (!initialized)
    {
        clock_getres(CLOCKID, &linux_rate);
        initialized = true;
    }
    uint64_t now;
    struct timespec spec;
    clock_gettime(CLOCKID, &spec);
    now = spec.tv_sec * 1.0e9 + spec.tv_nsec;
    return now;
#elif defined(_WIN32)
    static LARGE_INTEGER win_frequency;
    if (!initialized)
    {
        QueryPerformanceFrequency(&win_frequency);
        initialized = true;
    }
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    return (uint64_t) ((1e9 * now.QuadPart)  / win_frequency.QuadPart);
#endif
}

} /* namespace timer */

} /* namespace utils */
