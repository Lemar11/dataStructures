#pragma once
#include <iostream>
#include <stdexcept>
#include <utility>

namespace DLL{
    template<typename T>
    class Dll
    {
        private:
            class Node
            {
                public:
                    T m_data;
                    Node* m_next{nullptr};
                    Node* m_prev{nullptr};
                    template<typename U>
                    Node(U&& value = U{}) : m_data{std::forward<U>(value)}{};   ///perfect Forwarding
            };

            size_t m_itemCount{0};
            Node* head{nullptr};
            Node* tail{nullptr};

            void validRange(size_t index) const;
            Node* mergeTwoLists(Node* head1, Node* head2);   ///for mergeSort
            Node* mergeSort(Node* head);
            Node* getMiddleNode(Node* head);                 ///for mergeSort
            void setTail();

        public:
            Dll();
            Dll(std::initializer_list<T> list);
            Dll(const Dll<T>& src);
            Dll<T>& operator=(const Dll<T>& rhs);

            Dll(Dll<T>&& src) noexcept;
            Dll<T>& operator=(Dll<T>&& rhs) noexcept;

            template<typename U>
            void push_back(U&& value);
            void pop_back();
            template<typename U>
            void push_front(U&& value);
            void pop_front();
            template<typename U>
            void insert(size_t index,U&& value);
            void remove(size_t index);
            void reverse();                 /// iterative
            Node* reverse(Node* head);      /// recursive
            void merge(Dll<T>&& rhs);

            void sort();     ////delegating to private mergesort
            void clear();

            static bool hasCycle(Node* head);
            Node* getCycleStartNode(Node* head);
            
            bool isempty() const;
            void print() const;
            void printFromTail() const;
            size_t size() const;
            Node* getHead() const;

            ~Dll(){clear();}                   

    };

    template<typename T>
    void DLL::Dll<T>::validRange(size_t index) const
    {
        if(index >= m_itemCount)
        {
            throw std::range_error("index is out of bounds");
        }
    }

    template<typename T>                                                  /// default ctor
    DLL::Dll<T>::Dll() : m_itemCount{0},
                         head{nullptr},
                         tail{nullptr}
    {}

    template<typename T>
    DLL::Dll<T>::Dll(const Dll<T>& src) : m_itemCount{src.m_itemCount}    /// copy ctor
    {
        Node* current = src.head;
        try
        {
            while(current != nullptr)
            {
                push_back(current->m_data);
                current = current->m_next;
            }
        }
        catch(const std::bad_alloc& e)
        {
            std::cerr << e.what() << '\n';
            clear();
            throw;
        }
    }

    template<typename T>
    DLL::Dll<T>::Dll(std::initializer_list<T> list)                         /// initializer list ctor
    {
        try
        {
            auto iter = list.begin();
            head = new Node{*iter};
            Node* current = head;
            m_itemCount = list.size();
            ++iter;
            while(iter != list.end())
            {
                current->m_next = new Node{*iter};
                current->m_next->m_prev = current;
                current = current->m_next;
                ++iter;
            }
            tail = current;
        }
        catch (const std::bad_alloc& e)
        {
            clear();
            throw;
        }
    }

    template<typename T>
    DLL::Dll<T>& DLL::Dll<T>::operator=(const DLL::Dll<T>& rhs)             ///copy assignemnt 
    {
        if(this == &rhs)
            return *this;

        Dll<T> tmp{rhs};
        m_itemCount = tmp.m_itemCount;
        clear();
        head = tmp.head;
        tail = tmp.tail;
        tmp.head = nullptr;
        return *this;
    }

    template<typename T>
    DLL::Dll<T>::Dll(DLL::Dll<T>&& src) noexcept                            ///move ctor
                    : m_itemCount{src.m_itemCount},
                    head{std::exchange(src.head,nullptr)},
                    tail{std::exchange(src.tail,nullptr)}                           
    {}

    template<typename T>
    DLL::Dll<T>& DLL::Dll<T>::operator=(DLL::Dll<T>&& rhs) noexcept        ///move assignment 
    {
        clear();
        m_itemCount = rhs.m_itemCount;
        head = rhs.head;
        tail = rhs.tail;
        rhs.head = nullptr;
        return *this;
    }

    template<typename T>
    template<typename U>
    void DLL::Dll<T>::insert(size_t index,U&& value)
    {
        Node* newNode = new Node{std::forward<U>(value)};
        if(index == 0)
        {
            push_front(std::forward<U>(value));
        }
        else
        {
            validRange(index);
            Node* current = head;
            size_t count = 0;
            while(count != index-1)
            {
                current = current->m_next;
                ++count;
            }
            newNode->m_next = current->m_next;
            current->m_next->m_prev = newNode;
            current->m_next = newNode;
            newNode->m_prev = current;
            ++m_itemCount;
        }
    }

    template<typename T>
    template<typename U>
    void DLL::Dll<T>::push_back(U&& value)
    {
        Node* newNode = new Node{std::forward<U>(value)};
        if(isempty())
        {
            head = newNode;
        }
        else
        {
            Node* current = head;
            while(current->m_next != nullptr)
            {
                current = current->m_next;
            }
            current->m_next = newNode;
            newNode->m_prev = current;
        }
        tail = newNode; 
        ++m_itemCount;
    }

    template<typename T>
    void DLL::Dll<T>::pop_back()
    {
        if(!isempty())
        {
            if(head == tail)
            {
                tail = nullptr;
                delete head;
                head = nullptr;
            }
            else
            {
                tail = tail->m_prev;
                delete tail->m_next;
                tail->m_next = nullptr;
                
            }
            --m_itemCount;
        }
    }

    template<typename T>
    template<typename U>
    void DLL::Dll<T>::push_front(U&& value)
    {
        Node* newnode = new Node{std::forward<U>(value)};
        if(!isempty())
        {
            head->m_prev = newnode;
            newnode->m_next = head;
            head = head->m_prev;
        }
        else
        {
            head = newnode;
            tail = head;
        }
        ++m_itemCount;
    }

    template<typename T>
    void DLL::Dll<T>::pop_front()
    {
        if(!isempty())
        {
            if(head == tail)
            {
                head = tail = head->m_next;
                return;
            }
            Node* current = head;
            head = current->m_next;
            delete current;
            --m_itemCount;
        }
    }

    template<typename T>
    void DLL::Dll<T>::remove(size_t index)
    {
        if(!isempty())
        {
            validRange(index);
            Node* current = head;
            int count = 0;
            if(index == 0)
            {
                pop_front();
                return;
            }
            if(index == m_itemCount-1)
            {
                pop_back();
                return;
            }
            while (count != index)
            {
                current = current->m_next;
                ++count;
            }
            current->m_prev->m_next = current->m_next;
            current->m_next->m_prev = current->m_prev;
            delete current;
            --m_itemCount;        
        }
    }


    template<typename T>
    void DLL::Dll<T>::reverse()    // iterative reverse implementation like for sigly linked list
    {
        if(head != nullptr)
        {
            Node* prev = nullptr;
            Node* current = head;
            tail = head;
            while(current != nullptr)
            {
                Node* next = current->m_next;
                current->m_next = prev;
                current->m_prev = next;
                prev = current;
                current = next;
            }
            head = prev;
        }
    }

    template<typename T>
    typename DLL::Dll<T>::Node* Dll<T>::reverse(DLL::Dll<T>::Node* head)   // recursive reverse implementation like for sigly linked list
    {
        if(head->m_next == nullptr || head == nullptr)
        {
            this->tail = this->head;
            this->head = head;
            return head;
        }
        Node* next = reverse(head->m_next);
        
        next->m_prev = head->m_next->m_next; //
        next->m_next = head;
        head->m_prev = next; //
        head->m_next = nullptr;
        return head;
    }

    template<typename T>
    typename DLL::Dll<T>::Node* Dll<T>::getMiddleNode(DLL::Dll<T>::Node* head)     ///get middle node in 1/2n time complexity
    {
        if(!isempty())
        {
            Node* fast = head;
            Node* slow = head;
            while(fast->m_next != nullptr && fast->m_next->m_next != nullptr)
            {
                slow = slow->m_next;
                fast = fast->m_next->m_next;
            }
            return slow;
        }
        return head;

    }

    template<typename T>
    bool DLL::Dll<T>::hasCycle(DLL::Dll<T>::Node* head)
    {
        if(head != nullptr)
        {
            Node* fast = head;
            Node* slow = head;
            while(fast != nullptr && fast->m_next != nullptr)
            {
                fast = fast->m_next->m_next;
                slow = slow->m_next;
                if(fast == slow)
                    return true;
            }
            return false;
        }
        std::cout << "list is empty" << std::endl;
        return false;
    }

    template<typename T>
    typename DLL::Dll<T>::Node* DLL::Dll<T>::getCycleStartNode(DLL::Dll<T>::Node* head)
    {
        if(head != nullptr)
        {
            Node* fast = head;
            Node* slow = head;
            while(fast != nullptr && fast->m_next != nullptr)
            {
                fast = fast->m_next->m_next;
                slow = slow->m_next;
                if(fast == slow)
                {
                    while(head != slow)
                    {
                        head = head->m_next;
                        slow = slow->m_next;
                    }
                    return head;
                }     
            }
            return nullptr;
        }
        std::cout << "list is empty" << std::endl;
        return nullptr;
    }


    template<typename T>
    void DLL::Dll<T>::merge(DLL::Dll<T>&& rhs)  
    {
        if(rhs.head == nullptr)
            return;

        Node* lhs_head = this->head;
        if(this->head->m_data <= rhs.head->m_data)
        {
            lhs_head = lhs_head->m_next;
        }
        else
        {
            this->head = rhs.head;
            rhs.head = rhs.head->m_next;
        }

        Node* current = this->head;
        while(lhs_head != nullptr && rhs.head != nullptr)
        {
            if(rhs.head->m_data <= lhs_head->m_data)
            {
                current->m_next =  rhs.head;
                rhs.head->m_prev = current;
                rhs.head = rhs.head->m_next;
            }
            else
            {
                current->m_next = lhs_head;
                lhs_head->m_prev = current;
                lhs_head = lhs_head->m_next;
            }
            current = current->m_next;
        }

        if(!lhs_head)
        {
            current->m_next = rhs.head;
            rhs.head->m_prev = current;
            this->tail = rhs.head;
        }
        else
        {
            current->m_next = lhs_head;
            lhs_head->m_prev = current;
        }
        rhs.head = nullptr;
        rhs.tail = nullptr;
    }

    template<typename T>
    typename DLL::Dll<T>::Node* Dll<T>::mergeTwoLists(DLL::Dll<T>::Node* h1, DLL::Dll<T>::Node* h2)   ///recursive
    {
        if(!h1){ return h2; }
        if(!h2){ return h1; }
        if(h1->m_data <= h2->m_data)
        {
            h1->m_next = mergeTwoLists(h1->m_next,h2);
            h1->m_next->m_prev = h1;
            return h1;
        }
        else
        {
            h2->m_next = mergeTwoLists(h1,h2->m_next);
            h2->m_next->m_prev = h2;
            return h2;
        }

    }

    template<typename T>
    typename DLL::Dll<T>::Node* DLL::Dll<T>::mergeSort(DLL::Dll<T>::Node* head)
    {
        if(!head || !head->m_next)
        {
            return head;
        }
        DLL::Dll<T>::Node* mid = getMiddleNode(head);
        DLL::Dll<T>::Node* tmp = mid;
        mid = mid -> m_next;
        tmp->m_next->m_prev = nullptr;
        tmp->m_next = nullptr;
        head = mergeSort(head);
        mid = mergeSort(mid);
        return  mergeTwoLists(head,mid);
    }

    template<typename T>
    void DLL::Dll<T>::sort()
    {
        this->head = mergeSort(this->head);
        setTail();
    }

    template<typename T>
    void DLL::Dll<T>::setTail()
    {
        Node* current = head;
        while(current->m_next != nullptr)
        {
            current = current->m_next;
        }
        this->tail = current;
    }

    template<typename T>
    bool DLL::Dll<T>::isempty() const
    {
        if(head == nullptr)
            return true;
        return false;
    }

    template<typename T>
    void DLL::Dll<T>::print() const
    {
        Node* current = head;
        while(current != nullptr)
        {
            std::cout << current->m_data << ", ";
            current = current->m_next;
        }
        std::cout << std::endl;
    }
    template<typename T>
    void DLL::Dll<T>::printFromTail() const
    {
        Node* current = tail;
        while(current != nullptr)
        {
            std::cout << current->m_data << ", ";
            current = current->m_prev;
        }
        std::cout << std::endl;
    }

    template<typename T>
    void DLL::Dll<T>::clear()
    {
        Node* current = head;
        while(current != nullptr)
        {
            current = current->m_next;
            delete head;
            head = current;
        }
        tail = nullptr;
        m_itemCount = 0;
    }

    template<typename T>
    size_t DLL::Dll<T>::size() const
    {
        return m_itemCount;
    }

    template<typename T>
    typename DLL::Dll<T>::Node* DLL::Dll<T>::getHead() const
    {
        return this->head;
    }
}