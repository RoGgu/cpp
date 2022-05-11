#include "BinarySearchTree.h"
#include <iostream>
#include <Windows.h>
using namespace std;

enum class ConsoleColor // 콘솔 커서 색
{
    BLACK = 0,
    RED = FOREGROUND_RED,
    GREEN = FOREGROUND_GREEN,
    BLUE = FOREGROUND_BLUE,
    YELLOW = RED | GREEN,
    WHITE = RED | GREEN | BLUE,
};

void SetCursorColor(ConsoleColor color) // 커서 색깔 지정 ConsoleHelper에서 가져옴
{
    HANDLE output = ::GetStdHandle(STD_OUTPUT_HANDLE);
    ::SetConsoleTextAttribute(output, static_cast<SHORT>(color));
}


// ConsoleHelper에 함수 빌려옴
void SetCursorPosition(int x, int y)
{
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    ::SetConsoleCursorPosition(output, pos);
}


BinarySearchTree::BinarySearchTree()
{
    _nil = new Node(); // nullptr대신 _null로 표시
    _root = _nil; // 처음 만들땐 root가 없으니 nullptr인데 대신 _nil로 표시
}

BinarySearchTree::~BinarySearchTree()
{
    delete _nil;
}

// 트리 이쁘게 나타내는 함수
void BinarySearchTree::Print(Node* node, int x, int y)
{
    if (node == _nil)
        return;

    SetCursorPosition(x, y);

    if (node->color == Color::Black) // 노드 색에 따라서 커서 색깔 변경
        SetCursorColor(ConsoleColor::BLUE);
    else
        SetCursorColor(ConsoleColor::RED);


    cout << node->key;
    Print(node->left, x - (5 / (y + 1)), y+1);
    Print(node->right, x + (5 / (y + 1)), y+1);

    SetCursorColor(ConsoleColor::WHITE); // 출력하고 기본 WHITE로 설정
}



Node* BinarySearchTree::Search(Node* node, int key)
{
    // nullptr 대신 _nil로 비어있는 표시 체크
    if (node == _nil || key == node->key)
        return node;
    
    // 재귀로 호출 - 일반적으로 재귀 방식으로 많이 씅미
    if (key < node->key)
        return Search(node->left, key);
    else
        return Search(node->right, key);
}


Node* BinarySearchTree::Min(Node* node)
{
    // 이중 트리중 가장 작은 값은 왼쪽 노드로 계속 타고 들어가면 나온다
    while (node->left != _nil)
    {
        node = node->left;
    }
    return node;
}

Node* BinarySearchTree::Max(Node* node)
{
    // 가장 큰 값은 오른쪽으로 노드 쭉 타고가면 나온다.
    while (node->right != _nil)
    {
        node = node->right;
    }
    return node;
}

Node* BinarySearchTree::Next(Node* node)
{
    // 현재 노드에 오른쪽 자식이 있으면 그 오른쪽 노드에서부터 최솟값이 자신다음 가장 큰 숫자
    if (node->right != _nil)
        return Min(node->right);

    Node* parent = node->parent;

    while (parent != _nil && parent->right == node) // 자기 위에 부모가 자신을 오른쪽 노드로 들고있으면 더 작은 부모니까 더큰 부모 찾아서 올라감
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
    

    Node* node = _root;
    Node* parent = _nil;

    while (node != _nil) // 부모 노드 찾기
    {
        parent = node;

        if (key < node->key)
            node = node->left;
        else
            node = node->right;
    }

    newNode->parent = parent; // 부모노드 세팅

    if (parent == _nil) // 부모 노드가 없으면 자신이 root노드
        _root = newNode;
    else if (key < parent->key) // 부모보다 작으면 왼쪽 크면 오른쪽에 붙인다.
        parent->left = newNode;
    else
        parent->right = newNode;

    // 레드 블랙 트리 검사
    newNode->left = _nil;
    newNode->right = _nil;
    newNode->color = Color::Red; // 새로 추가하는 노드 색상은 RED
    InsertFixup(newNode); // 레드블랙트리 체크해서 만들어주는 함수
}

void BinarySearchTree::InsertFixup(Node* node)
{
    // 규칙
    // 1) p = red, uncle = red
    //  -> p = black, uncle = black, pp = red로 바꿈
    // 2) p = red, uncle = black (triangle)
    //  -> 회전을 통해 case 3으로 바꿈
    // 3) p = red, uncle = black (list)
    //  -> 색상 변경 + 회전

    while (node->parent->color == Color::Red)
    {   
        //            [pp(B)]
        //      [p(R)]      [u(?)]
        //      ?[n(R)] // 왼쪽, 오른쪽인진 모름
        if (node->parent == node->parent->parent->left) // 부모가 조상에 왼쪽이면 형제노드는 조상의 오른쪽 자식
        {
            // 부모에 형제 노드 찾는다.
            Node* uncle = node->parent->parent->right;
            if (uncle->color == Color::Red)
            {
                //            [pp(B)]
                //      [p(R)]      [u(R)]
                //      ?[n(R)] // 왼쪽인지 오른쪽인지 모름
                // 부모 레드고 사촌까지 레드면
                // 부모 사촌 블랙으로 바꿔주고, 부모부모는 레드로 바꿔준다. -가장 쉬운 경우
                //            [pp(R)]
                //      [p(B)]      [u(B)]
                //      ?[n(R)]
                node->parent->color = Color::Black; // p
                uncle->color = Color::Black; // u
                node->parent->parent->color = Color::Red; // pp
                node = node->parent->parent; // 부모 부모부모 까지체크 하기 위해 node 새로세팅 - while문 돌면서 체크께속한다.
            }
            else // 사촌이 Red가 아니고 Black일 경우
            {
                //            [pp(B)]
                //      [p(R)]      [u(B)]
                //        [n(R)] // 자식이 오른쪽 ㄱ자모양
                // Triangle타입이면 list타입으로 바꾸기 위해
                // 레프트 로테이트 왼쪽으로 돌려줌
                //            [pp(B)]
                //      [p(R)]      [u(B)]
                //    [n(R)]
                // case 2를 case 3로 바꿈
                if (node == node->parent->right) // Triangle 타입
                {
                    node = node->parent;
                    LeftRotate(node);
                }

                // List 타입
                //            [pp(B)]
                //      [p(R)]      [u(B)]
                //    [n(R)]
                // 색상 변경
                //            [pp(R)]
                //      [p(B)]      [u(B)]
                //    [n(R)]
                // 오른쪽 회전
                //            [p(B)]
                //      [u(R)]      [pp(R)]
                //                       [u(B)]
                node->parent->color = Color::Black;
                node->parent->parent->color = Color::Red;
                RightRotate(node->parent->parent);
            }
        }
        else // 위에랑 반대 부모 노드가 오른쪽에 있을 경우
        {
            //            [pp(B)]
            //       [u(?)]    [p(R)]  
            //                    [n(R)] // 왼쪽,오른쪽인진 모름
            // 부모에 형제 노드 찾는다.
            Node* uncle = node->parent->parent->left;
            if (uncle->color == Color::Red)
            {
                //            [pp(B)]
                //      [u(R)]      [r(R)]
                //                       [n(R)]
                // 부모 레드고 사촌까지 레드면
                // 부모 사촌 블랙으로 바꿔주고, 부모부모는 레드로 바꿔준다.
                //            [pp(R)]
                //      [u(B)]      [p(B)]
                //                      [n(R)]
                node->parent->color = Color::Black; // p
                uncle->color = Color::Black; // u
                node->parent->parent->color = Color::Red; // pp
                node = node->parent->parent; // 부모 부모부모 까지체크 하기 위해 node 새로세팅
            }
            else
            {
                //            [pp(B)]
                //      [u(B)]      [p(R)]
                //                [n(R)]
                // Triangle타입이면 list타입으로 바꾸기 위해
                // 오른쪽으로 돌려줌
                //            [pp(B)]
                //      [u(B)]      [p(R)]
                //                      [n(R)]
                // case 2를 case 3로 바꿈
                if (node == node->parent->left) // Triangle 타입
                {
                    node = node->parent;
                    RightRotate(node);
                }

                // List 타입
                //            [pp(B)]
                //      [u(B)]      [p(R)]
                //                      [n(R)]
                // 색상 변경
                //            [pp(R)]
                //      [u(B)]      [p(B)]
                //                      [n(R)]
                // 왼쪽 회전
                //            [p(B)]
                //      [pp(R)]      [n(R)]
                //    [u(B)]                  
                node->parent->color = Color::Black;
                node->parent->parent->color = Color::Red;
                LeftRotate(node->parent->parent);
            }
        }
    }

    // 루트 컬러는 항상 블랙
    _root->color = Color::Black;
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


//       [x]
//   [1]     [y]
//         [2] [3]

//  위에서 아래로 가면 왼쪽 회전 (왼쪽으로 돌리고 [2]가 x 오른쪽 자식으로 들어감)

//       [y]
//   [x]     [3]
// [1] [2]
void BinarySearchTree::LeftRotate(Node* x)
{
    Node* y = x->right;

    x->right = y->left; // [2]
    
    if(y->left != _nil) // 비어있지 않으면 부모까지 세팅
        y->left->parent = x; // [2] 번 부모 x로세팅

    // 기존 x의 부모를 y의 부모로 세팅
    y->parent = x->parent;

    // 기존 x의 부모노드에 자식으로 y노드 세팅
    if (x->parent == _nil) // x가 root였으면 y를 root로 설정
        _root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    // x를 y왼쪽자식으로 둔다.
    y->left = x;
    x->parent = y;
}


//       [y]
//   [x]     [3]
// [1] [2]

// 위에서 아래로 오른쪽 회전 (오른쪽으로 돌리고 [2]가 y왼쪽 자식으로 들어감)

//       [x]
//   [1]     [y]
//         [2] [3]
void BinarySearchTree::RightRotate(Node* y)
{
    Node* x = y->left;

    y->left = x->right; // [2]

    if (x->right != _nil) // 비어있지 않으면 부모까지 세팅
        x->right->parent = y; // [2] 번 부모 x로세팅

    // 기존 y의 부모를 x의 부모로 세팅
    x->parent = y->parent;

    // 기존 y의 부모노드에 자식으로 x노드 세팅
    if (x->parent == _nil) // y가 root였으면 x를 root로 설정
        _root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;

    // y를 x의 오른쪽 자식으로 둔다.
    x->right = y;
    y->parent = x;

}
