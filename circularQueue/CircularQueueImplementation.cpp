#include <iostream>

template<typename T>
class CircularQueue
{
    private:
        static const int MAX = 100;
        int front{0};
        int back{-1};
        int count{0};
        T items[MAX];

    public:
        CircularQueue() = default;
        CircularQueue(const CircularQueue<T>& src) = default;
        explicit CircularQueue(const std::initializer_list<T>& init)
        {
            for(const T& elem : init)
            {
                items[++back] = elem;
                ++count;
            }
        }
        ~CircularQueue() = default;

        bool isEmpty() const;
        void enqueue(const T& data);
        void dequeue();
        const int& size() const; 
        const T& getFront() const;
};

template<typename T>
bool CircularQueue<T>::isEmpty() const
{
    return count == 0;
}
template<typename T>
void CircularQueue<T>::enqueue(const T& data)
{
    if(count == MAX)
        throw std::runtime_error("queue overflow");
    else
    {
        back = (back+1) % MAX;
        items[back] = data;
        ++count;
    }
}
template<typename T>
void CircularQueue<T>::dequeue()
{
    if(count == 0)
        throw std::out_of_range("queue empty");
    else
    {
        front = (front+1) % MAX;
        --count;
    }
}
template<typename T>
const T& CircularQueue<T>::getFront() const
{
    if(count == 0)
        throw std::out_of_range("queue empty");
    else
    {
        return items[front];
    }
}
template<typename T>
const int& CircularQueue<T>::size() const
{
    return count;
}
