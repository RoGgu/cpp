#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
using namespace std;


// DFS (Depth Frist Search) 깊이 우선 탐색
// BFS (Breadth First Search) 너비 우선 탐색

struct Vertex
{
    // int data;
};

vector<Vertex> vertices;
vector<vector<int>> adjacent;
vector<bool> visited; // 방문한 곳 목록

void CreateGraph()
{
    vertices.resize(6);
    adjacent = vector<vector<int>>(6);

    //// 인접 리스트
    //adjacent[0].push_back(1);
    //adjacent[0].push_back(3);
    //adjacent[1].push_back(0);
    //adjacent[1].push_back(2);
    //adjacent[1].push_back(3);
    //adjacent[3].push_back(4);
    //adjacent[5].push_back(4);

    // 인접 행렬
    adjacent = vector<vector<int>>
    {
        {0, 1, 0, 1, 0, 0},
        {1, 0, 1, 1, 0, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0},
    };
}

// DFS

// Dfs(0)
// - Dfs(1)
// -- Dfs(2)
// -- Dfs(3)
// --- Dfs(4)

void Dfs(int here)
{
    // 방문!
    visited[here] = true;
    cout << "Visited : " << here << endl;
    //// 인접 리스트 version
    //// 모든 인접 정점을 순회한다
    //for (int i = 0; i < adjacent[here].size(); i++)
    //{
    //    int there = adjacent[here][i];  // 연결된 곳 목적지
    //    if (visited[there] == false) // 방문 체크
    //        Dfs(there); // 방문한적 없으면 타고 들어간다.
    //}

    // 인접 행렬 version
    // 모든 인접 정점을 순회한다
    for (int there = 0; there < 6; there++)
    {
        if (adjacent[here][there] == 0)
            continue;

        // 아직 방문하지 않은 곳이 있으면 방문한다
        if (visited[there] == false)
            Dfs(there);
    }
}


// 시작지점을 다 해서 연결된거 없어도 체크하도록 돈다
void DfsAll()
{
    for (int i = 0; i < 6; i++)
        if (visited[i] == false)
            Dfs(i);
}

int main()
{
    CreateGraph();
    
    visited = vector<bool>(6, false);
    //Dfs(0); // 0부터 시작하도록

    DfsAll();
}
