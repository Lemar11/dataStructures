#include <iostream>

template<typename T>
class Stack
{
    static const uint32_t MAX = 1000;

    protected:
        T array[MAX];
        int mtop{-1};
    public:
        Stack(){};
        ~Stack(){};
        explicit Stack(const std::initializer_list<T>& init)
        {
            for(const T& elem : init)
            {
                array[++mtop] = elem;
            }
        }         
        Stack(const Stack& src)                                       
        {
            mtop = src.mtop;
            for(uint32_t i = 0; i < src.mtop; ++i)
            {
                array[i] = src.array[i];
            }
        }
        bool isEmpty()
        {
            return mtop < 0;
        }
        void push(const T& var)
        {
            if(mtop >= MAX-1)
                throw std::runtime_error("underflow");
            array[++mtop] = var;
        }
        void pop()
        {
            if(mtop < 0)
                throw std::runtime_error("underflow");
            else
                --mtop;
        }
        const T& top() const
        {
            if(mtop < 0)
                throw std::runtime_error("underflow");
            return array[mtop];
        }
        uint32_t size() const
        {
            return mtop+1;
        }
        Stack<T>& operator=(const Stack<T>& rhs)
        {
            if(this == &rhs)
                return *this;
            mtop = rhs.mtop;
            for(uint32_t i = 0; i <= rhs.mtop; ++i)
            {
                push(rhs.array[i]);
            }
            return *this;
        }  
        void swap(Stack<T>& rhs) noexcept
        {
            uint32_t i = 0;
            uint32_t j = 0;
            while(i < size() && i < rhs.size())
            {    
                std::swap(array[i],rhs.array[i]);
                ++i;
                ++j;
            }
            while(i < size())
            {
                rhs.array[i] = array[i];
                ++i;
            }
            while(j < size())
            {
                array[j] = rhs.array[j];
                ++j;
            }
            std::swap(mtop,rhs.mtop);
        } 
};

