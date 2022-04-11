#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
using namespace std;


// DFS (Depth Frist Search) 깊이 우선 탐색
// - 발견과 방문이 동시에 일어난다
// BFS (Breadth First Search) 너비 우선 탐색
// - 시작점을 기준으로 까가운것부터 찾아서 길찾기에 유리하다.
// - 발견과 방문이 동시에 일어나지 않는다.

struct Vertex
{
    // int data;
};

vector<Vertex> vertices;
vector<vector<int>> adjacent;
vector<bool> discovered; // 발견 여부

void CreateGraph()
{
    vertices.resize(6);
    adjacent = vector<vector<int>>(6);

    // 인접 리스트
    adjacent[0].push_back(1);
    adjacent[0].push_back(3);
    adjacent[1].push_back(0);
    adjacent[1].push_back(2);
    adjacent[1].push_back(3);
    adjacent[3].push_back(4);
    adjacent[5].push_back(4);

    //// 인접 행렬
    //adjacent = vector<vector<int>>
    //{
    //    {0, 1, 0, 1, 0, 0},
    //    {1, 0, 1, 1, 0, 0},
    //    {0, 0, 0, 0, 0, 0},
    //    {0, 0, 0, 0, 1, 0},
    //    {0, 0, 0, 0, 0, 0},
    //    {0, 0, 0, 0, 1, 0},
    //};
}

void Bfs(int here)
{
    // 누구에 의해 발견 되었는지?
    vector<int> parent(6, -1);
    // 시작점에서 얼만큼 떨어져 있는지?
    vector<int> distance(6, -1);

    queue<int> q;
    q.push(here);
    discovered[here] = true; // 발견하면 넣어줌
    
    distance[here] = 0;
    parent[here] = here;

    while (q.empty() == false) // 탐색할 곳 없을떄까지 돈다
    {
        here = q.front();
        q.pop();

        cout << "Visited : " << here << endl;

        for (int there : adjacent[here]) // 현재 노드에서 연결된 노드 다 넣어줌
        {
            if (discovered[there]) // 발견한 애면 넘김
                continue;

            q.push(there);
            discovered[there] = true;

            parent[there] = here; // 날 발견한 애 세팅
            distance[there] = distance[here] + 1; // 전에 발견된 거리에서 한번 더 들어온거니 + 1해준다
        }
    }
    
    cout << "끝";
}

void BfsAll()
{
    for (int i = 0; i < 6; i++)
        if (discovered[i] == false)
            Bfs(i);
}

int main()
{
    CreateGraph();
    
    discovered = vector<bool>(6, false);

    Bfs(0);
}
