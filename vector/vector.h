#pragma once
#include <iostream>
#include <exception>

template<typename T>
class Vector
{
public:
    Vector() = default;
    explicit Vector(int count,int value);
    Vector(const std::initializer_list<T>&);
    Vector(const Vector<T>& src);                             ///copy ctor
    Vector<T>& operator=(const Vector<T>& src);               /// copy assignment copy swap idiom
    Vector(Vector<T>&& rhs) noexcept;                         /// move ctor
    Vector<T>& operator=(Vector<T>&& rhs) noexcept;           /// move assignment operator
    
    ~Vector(){ delete[] m_ptr;}                               /// destructor

    T operator[](const size_t index);

    template<typename U>
    void push_back(U&& value);  /// universal reference/perfect forwarding
    
    void insert(const T& value, const size_t pos);                      
    void insert(const T* const sArray,const int size,const size_t pos);     
    void insert(Vector<T> dArray,const size_t pos);                       

    void deleteitem(const size_t pos);                              ///                       
    void deleteRange(size_t pos, int count);

    void search(const T& value) const;                                  

    void push(const T* array,int size);
    void push(const T* array);
    void push(int value,int count);

    void resize(int count, int value);                                  ///

    bool isempty() const;
    void print() const;
    void capacity() const;
    size_t getSize() const;

private:
    size_t m_capacity = 0;
    size_t m_size = 0;
    T* m_ptr{nullptr};

    void validRange(const size_t pos) const;
    void resize(const size_t& new_capacity);
};

template<typename T>
Vector<T>::Vector(const Vector<T>& src) 
            : m_capacity{src.m_capacity},
              m_size{src.m_size}
{
    std::cout << "copy ctor called" << std::endl;
    m_ptr = new T [m_capacity];
    
    for(int i = 0; i < m_size; ++i)
    {
        *(m_ptr+i) = *(src.m_ptr+i);
    }
}

template<typename T>
Vector<T>::Vector(const std::initializer_list<T>& list)
{
    for(const T& value : list)
    {
        push_back(value);
    }
}

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& src)         /// copy assignment operator
{
    if(&src == this)
        return *this;
        
    Vector<T> tmp {src};
    m_capacity = tmp.m_capacity;
    m_size = tmp.m_size;
    delete[] m_ptr;
    m_ptr = tmp.m_ptr;
    tmp.m_ptr = nullptr;
    return *this;
}

template<typename T>
Vector<T>::Vector(Vector<T>&& rhs) noexcept                             ///move ctor
{
    std::cout << "move ctor called" << std::endl;
    m_capacity = rhs.m_capacity;
    m_size = rhs.m_size;
    m_ptr = rhs.m_ptr;

    rhs.m_ptr = nullptr;
}

template<typename T>
Vector<T>& Vector<T>::operator=(Vector<T>&& rhs) noexcept                ///move assignment operator
{
    if(this == &rhs)
        return *this;
    delete[] this->m_ptr;
    this->m_ptr = rhs.m_ptr;
    this->m_size = rhs.m_size;
    this->m_capacity = rhs.m_capacity;

    rhs.m_ptr = nullptr;
}

template<typename T>
T Vector<T>::operator[](const size_t index)
{
    validRange(index);
    return m_ptr[index];
}

template<typename T>
template<typename U>
void Vector<T>::push_back(U&& value)
{
    std::cout << "push_back T&& called" << std::endl;
    if(m_capacity == m_size)
    {
        resize(m_capacity == 0 ? 1 : m_capacity<<1);
    }
    m_ptr[m_size++] = std::forward<U>(value);
    return;
         
}

template<typename T>
void Vector<T>::insert(const T& value,const size_t pos)
{
    validRange(pos);
    if(m_capacity == m_size)
    {
        resize(m_capacity == 0 ? 1 : m_capacity<<1);
    }
    for(int i = m_size; i > pos; --i)
    {
        m_ptr[i] = m_ptr[i - 1];
    }
    m_ptr[pos] = value;
    ++m_size;
    return;
}

template<typename T>
void Vector<T>::insert(const T* const sArray,int size,const size_t pos)
{
    validRange(pos);
    if(m_capacity <= m_size + size)
    {
        resize(m_capacity == 0 ? 1 : (m_capacity+size)<<1);
    }
    for(int i = m_size-1, j = m_size + size -1; i >= pos; --i,--j)
    {
        m_ptr[j] = m_ptr[i];
    }
    for(int i = pos, k = 0; i < pos + size; ++i,++k)
    {
        m_ptr[i] = sArray[k];
    }
    m_size += size;
}

template<typename T>
void Vector<T>::insert(Vector<T> dArray,const size_t pos)
{
    validRange(pos);
    if(m_capacity <= m_size + dArray.getSize())
    {
        resize(m_capacity == 0 ? 1 : (m_capacity + dArray.getSize())<<1);
    }
    for(int i = m_size-1, j = m_size + dArray.getSize()-1; i >= pos; --i,--j)
    {
        m_ptr[j] = m_ptr[i];
    }
    for(int i = pos, k = 0; k < dArray.getSize(); ++i,++k)
    {
        m_ptr[i] = std::move(dArray[k]);
    }
    m_size += dArray.getSize();
}

template<typename T>
void Vector<T>::validRange(const size_t pos) const
{
    if(pos >= m_size)
    {
        throw std::out_of_range("index is out of bounds");
    }    
}

template<typename T>
void Vector<T>::resize(const size_t& new_capacity)
{
    T* tmp_ptr = new T [new_capacity];
    m_capacity = new_capacity;
    for(size_t i = 0; i < m_size; ++i)
    {
        tmp_ptr[i] = std::move(m_ptr[i]);
    }
    delete[] m_ptr;
    m_ptr = tmp_ptr;
    tmp_ptr = nullptr;
}

template<typename T>
void Vector<T>::print() const
{
    for(int i = 0; i < m_size; ++i)
    {
        std::cout<< *(m_ptr+i) << " ";
    }
    std::cout << std::endl;
}

template<typename T>
void Vector<T>::capacity() const
{
    std::cout << m_capacity << std::endl;
}

template<typename T>
bool Vector<T>::isempty() const
{
    return m_size;
}

template<typename T>
size_t Vector<T>::getSize() const
{
    return m_size;
}