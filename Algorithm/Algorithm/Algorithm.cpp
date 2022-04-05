#include <iostream>
#include <vector>
#include <list>
using namespace std;

template<typename T> 
class Node
{
public:
    Node() : _prev(nullptr), _next(nullptr), _data(T())
    {
    }

    Node(const T& value) : _prev(nullptr), _next(nullptr), _data(value)
    {
    }

public:
    Node* _prev;
    Node* _next;
    T _data;
};

template<typename T>
class Iterator
{
public:
    Iterator() :_node(nullptr)
    {}

    Iterator(Node<T>* node) : _node(node)
    {}

    // ++it
    Iterator& operator++()
    {
        _node = _node->_next;
        return *this;
    }

    // it++
    Iterator& operator++(int)
    {
        Iterator<T> temp = *this;
        _node = _node->_next;
        return temp;
    }

    // --it
    Iterator& operator--()
    {
        _node = _node->_prev;
        return *this;
    }
    // it--
    Iterator& operator--(int)
    {
        Iterator<T> temp = *this;
        _node = _node->_prev;
        return temp;
    }

    bool operator ==(const Iterator& other)
    {
        return _node == other._node;
    }

    bool operator !=(const Iterator& other)
    {
        return _node != other._node;
    }

    // *it
    T& operator*()
    {
        return _node->_data;
    }


public:
    Node<T>* _node;
};


template<typename T>
class List
{
public:

    List() : _size(0)
    {
        _head = new Node<T>();
        _tail = new Node<T>();
        _head->_next = _tail;
        _tail->_prev = _head;
    }

    ~List()
    {
        while (_size > 0)
            pop_back();
     
        delete _head;
        delete _tail;
    }


    void push_back(const T& value)
    {
        AddNode(_tail, value);
    }

    void pop_back()
    {
        RemoveNode(_tail->_prev);
    }

    // 붙일때는 위치 노드 앞에 붙인다.
    Node<T>* AddNode(Node<T>* before, const T& value)
    {
        Node<T>* newNode = new Node<T>(value);
        Node<T>* prev = before->_prev;

        prev->_next = newNode;
        newNode->_prev = prev;

        newNode->_next = before;
        before->_prev = newNode;

        _size++;

        return newNode;
    }

    // 삭제할 노드 삭제 전에 그 노드의 앞과 두에 노드 연결
    Node<T>* RemoveNode(Node<T>* node)
    {
        Node<T>* prev = node->_prev;
        Node<T>* next = node->_next;

        prev->_next = next;
        next->_prev = prev;

        delete node;

        _size--;

        return next;
    }

    int Size() { return _size; }


public:
    using iterator = Iterator<T>;

    iterator begin() { return iterator(_head->_next); }
    iterator end() { return iterator(_tail); }

    iterator insert(iterator it, const T& value)
    {
        Node<T>* node =  AddNode(it._node, value);
        return iterator(node);
    }

    iterator erase(iterator it)
    {
        Node<T>* node =RemoveNode(it._node);
        return iterator(node);
    }

private:
    Node<T>* _head; // 머리 더미 데이터
    Node<T>* _tail; // 끝을 나타내는 더미 데이터
    int _size;
};



int main()
{
    List<int> li;

    List<int>::iterator eraseIt;

    for (int i = 0; i < 10; i++)
    {
        if (i == 5)
            eraseIt = li.insert(li.end(), i);
        else
            li.push_back(i);
    }

    li.pop_back();

    li.erase(eraseIt);

    for (List<int>::iterator it = li.begin(); it != li.end(); it++)
    {
        cout << (*it) << endl;
    }
}
