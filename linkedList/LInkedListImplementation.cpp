#include <iostream>
#include <initializer_list>
#include <list>


template<typename T>
class List
{
    private:
        class Node
        {
            public:
                T data;
                Node* next{nullptr};
                Node(T data = T(), Node* next = nullptr)   ///ctor
                {
                    this->data = data;
                    this->next = next;
                }
        };

        uint32_t size{0};
        Node* head{nullptr};

        void inRange(const uint32_t& index)
        {
            if(index > size || index < 0)
                throw std::out_of_range { "index is out of range"};
        }
        Node* find(const uint32_t& index);

    public:
        List();                                                 /// default ctor with default parameters
        List(const List<T>& src);                               /// copy ctor
        explicit List(const std::initializer_list<T>& init);    /// ctor initializer-List
        ~List();                                                /// dtor

        T& operator[](const uint32_t& index);                   ///subscript operator
        const Node* getHead() const;

        void push_back(const T& data);
        void push_front(const T& data);
        
        void pop_back();
        void pop_front();

        void insert(const T& data,const uint32_t& index);
        void remove(const uint32_t& index);
        void clear();

        void printList();
        const uint32_t& Size() const;

};

template<typename T>
List<T>::List()
{
    size = 0U;
    head = nullptr;
}

template<typename T>
List<T>::List(const std::initializer_list<T>&  init)
{
    Node* head = new Node{};
    Node* curr = head;
    size = init.size();
    
    typename std::initializer_list<T>::iterator iter = init.begin();
    while(iter != init.end()-1)
    {
        curr->data = *iter;
        curr->next = new Node{};
        curr = curr->next;
        ++iter;
    }
    curr->data = *iter;
    this->head = head;
}

template<typename T>
List<T>::~List()
{
    clear();
}
template<typename T>
const typename List<T>::Node* List<T>::getHead() const
{
    return head;
}

template<typename T>
T& List<T>::operator[](const uint32_t& index)
{
    inRange(index);
    return find(index)->data;
    throw std::runtime_error("No valid return value found.");
}

template<typename T>
typename List<T>::Node* List<T>::find(const uint32_t& index)
{
    inRange(index);
    uint32_t count = 0U;
    Node* current = this->head;
    while(current != nullptr)
    {
        if(count == index)
            return current;
        current = current->next;
        ++count;
    }
    throw std::runtime_error("No valid return value found.");
}

template<typename T>
void List<T>::push_back(const T& data)
{
    if(this->head == nullptr)
    {
        this->head = new Node{data};
    }
    else
    {
        Node* current = this->head;
        while(current->next != nullptr)
        {
            current = current->next;
        }
        current->next = new Node{data};
    }
    ++size;  
}

template<typename T>
void List<T>::push_front(const T& data)
{
    this->head = new Node(data,this->head);
    ++size;
}

template<typename T>
void List<T>::pop_front()
{
    if(head != nullptr)
    {
        Node* current = this->head;
        this->head = head->next;
        current->next = nullptr;
        delete current;
        current = nullptr;
        --size;  
    }
    else
    {
        throw std::range_error("list is empty");
    } 
}

template<typename T>
void List<T>::insert(const T& data,const uint32_t& index)
{
    inRange(index);
    Node* newNode = new Node{data};
    if(newNode == nullptr)
    {
        throw std::bad_alloc{};
    }
    else
    {
        if(index == 0)
        {
            newNode->next = this->head;
            this->head = newNode;
            ++size;
        }
        else
        {
            Node* prev = find(index-1);
            newNode->next = prev->next;
            prev->next = newNode;
        }
    }

}
template<typename T>
void List<T>::remove(const uint32_t& index)
{
    inRange();
    Node* current {nullptr};
    if(index == 0)
    {
        current = this->head;
        this->head = current->next;
    }
    else
    {
        Node* prev = find(index-1);
        current = prev->next;
        prev->next = current->next;
    }
    current->next = nullptr;
    delete current;
    current = nullptr;
}

template<typename T>
void List<T>::clear()
{
    while(size)
    {
        pop_front();
    }
}

template<typename T>
void List<T>::printList()
{
    if(head != nullptr)
    {
        Node* curr = this->head;
        while(curr != nullptr)
        {
            std::cout << curr->data << std::endl; 
            curr = curr->next;       
        }
    }
    else
    {
        throw std::range_error("list is empty");
    }
}   

template<typename T>
const uint32_t& List<T>::Size() const {return size;}

