#include <iostream>
#include <optional>
#include <queue>


template <typename T>
class AVL
{
    private:
        class Node
        {
            public:
                T value;
                int height{-1};
                Node* left{nullptr};
                Node* right{nullptr};
                Node(T data) : value{std::move(data)}, height{0} {};
        };

        Node* root{nullptr};

        /// HELPERS
        Node* clear(Node* node);
        Node* copyTree(Node* node);
        Node* Rinsert(Node* node, const T& value);                                              /// recursive insert
        Node* RdeleteNode(Node* node, const T& value);                                          /// recursive delete
        void inorder(const typename AVL<T>::Node* node, void (*visitor)(const T&) = nullptr);   /// traversals - inorder postorder preorder levelorder
        void preorder(const typename AVL<T>::Node* node, void (*visitor)(const T&) = nullptr);
        void postorder(const typename AVL<T>::Node* node, void (*visitor)(const T&) = nullptr);

    public:
        AVL() = default;                                                                        /// default ctor
        AVL(const std::initializer_list<T>& list);                                              /// initializer_list ctor
                                                    
        AVL(const AVL<T>& src);                                                                 /// copy ctor
        AVL<T>& operator=(const AVL<T>& rhs);                                                   /// copy assignment operator

        AVL(AVL<T> && src) noexcept;                                                            /// move ctor 
        AVL<T>& operator=(AVL<T>&& rhs) noexcept;                                               /// move assignment operator

        ~AVL();                                                                                 /// destructor


        const T& getValue(typename AVL<T>::Node*) const;                                        /// getValue
                                                                                                /// clean
        std::optional<T> search(const T& key);                                                  /// search iterative

        int getHeight(Node* node);                                                              /// getHeight recursive
        int BalanceFactor(Node* node);                                                          /// Balance Factor

        Node* getMax(Node* node) const;                                                         /// max                                                        
        Node* getMin(Node* node) const;                                                         /// min

        Node* successor(Node* node) const;                                                       /// predecessor
        Node* predecessor(Node* node) const;                                                     /// successor

        Node* leftRotation(Node* node);
        Node* rightRotation(Node* node);

        template<typename U>
        void insert(U&& value);                                                                  /// rinsert                     

        template<typename U>                                                                                        
        void deleteNode(U&& value);                                                              /// rdelete
        
        void inorderTraversal(void (*visitor)(const T&) = [](const int& a) {std::cout << a << std::endl;});
        void preorderTraversal(void (*visitor)(const T&) = [](const int& a) {std::cout << a << std::endl;});
        void postorderTraversal(void (*visitor)(const T&) = [](const int& a) {std::cout << a << std::endl;});

        void levelOrder(void (*visitor)(const T&) = [](const int& a) {std::cout << a << std::endl;});

};


template<typename T>
AVL<T>::AVL(const std::initializer_list<T>& list)
{
    for(const T& item : list)
    { 
        insert(item);
    }
}

template<typename T>
AVL<T>::AVL(const AVL<T>& src) : root{copyTree(src.root)}
{
    std::cout << "copy ctor is called" << std::endl;
}

template<typename T>
AVL<T>& AVL<T>::operator=(const AVL<T>& rhs)
{
    AVL<T> tmp {rhs};
    std::swap(root,tmp.root);
    return *this;
}

template<typename T>
AVL<T>::AVL(AVL<T>&& src) noexcept
{
    std::cout << "move ctor is called" << std::endl;
    root = src.root;
    src.root = nullptr;
}

template<typename T>
AVL<T>& AVL<T>::operator=(AVL<T>&& rhs) noexcept
{
    if(&rhs == this)
        return *this;

    clear(root);
    root = rhs.root;
    rhs.root = nullptr;
    std::cout << "move assignment operator is called" << std::endl;
    return *this;
}

template<typename T>
typename AVL<T>::Node* AVL<T>::copyTree(Node* node)      ///deep copy
{ 
    if(!node)
        return nullptr;

    Node* newNode = new Node{node->value};
    newNode->left = copyTree(node->left);
    newNode->right = copyTree(node->right);

    return newNode;
}

template<typename T>
typename AVL<T>::Node* AVL<T>::clear(Node* node)
{
    if(!node)
        return nullptr;
    clear(node->left);
    clear(node->right);
    delete node;
    return nullptr;
}

template<typename T>
AVL<T>::~AVL()
{
    root = clear(root);
}

template<typename T>
const T& AVL<T>::getValue(typename AVL<T>::Node* node) const
{
    return node->value;
}

template<typename T>
std::optional<T> AVL<T>::search(const T& key)                                   ///iterative search
{
    Node* current = this->root;

    while(current != nullptr && key != current->value)
    {
        if(key < current->value)
            current = current->left;
        else
            current = current->right;
    }
    if(current)
        return current->value;
    else
        return std::nullopt;

}

template<typename T>                                                              //// get Height
int AVL<T>::getHeight(typename AVL<T>::Node* node)
{
    if(!node)
        return -1;
    return node->height;
}

template<typename T>                                                              //// balace factor
int AVL<T>::BalanceFactor(typename AVL<T>::Node* node)
{
    if(!node)
        return 0;
    return getHeight(node->left) - getHeight(node->right);
}

template<typename T>
typename AVL<T>::Node* AVL<T>::getMax(AVL<T>::Node* node) const
{
    Node* current = node;
    while(current)
        current= current->right;
    return current;
}

template<typename T>
typename AVL<T>::Node* AVL<T>::getMin(AVL<T>::Node* node) const
{
    Node* current = node;
    while(current)
        current = current->left;
    return current;
}

template<typename T>
typename AVL<T>::Node* AVL<T>::successor(Node* node) const
{
    if(!node)
        return nullptr;
    if(node->right)
        return getMin(node->right);
    Node* successor = nullptr;             /// successor will traverese when it's value is bigger than node's value
    Node* ancestor = this->root;           
    while(ancestor != node)
    {
        if(ancestor->value > node->value)   
        {
            successor = ancestor;
            ancestor = ancestor->left;
        }
        else
        {
            ancestor = ancestor->right;
        }
    }
    return successor;
}

template<typename T>
typename AVL<T>::Node* AVL<T>::predecessor(Node* node) const
{
    if(!node)
        return nullptr;
    if(node->left)
        return getMax(node->left);
    Node* predecessor = nullptr;             /// predecessor will traverese when it's value is less than node's value
    Node* ancestor = this->root;           
    while(ancestor != node)
    {
        if(ancestor->value < node->value)   
        {
            predecessor = ancestor;
            ancestor = ancestor->right;
        }
        else
        {
            ancestor = ancestor->left;
        }
    }
    return predecessor;
}

template<typename T>
typename AVL<T>::Node* AVL<T>::leftRotation(Node* node)
{
    Node* tmpNode = node->right;
    node->right = tmpNode->left;
    tmpNode->left = node;
    
    /// update heights of rotated nodes
    node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
    tmpNode->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;

    return tmpNode;
}

template<typename T>
typename AVL<T>::Node* AVL<T>::rightRotation(Node* node)
{
    Node* tmpNode = node->left;
    node->left = tmpNode->right;
    tmpNode->right = node;
    
    /// update heights of rotated nodes
    node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
    tmpNode->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;

    return tmpNode;
}


template<typename T>
typename AVL<T>::Node* AVL<T>::Rinsert(Node* node, const T& value)
{
    Node* newNode = new Node{value};
    if(!node)
        return newNode;

    if(value > node->value)
        node->right = Rinsert(node->right, value);
    else if(value < node->value)
        node->left = Rinsert(node->left, value);
    else 
        return node; ///equal keys are not alowed in this case
    
    ///update the height of newly inserted node's parent node
    node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;

    /// compute the balance factor of current node and it will go up recursivley and cumpute all ansetor nodes BF also
    int BF = BalanceFactor(node);

    if(BF < -1 && value > node->right->value)
    {
        return leftRotation(node);
    }
    if(BF < -1 && value < node->right->value)
    {
        node->right = rightRotation(node->right);
        return leftRotation(node);
    }
    if(BF > 1 && value < node->left->value)
    {
        return rightRotation(node);
    }
    if(BF > 1 && value > node->left->value)
    {
        node->left = leftRotation(node->left);
        return rightRotation(node);
    }
    return node;
}

template<typename T>
template<typename U>
void AVL<T>::insert(U&& value)
{
    root = Rinsert(root, std::forward<U>(value));
}

template<typename T>
typename AVL<T>::Node* AVL<T>::RdeleteNode(typename AVL<T>::Node* node, const T& value)
{
    if(!node)
    {
        std::cout << "node with this value doesn't exist" << std::endl;
        return nullptr;
    }

    if(node->value < value)
        node->right = RdeleteNode(node->right,value);
    else if(node->value > value)
        node->left = RdeleteNode(node->left,value);
    else
    {
        if(!node->left)
        {
            typename AVL<T>::Node* tmpNode = node->right;
            delete node;
            return tmpNode;
        }
        else if (!node->right)
        {
            typename AVL<T>::Node* tmpNode = node->left;
            delete node;
            return tmpNode;
        }
        else
        {
            typename AVL<T>::Node* tmpNode = getMin(node->right);
            node->value = tmpNode->value;
            node->right = RdeleteNode(node->left,tmpNode->value);
        }
    }

    ///update the height of newly DELETED node's parent 
    node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;

    /// compute the balance factor of current node and it will go up recursivley and compute all ansetor nodes BF also
    int BF = BalanceFactor(node);

    if(BF < -1 && BalanceFactor(node->right) <= 0)
    {
        return leftRotation(node);
    }
    if(BF < -1 && BalanceFactor(node->right) > 0)
    {
        node->right = rightRotation(node->right);
        return leftRotation(node);
    }
    if(BF > 1 && BalanceFactor(node->left) >= 0)
    {
        return rightRotation(node);
    }
    if(BF > 1 && BalanceFactor(node->left) < 0)
    {
        node->left = leftRotation(node->left);
        return rightRotation(node);
    }
    return node;   
}

template<typename T>
template<typename U>
void AVL<T>::deleteNode(U&& value)
{
    root = RdeleteNode(root, std::forward<U>(value));
}

template<typename T>
void AVL<T>::inorder(const typename AVL<T>::Node* node, void (*visitor)(const T&))
{
    if(!node)
        return;
    inorder(node->left, visitor);
    visitor(node->value);
    inorder(node->right, visitor);

}
template<typename T>
void AVL<T>::inorderTraversal(void (*visitor)(const T&))
{
    inorder(this->root,visitor);
}

template<typename T>
void AVL<T>::preorder(const typename AVL<T>::Node* node, void (*visitor)(const T&))
{
    if(!node)
        return;
    visitor(node->value);
    preorder(node->left, visitor);
    preorder(node->right, visitor);

}
template<typename T>
void AVL<T>::preorderTraversal(void (*visitor)(const T&))
{
    preorder(this->root,visitor);
}

template<typename T>
void AVL<T>::postorder(const typename AVL<T>::Node* node, void (*visitor)(const T&))
{
    if(!node)
        return;
    postorder(node->left, visitor);
    postorder(node->right, visitor);
    visitor(node->value);

}
template<typename T>
void AVL<T>::postorderTraversal(void (*visitor)(const T&))
{
    postorder(this->root,visitor);
}

template<typename T>
void AVL<T>::levelOrder(void (*visitor)(const T&))
{
    if(!root)
        return;
    std::queue<Node*> q;
    q.push(root);

    while(!q.empty())
    {
        Node* node = q.front();
        q.pop();
        visitor(node->value);
        if(node->left)
            q.push(node->left);
        if(node->right)
            q.push(node->right);
    }
}