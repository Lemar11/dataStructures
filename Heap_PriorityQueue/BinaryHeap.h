#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <cstddef>


/// MAXHEAP imlementation - Priority Queue
template<typename T>
class Heap
{
    private:
        size_t heap_size;
        std::vector<T> array;
        void maxHeapify(size_t index);
        void buildMaxHeap();

    public:
        Heap();
        Heap(const std::initializer_list<T>& list);

        Heap(const Heap<T>& src) = default;
        Heap<T>& operator=(const Heap<T>& rhs) = default;

        Heap(Heap<T>&& src) = default;                      
        Heap<T>& operator=(Heap<T>&& rhs) = default;

        ~Heap() = default;

        void insert(T value);                           ////////////////  TC log n
        void print() const;
        T extractMax();                                 ////////////////  TC log n
        T getMax() const;                               ////////////////  TC O(1)
        void increaseKey(size_t index,size_t value);    ////////////////  TC log n
};

template<typename T>
Heap<T>::Heap() : heap_size{0} {}

template<typename T>
Heap<T>::Heap(const std::initializer_list<T>& list) 
                        : array{list.begin(),list.end()},
                          heap_size{list.size()}
{
    buildMaxHeap();
}

template<typename T>
void Heap<T>::maxHeapify(size_t index)
{
    size_t L = 2*index + 1;
    size_t R = 2*index + 2;
    size_t largest = index;
    if(L < heap_size && array[index] < array[L])
        largest = L;
    if(R < heap_size && array[largest] < array[R])
        largest = R;
    if(largest != index)
    {
        std::swap(array[index],array[largest]);
        maxHeapify(largest);
    }
}

template<typename T>
void Heap<T>::buildMaxHeap()
{
    size_t i = heap_size / 2 - 1;                                   ///last root node // bottom up cheaking
    while(i != SIZE_MAX)
    {
        maxHeapify(i);
        --i;
    }   
}

template<typename T>
void Heap<T>::insert(T value)
{
    array.push_back(std::move(value));
    ++heap_size;

    size_t curindex = heap_size - 1;
    size_t parent = (curindex - 1)/2;

    const T tmp = array[curindex];
    while(tmp > array[parent] && curindex != 0)
    {
        array[curindex] = array[parent];
        curindex = parent;
        parent = (curindex - 1)/2;
    }
    array[curindex] = tmp;
}

template<typename T>
T Heap<T>::extractMax()
{
    std::swap(array[0],array[heap_size-1]);
    --heap_size;
    maxHeapify(0);
    return array[heap_size];
}

template<typename T>
T Heap<T>::getMax() const
{
    return array[0];
}

template<typename T>
void Heap<T>::increaseKey(size_t index,size_t value)    ///increase priority
{
    if(value < array[index])
    {
        throw std::logic_error("value you entered is smaller than exciting priority value");
    }
    array[index] = value;
    while(index > 0 && array[index] > array[(index-1)/2])
    {
        std::swap(array[index],array[(index-1)/2]);
        index = (index-1)/2;
    }
}

template<typename T>
void Heap<T>::print() const
{
    for(int i = 0; i < heap_size; ++i)
    {
        std::cout << array[i] << ' ';
    }

    std::cout << std::endl;
}