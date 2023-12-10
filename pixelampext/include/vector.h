#pragma once

// Self implementation of the vector Class in C++

#ifdef SIMULATION
#ifdef _DEBUG
#include "../simu_include/TrackAllocation.h"
#endif
#endif

/// <summary>
/// Simple vector class to replace std::vector not avalaible on Arduino.
/// </summary>
/// <typeparam name="T">Type of data to record.</typeparam>
template <class T> class  vector
{
public:
    /// <summary>
    /// Size of buffer.
    /// </summary>
    typedef size_t              size_type;

    /// <summary>
    /// Iterator.
    /// </summary>
    typedef T*                  iterator;

    /// <summary>
    /// Constant iterator.
    /// </summary>
    typedef const T*            const_iterator;

private:
    /// <summary>
    /// Real size.
    /// </summary>
    size_type                   m_stSize;

    /// <summary>
    /// Buffer capacity (>= m_stSize).
    /// </summary>
    size_type                   m_stCapacity;

    /// <summary>
    /// Internal array.
    /// </summary>
    T *                         m_buffer;

public:
    /// <summary>
    /// Default constructor.
    /// </summary>
    vector();

    /// <summary>
    /// Copy constructor.
    /// </summary>
    /// <param name="_rOther">Array to copy from.</param>
    vector(const vector<T> & _rOther);

    /// <summary>
    /// Move constructor.
    /// </summary>
    /// <param name="_rOther"></param>
    /// <param name="_rOther">Array to copy from. This array is cleared after this function call.</param>
    vector(vector<T>&& _rOther);

    /// <summary>
    /// Constructor with firset capacity.
    /// </summary>
    /// <param name="_stSize">Size of buffer and capacity.</param>
    vector(size_type _stSize);

    /// <summary>
    /// Constructor with one item several time.
    /// </summary>
    /// <param name="_stSize">Initiale buffer size and capacity.</param>
    /// <param name="_initialBuffer">Inititial buffer to copy from. It's copied x _stSize to internal buffer.</param>
    vector(size_type _stSize, const T& _initialBuffer);

    /// <summary>
    /// Destructor.
    /// </summary>
    virtual ~vector();

    /// <summary>
    /// Get the capacity of internal buffer.
    /// </summary>
    /// <returns>The capacity.</returns>
    size_type                   capacity() const;

    /// <summary>
    /// Get the size of vector (less or equal than capacity).
    /// </summary>
    /// <returns>The capacity.</returns>
    size_type                   size() const;

    /// <summary>
    /// Is the vector is empty?
    /// </summary>
    /// <returns></returns>
    bool                        empty() const;

    /// <summary>
    /// Begin iterator.
    /// </summary>
    /// <returns>An iterator from the beginning of the list.</returns>
    iterator                    begin();

    /// <summary>
    /// End iterator.
    /// </summary>
    /// <returns>An iterator from the end of the list.</returns>
    iterator                    end();

    /// <summary>
    /// Begin const iterator.
    /// </summary>
    /// <returns>An iterator from the beginning of the const list.</returns>
    const_iterator              begin() const;

    /// <summary>
    /// Const end iterator.
    /// </summary>
    /// <returns>An iterator from the end of the const list.</returns>
    const_iterator	            end() const;

    /// <summary>
    /// Get the first element of the list.
    /// 
    /// Be careful, no check is done to know if the list is empty or not, check with empty() function before call
    /// this function.
    /// </summary>
    /// <returns>The first element of the list.</returns>
    T&                          front();

    /// <summary>
    /// Get the last element of the list.
    /// 
    /// Be careful, no check is done to know if the list is empty or not, check with empty() function before call
    /// this function.
    /// </summary>
    /// <returns>The first element of the list.</returns>
    T&                          back();

    /// <summary>
    /// Get the first element of the const list.
    /// 
    /// Be careful, no check is done to know if the list is empty or not, check with empty() function before call
    /// this function.
    /// </summary>
    /// <returns>The first element of the const list.</returns>
    const T&                    front() const;

    /// <summary>
    /// Get the last element of the const list.
    /// 
    /// Be careful, no check is done to know if the list is empty or not, check with empty() function before call
    /// this function.
    /// </summary>
    /// <returns>The first element of the const list.</returns>
    const T&                    back() const;

    /// <summary>
    /// Add new item at the last item of the array.
    /// 
    /// Grow array if needed.
    /// </summary>
    /// <param name="_rValue">Value to add.</param>
    void                        push_back(const T& _rValue);

    /// <summary>
    /// Remove last item of the array.
    /// 
    /// The capacity is not changed, only size of array change.
    /// </summary>
    void                        pop_back();

    /// <summary>
    /// Reserve a specific capacity.
    /// 
    /// Allocate new buffer for this capacity, elements are copied from old buffer to new one.
    /// If the capacity is less than array size, all more elements are lost.
    /// </summary>
    /// <param name="_stCapacity"New capacity.></param>
    void                        reserve(size_type _stCapacity);

    /// <summary>
    /// Resize to new number of elements.
    /// 
    /// Allocate new buffer for this size only if needed.
    /// If the size is less than current array size, all more elements are lost.
    /// </summary>
    /// <param name="_stCapacity"New size.></param>
    void                        resize(size_type _stSize);

    /// <summary>
    /// Access to one element.
    /// </summary>
    /// <param name="_stIndex">Index of item.</param>
    /// <returns>The element at _stIndex position.</returns>
    T &                         operator[](size_type _stIndex);

    /// <summary>
    /// Access to one const element.
    /// </summary>
    /// <param name="_stIndex">Index of item.</param>
    /// <returns>The const element at _stIndex position.</returns>
    const T &                   operator[](size_type _stIndex) const;

    /// <summary>
    /// Equal operator.
    /// </summary>
    /// <param name="_rOther">Copy array into this.</param>
    /// <returns><A reference to this./returns>
    vector<T> &                 operator=(const vector<T> &_rOther);

    /// <summary>
    /// Clear internal buffer.
    /// </summary>
    void                        clear();

    /// <summary>
    /// Add an array element into the list.
    /// </summary>
    /// <typeparam name="N">Number of elements.</typeparam>
    /// <param name="_data">Array of datas.</param>
    template<uint32_t N> void   push_array(const T _data[N]);

private:
    /// <summary>
    /// Make the buffer allocation.
    ///
    /// The buffer is allocated with capacity size.
    /// </summary>
    void                        allocate_buffer();

    /// <summary>
    /// Copy buffer into this.
    /// </summary>
    /// <param name="_buffer">Origin buffer.</param>
    /// <param name="_destBuffer">Destination buffer if needed, else use internal buffer (if == nullptr).</param>
    void                        copy_buffer(const T* _buffer, T* _destBuffer = nullptr);
};

template<class T>
vector<T>::vector()
{
    m_stCapacity = 0;
    m_stSize     = 0;
    m_buffer     = nullptr;
}

/// <summary>
/// Copy constructor.
/// </summary>
/// <typeparam name="T"><Type of data/typeparam>
/// <param name="_rOther">Reference to other vector.</param>
template<class T>
vector<T>::vector(const vector<T>& _rOther)
{
    m_stSize     = _rOther.m_stSize;
    m_stCapacity = _rOther.m_stCapacity;
    allocate_buffer();
    copy_buffer(_rOther.m_buffer);
}

template<class T>
vector<T>::vector(vector<T>&& _rOther)
    : m_stSize(_rOther.m_stSize)
    , m_stCapacity(_rOther.m_stCapacity)
    , m_buffer(_rOther.m_buffer)
{
    // Clear _rOther without deleting buffer (move constructor).
    _rOther.m_buffer = nullptr;
    _rOther.m_stSize = _rOther.m_stCapacity = 0;
}

template<class T>
vector<T>::vector(size_type _stSize)
{
    m_stSize = m_stCapacity = _stSize;
    allocate_buffer();
}

template<class T>
vector<T>::vector(size_type _stSize, const T& _initialBuffer)
{
    m_stSize = m_stCapacity = _stSize;
    allocate_buffer();
    for (size_type stIndex = 0; stIndex < size; ++stIndex)
    {
        m_buffer[stIndex] = _initialBuffer;
    }
}

template<class T>
vector<T>::~vector()
{
    clear();
}

template<class T>
typename vector<T>::size_type vector<T>::capacity() const
{
    return m_stCapacity;
}

template<class T>
typename vector<T>::size_type vector<T>::size() const
{
    return m_stSize;
}

template<class T>
bool vector<T>::empty() const
{
    return size() == 0;
}

template<class T>
typename vector<T>::iterator vector<T>::begin()
{
    return m_buffer;
}

template<class T>
typename vector<T>::iterator vector<T>::end()
{
    return m_buffer == nullptr
        ? nullptr
        : m_buffer + size();
}

template<class T>
typename vector<T>::const_iterator vector<T>::begin() const
{
    return m_buffer;
}

template<class T>
typename vector<T>::const_iterator vector<T>::end() const
{
    return m_buffer == nullptr
        ? nullptr
        : m_buffer + size();
}

template<class T>
T& vector<T>::front()
{
    return m_buffer[0];
}

template<class T>
T& vector<T>::back()
{
    return m_buffer[m_stSize - 1];
}

template<class T>
const T& vector<T>::front() const
{
    return m_buffer[0];
}

template<class T>
const T& vector<T>::back() const
{
    return m_buffer[m_stSize - 1];
}

template<class T>
void vector<T>::push_back(const T& _rValue)
{
    if (m_stSize >= m_stCapacity)
    {
        reserve(m_stCapacity + 5);
    }
    m_buffer[m_stSize++] = _rValue;
}

template<class T>
void vector<T>::pop_back()
{
    T * toDelete = &m_buffer[m_stSize - 1];

    if (++m_stSize == 0)
    {
        clear();
    }
}

template<class T>
void vector<T>::reserve(size_type _stCapacity)
{
    T* pNewBuffer = _stCapacity == 0 ? nullptr : new T[_stCapacity];
    m_stSize = min(_stCapacity, size());
    m_stCapacity = _stCapacity;
    copy_buffer(m_buffer, pNewBuffer);
    if (m_buffer !=  nullptr)
    {
        delete[] m_buffer;
    }
    m_buffer = pNewBuffer;
}

template<class T>
void vector<T>::resize(size_type _stSize)
{
    reserve(_stSize);
    m_stSize = size;
}

template<class T>
T & vector<T>::operator[](size_type _stIndex)
{
    return m_buffer[_stIndex];
}

template<class T>
const T & vector<T>::operator[](size_type _stIndex) const
{
    return m_buffer[_stIndex];
}

template<class T>
vector<T>& vector<T>::operator=(const vector<T> & _rValue)
{
    clear();
    m_stSize = _rValue.m_stSize;
    m_stCapacity = _rValue.m_stCapacity;
    allocate_buffer();
    copy_buffer(_rValue.m_buffer);
    return *this;
}

template <class T>
void vector<T>::clear()
{
    m_stCapacity = 0;
    m_stSize = 0;
    if (m_buffer != nullptr)
    {
        delete[]m_buffer;
    }
    m_buffer = nullptr;
}

template <class T>
template<uint32_t N> void vector<T>::push_array(const T _data[N])
{
    if (m_stCapacity < N)
    {
        clear();
        m_stCapacity = m_stSize = N;
        allocate_buffer();
    }
    else
    {
        m_stSize = N;
    }
    copy_buffer(_data);
}

template <class T>
void vector<T>::allocate_buffer()
{
    m_buffer = (capacity() == 0)
        ? nullptr
        : new T[m_stCapacity];
}

template <class T>
void vector<T>::copy_buffer(const T* _buffer, T* _destBuffer)
{
    _destBuffer = (_destBuffer == nullptr) ? m_buffer : _destBuffer;
    for (size_type stIndex = 0; stIndex < size(); ++stIndex)
    {
        _destBuffer[stIndex] = _buffer[stIndex];
    }
    // memcpy(_destBuffer, _buffer, size() * sizeof(T));
}