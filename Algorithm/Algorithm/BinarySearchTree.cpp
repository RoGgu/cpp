#include "BinarySearchTree.h"
#include <iostream>
#include <Windows.h>
using namespace std;

// ConsoleHelper에 함수 빌려옴
void SetCursorPosition(int x, int y)
{
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    ::SetConsoleCursorPosition(output, pos);
}


// 트리 이쁘게 나타내는 함수
void BinarySearchTree::Print(Node* node, int x, int y)
{
    if (node == nullptr)
        return;

    SetCursorPosition(x, y);

    cout << node->key;
    Print(node->left, x - (5 / (y + 1)), y+1);
    Print(node->right, x + (5 / (y + 1)), y+1);
}

void BinarySearchTree::Print_Inorder(Node* node)
{
    // 전위 순회 (preorder Traverse)
    // 중위 순회 (inorder)
    // 후위 순회 (postorder)

    //   [중]
    // [좌] [우]

    if (node == nullptr)
        return;

    // 전위 : [중]이 앞에 온다.     //  [중][좌][우]
    // 중위 : [중]이 중간에 온다.   //  [좌][중][우]
    // 후위 : [중]이 마지막에 온다. //  [좌][우][중]

    cout << node->key << endl; // 전위
    Print_Inorder(node->left);
    //cout << node->key << endl; // 중위
    Print_Inorder(node->right);
    //cout << node->key << endl; // 후위
    
}

Node* BinarySearchTree::Search(Node* node, int key)
{
    if (node == nullptr || key == node->key)
        return node;
    
    // 재귀로 호출 - 일반적으로 재귀 방식으로 많이 씅미
    if (key < node->key)
        return Search(node->left, key);
    else
        return Search(node->right, key);
}

Node* BinarySearchTree::Search2(Node* node, int key)
{
    // 재귀 안 쓰는 방식
    while (node && key != node->key)
    {
        if (key < node->key)
            node = node->left;
        else
            node = node->right;
    }

    return node;
}

Node* BinarySearchTree::Min(Node* node)
{
    // 이중 트리중 가장 작은 값은 왼쪽 노드로 계속 타고 들어가면 나온다
    while (node->left != nullptr)
    {
        node = node->left;
    }
    return node;
}

Node* BinarySearchTree::Max(Node* node)
{
    // 가장 큰 값은 오른쪽으로 노드 쭉 타고가면 나온다.
    while (node->right != nullptr)
    {
        node = node->right;
    }
    return node;
}

Node* BinarySearchTree::Next(Node* node)
{
    // 현재 노드에 오른쪽 자식이 있으면 그 오른쪽 노드에서부터 최솟값이 자신다음 가장 큰 숫자
    if (node->right)
        return Min(node->right);

    Node* parent = node->parent;

    while (parent != nullptr && parent->right == node) // 자기 위에 부모가 자신을 오른쪽 노드로 들고있으면 더 작은 부모니까 더큰 부모 찾아서 올라감
    {
        node = parent;
        parent = node->parent;
    }

    return parent; // 그렇게 찾은 부모가 자신보다 큰 다음 숫자
}

void BinarySearchTree::Insert(int key)
{
    Node* newNode = new Node();
    newNode->key = key;

    if (_root == nullptr) // 루트노드 즉 노드가 아무것도 없으면 새로만든 노드가 root노드
    {
        _root = newNode;
        return;
    }
    

    Node* node = _root;
    Node* parent = nullptr;

    while (node) // 부모 노드 찾기
    {
        parent = node;

        if (key < node->key)
            node = node->left;
        else
            node = node->right;
    }

    newNode->parent = parent; // 부모노드 세팅

    if (key < parent->key) // 부모보다 작으면 왼쪽 크면 오른쪽에 붙인다.
        parent->left = newNode;
    else
        parent->right = newNode;

}

void BinarySearchTree::Delete(int key)
{
	Node* deleteNode = Search(_root, key);
	Delete(deleteNode);

}

void BinarySearchTree::Delete(Node* node)
{
    // 삭제할 경우 세 가지 경우를 생각해야 한다.
    // 1) 자식이 하나도 없을경우
    // 2) 자식이 하나 있을경우
    // 3) 자식이 둘 다 있을경우

	if (node == nullptr)
		return;

	if (node->left == nullptr) // 자식이 하나만 있거나 아예 없거나 두 케이스에 사용가능
	{
        // right도 nullptr 즉 자식노드가 하나도 없는애면 nullptr로 세팅될꺼고
        // right가 있다면 현재 node 위치에 세팅된다.
        Replace(node, node->right);
	}
    else if (node->right == nullptr)
    {
        // 위와 반대로 왼쪽을 세팅해주거나 nullptr 세팅
        Replace(node, node->left);
    }
    else // 자식이 둘 다 있을경우 
    {
        // 다음 데이터 찾기
        Node* next = Next(node);
        node->key = next->key; // 그 데이터를 현재 노드에 세팅
        Delete(next); // 바꿔지기 하고 남은 원래있던 자리 데이터 삭제
    }

}

// u 서브트리를 v 서브트리로 교체
// 그리고 delete u
void BinarySearchTree::Replace(Node* u, Node* v)
{
    if (u->parent == nullptr)
        _root = v;
    else if (u == u->parent->left) // 부모가 삭제될 u를 어느쪽에 들고있엇는지에 따라서 v를 붙여줌
        u->parent->left = v;
    else
        u->parent->right = v;

    if (v != nullptr)
        v->parent = u->parent;

    delete u;
}
