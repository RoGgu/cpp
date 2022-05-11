#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
using namespace std;
#include "BinarySearchTree.h"
#include<thread>

// 오늘의 주제 : 이진 탐색 트리

// 이진 탐색트리

// 이진 탐색에 데이터 삽입 삭제 처리가 느린 한계를 극복하기 위해 사용
// 부모 노드기준으로 오른쪽엔 부모보다 큰 값, 왼쪽엔 작은 값이 세팅된다.

// -한계 
// -- 데이터가 순차적으로 들어갈 경우 한쪽으로만 데이터가 쏠리는 경우가 발생한다. (ex 게쏙 오른쪽 or 왼쪾 자신만 일렬)

//        [20]
//   [10]     [30]
//         [25]  [40]
//           [26]   [50]

int main()
{
    BinarySearchTree bst;

    bst.Insert(30);
    bst.Print();
    this_thread::sleep_for(1s);

    bst.Insert(10);
    bst.Print();
    this_thread::sleep_for(1s);
    bst.Insert(20);
    bst.Print();
    this_thread::sleep_for(1s);
    bst.Insert(25);
    bst.Print();
    this_thread::sleep_for(1s);
    bst.Insert(40);
    bst.Print();
    this_thread::sleep_for(1s);
    bst.Insert(50);
    bst.Print();
    this_thread::sleep_for(1s);
}