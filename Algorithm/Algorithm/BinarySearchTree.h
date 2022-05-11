#pragma once

enum class Color
{
    Red = 0,
    Black = 1
};


struct Node
{
    Node* parent = nullptr;
    Node* left = nullptr;
    Node* right = nullptr;
    int key = {};
    Color color = Color::Black; // 기본 색상은 블랙으로 한다.
};

// Red-Black Tree
// 1) 모든 노드는 Red or Black
// 2) Root는 Black
// 3) Leaf(NIL)는 Black - 기존에 없으면 nullptr로 했는데 대신 _nil노드 사용
// 4) Red 노드의 자식은 Black (연속해서 Red-Red X)
// 5) 각 노드로부터 ~ 리프까지 가는 경로들은 모드 같은 수의 Black

class BinarySearchTree
{
public:

    BinarySearchTree();
    ~BinarySearchTree();

    void    Print() { Print(_root, 10, 0); }
    void    Print(Node* node, int x, int y);


    Node*   Search(Node* node, int key);

    Node*   Min(Node* node);
    Node*   Max(Node* node);
    Node*   Next(Node* node);

    void    Insert(int key);
    void    InsertFixup(Node* node); // 레드블랙트리 잘 되는지 체크

    void    Delete(int key);
    void    Delete(Node* node);
    
    void    Replace(Node* u, Node* v); 

    // Red-Black Tree
    void    LeftRotate(Node* node); // 왼쪽 회전
    void    RightRotate(Node* node); // 오른쪽 회전

private:
    Node* _root = nullptr;
    Node* _nil = nullptr; // 데이터 없음을 나타내는 노드 - nullptr대신 _nil로 체크한다.
};

