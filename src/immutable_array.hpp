#ifndef _immutable_array_included_915206811753082280627561098560817235608271357
#define _immutable_array_included_915206811753082280627561098560817235608271357

#include <cstdlib>
#include <mm_malloc.h>
#include <cstring>
#include <algorithm>

template<typename T> struct immutable_array_t
{
    unsigned int size;
    T* data;

    immutable_array_t() : size(0), data(0) {}

    immutable_array_t(unsigned int size)
    {
        /* allocate space */
        immutable_array_t::size = 0;
        if (data = (T*) std::malloc(size * sizeof(T)))
            immutable_array_t::size = size;

        /* call placement new with default constructor for each element */
        for (unsigned int i = 0; i < immutable_array_t::size; ++i)
            new(data + i) T();
    }

    immutable_array_t(unsigned int size, const T& value)
    {
        /* allocate space */
        immutable_array_t::size = 0;
        if (data = (T*) std::malloc(size * sizeof(T)))
            immutable_array_t::size = size;

        /* call placement new with copy constructor for each element */
        for (unsigned int i = 0; i < immutable_array_t::size; ++i)
            new(data + i) T(value);
    }

    immutable_array_t& operator = (immutable_array_t&& other)
    {
        std::swap(size, other.size);
        std::swap(data, other.data);
        return *this;
    }

    immutable_array_t(immutable_array_t&& other)
    {
        size = other.size;
        data = other.data;
        other.size = 0;
        other.data = 0;
    }

    immutable_array_t(const immutable_array_t& other) = delete;
    immutable_array_t& operator = (const immutable_array_t&) = delete;

    /* implicit conversion to bool */
    operator bool() const
        { return data != 0; }

    /* allocate and default create elements */
    void allocate(unsigned int size)
    {
        /* call destructor for each element */
        if (data)
        {
            for (unsigned int i = 0; i < immutable_array_t::size; ++i)
                data[i].~T();
            std::free(data);
        }

        /* allocate space */
        immutable_array_t::size = 0;
        if (data = (T*) std::malloc(size * sizeof(T)))
            immutable_array_t::size = size;

        /* call placement new with default constructor for each element */
        for (unsigned int i = 0; i < immutable_array_t::size; ++i)
            new(data + i) T();
    }

    /* allocate and default create elements */
    void allocate(unsigned int size, const T& value)
    {
        /* call destructor for each element */
        if (data)
        {
            for (unsigned int i = 0; i < immutable_array_t::size; ++i)
                data[i].~T();
            std::free(data);
        }

        /* allocate space */
        immutable_array_t::size = 0;
        if (data = (T*) std::malloc(size * sizeof(T)))
            immutable_array_t::size = size;

        /* call placement new with default constructor for each element */
        for (unsigned int i = 0; i < immutable_array_t::size; ++i)
            new(data + i) T(value);
    }

    void copy(const T* src)
        { std::memcpy(data, src, size * sizeof(T)); }

    T& operator [] (unsigned int index)
        { return data[index]; }

    const T& operator [] (unsigned int index) const
        { return data[index]; }

    void clear()
        { std::memset(data, 0, size * sizeof(T)); }

    void clear(unsigned int base_index, unsigned int count)
        { std::memset(data + base_index, 0, count * sizeof(T)); }

    void fill(const T& value)
    {
        for(unsigned int i = 0; i < size; ++i)
            data[i] = value;
    }

    void fill(const T& value, unsigned int base_index, unsigned int count)
    {
        for(unsigned int i = base_index; i < base_index + count; ++i)
            data[i] = value;
    }

    T* release_ptr()
    {
        T* ptr = data;
        data = 0;
        size = 0;
        return ptr;
    }

    operator T* ()
        { return data; }

    ~immutable_array_t()
    {
        if (data)
        {
            for (unsigned int i = 0; i < size; ++i)
                data[i].~T();

            std::free(data);
        }
    }
};

template<typename T> struct aligned_array_t
{
    unsigned int size;
    T* data;

    aligned_array_t() : size(0), data(0) {}

    aligned_array_t(size_t alignment, unsigned int size)
    {
        /* allocate space */
        aligned_array_t::size = 0;
        if (data = (T*) _mm_malloc(size * sizeof(T), alignment))
            aligned_array_t::size = size;

        /* call placement new with default constructor for each element */
        for (unsigned int i = 0; i < aligned_array_t::size; ++i)
            new(data + i) T();
    }

    aligned_array_t(size_t alignment, unsigned int size, const T& value)
    {
        /* allocate space */
        aligned_array_t::size = 0;
        if (data = (T*) _mm_malloc(size * sizeof(T), alignment))
            aligned_array_t::size = size;

        /* call placement new with copy constructor for each element */
        for (unsigned int i = 0; i < aligned_array_t::size; ++i)
            new(data + i) T(value);
    }

    aligned_array_t& operator = (aligned_array_t&& other)
    {
        std::swap(size, other.size);
        std::swap(data, other.data);
        return *this;
    }

    aligned_array_t(aligned_array_t&& other)
    {
        size = other.size;
        data = other.data;
        other.size = 0;
        other.data = 0;
    }

    aligned_array_t(const aligned_array_t& other) = delete;
    aligned_array_t& operator = (const aligned_array_t&) = delete;

    /* implicit conversion to bool */
    operator bool() const
        { return data != 0; }

    /* allocate and default create elements */
    void allocate(size_t alignment, unsigned int size)
    {
        /* call destructor for each element */
        if (data)
        {
            for (unsigned int i = 0; i < aligned_array_t::size; ++i)
                data[i].~T();
            _mm_free(data);
        }

        /* allocate space */
        aligned_array_t::size = 0;
        if (data = (T*) _mm_malloc(size * sizeof(T), alignment))
            aligned_array_t::size = size;

        /* call placement new with default constructor for each element */
        for (unsigned int i = 0; i < aligned_array_t::size; ++i)
            new(data + i) T();
    }

    /* allocate and default create elements */
    void allocate(size_t alignment, unsigned int size, const T& value)
    {
        /* call destructor for each element */
        if (data)
        {
            for (unsigned int i = 0; i < aligned_array_t::size; ++i)
                data[i].~T();
            _mm_free(data);
        }

        /* allocate space */
        aligned_array_t::size = 0;
        if (data = (T*) _mm_malloc(size * sizeof(T), alignment))
            aligned_array_t::size = size;

        /* call placement new with default constructor for each element */
        for (unsigned int i = 0; i < aligned_array_t::size; ++i)
            new(data + i) T(value);
    }

    void copy(const T* src)
        { std::memcpy(data, src, size * sizeof(T)); }

    T& operator [] (unsigned int index)
        { return data[index]; }

    const T& operator [] (unsigned int index) const
        { return data[index]; }

    void clear()
        { std::memset(data, 0, size * sizeof(T)); }

    void clear(unsigned int base_index, unsigned int count)
        { std::memset(data + base_index, 0, count * sizeof(T)); }

    void fill(const T& value)
    {
        for(unsigned int i = 0; i < size; ++i)
            data[i] = value;
    }

    void fill(const T& value, unsigned int base_index, unsigned int count)
    {
        for(unsigned int i = base_index; i < base_index + count; ++i)
            data[i] = value;
    }

    T* release_ptr()
    {
        T* ptr = data;
        data = 0;
        size = 0;
        return ptr;
    }

    operator T* ()
        { return data; }

    ~aligned_array_t()
    {
        if (data)
        {
            for (unsigned int i = 0; i < size; ++i)
                data[i].~T();

            _mm_free(data);
        }
    }
};

#endif /* _immutable_array_included_915206811753082280627561098560817235608271357 */
