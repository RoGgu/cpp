#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
using namespace std;

// 맘에 안드는 방식
// 정점에 간선 데이터가 같이 묶여있음 - 따로 관리하는게 좋음
void CreateGraph_1()
{
    struct Vertex
    {
        vector<Vertex*> edges;
        // int data;
    };

    vector<Vertex> v;
    v.resize(6);

    v[0].edges.push_back(&v[1]);
    v[0].edges.push_back(&v[3]);
    v[1].edges.push_back(&v[0]);
    v[1].edges.push_back(&v[2]);
    v[1].edges.push_back(&v[3]);
    v[3].edges.push_back(&v[4]);
    v[5].edges.push_back(&v[4]);

    // Q) 0번과 -> 3번 정점이 연결되어 있나요?
    bool connected = false;
    for (Vertex* edge : v[0].edges)
    {
        if (edge == &v[3])
        {
            connected = true;
            break;
        }
    }
}

void CreateGraph_2()
{
    struct Vertex
    {
        // int data;
    };

    vector<Vertex> v;
    v.resize(6);
    
    // 정점이 100개
    // - 지하철 노선도 -> 서로 드문 드문 연결  
    // - 페이스북 친추 -> 서로 빽빽하게 연결 

    // 단점 
    // 연결 관계를 많으면 관계를 배열 돌면서 하나 씩 다 찾아야해서 오래걸린다.


    // 연결된 목록을 따로 관리 - 따로 별도로 관리한다.
    // adjacent[n] -> n번째 정점과 연결된 정점 목록
    vector<vector<int>> adjacent(6);

    adjacent[0] = { 1, 3 };
    adjacent[1] = { 0, 2, 3 };
    adjacent[3] = { 4 };
    adjacent[5] = { 4 };

    // Q) 0번과 -> 3번 정점이 연결되어 있나요?
    bool connected = false;
    for (int vertex : adjacent[0])
    {
        if (vertex == 3)
        {
            connected = true;
            break;
        }
    }

    // STL
    vector<int>& adj = adjacent[0];
    bool connected2 = std::find(adj.begin(), adj.end(), 3) != adj.end(); // 3이 없으면 end() 뱉어줘서 다르면 있다는 뜻
}

void CreateGraph_3()
{
    struct Vertex
    {
        // int data;
    };

    vector<Vertex> v;
    v.resize(6);

    // 행렬로 관리 - 이 방식이 많이 쓰인다, 데이터 적을경우 위에 vector방식 쓰이기도 한다.
    // 메모리는 많이 잡지만 데이터 접근이 빠르다 - 메모리써서 성능향상시킴
    // [X][0][X][0][X][X]
    // [0][X][0][0][X][X]
    // [X][X][X][X][X][X]
    // [X][X][X][X][0][X]
    // [X][X][X][X][X][X]
    // [X][X][X][X][0][X]

    // 읽는 방법 : adjacent[form][to]
    // 행렬을 이용한 그래프 표현(2차원 배열)
    // 메모리 소모가 심하지만, 빠른 접근이 가능하다.
    // (간선이 많은 경우 이점이 있다)

    vector<vector<bool>> adjacent(6, vector<bool>(6, false));
    adjacent[0][1] = true;
    adjacent[0][3] = true;
    adjacent[1][0] = true;
    adjacent[1][2] = true;
    adjacent[1][3] = true;
    adjacent[3][4] = true;
    adjacent[5][4] = true;


    // Q) 0번과 -> 3번 정점이 연결되어 있나요?
    bool connected = adjacent[0][3];

    // 가중치 그래프
    // 연결 안되어 있으면 -1 넣어줌 연결되어 있으면 가중치 값 넣어준다
    vector<vector<int>> adjacent2 =
    {
        vector<int> {-1, 15, -1, 35, -1, -1},
        vector<int> {15, -1,  5, 10, -1, -1},
        vector<int> {-1, -1, -1, -1, -1, -1},
        vector<int> {-1, -1, -1, -1,  5, -1},
        vector<int> {-1, -1, -1, -1, -1, -1},
        vector<int> {-1, -1, -1, -1,  5, -1}
    };

    adjacent2[0][3]; // 0->3 연결된 가중치의 값 35
}

int main()
{
}
