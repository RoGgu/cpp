#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
using namespace std;

// 다익스트라 알고리즘
// - 우선순위 큐랑 궁합이 좋다 (최상의 경로를 찾는거에 쓰인다)

struct Vertex
{
    // int data;
};

vector<Vertex> vertices;
vector<vector<int>> adjacent; // 인접 행렬

void CreateGraph()
{
    vertices.resize(6);
    adjacent = vector<vector<int>>(6, vector<int>(6, -1)); // 연결 안되어있는 애들은 -1

    adjacent[0][1] = 15;
    adjacent[0][3] = 35;
    adjacent[1][0] = 15;
    adjacent[1][2] = 5;
    adjacent[1][3] = 10;
    adjacent[3][4] = 5;
    adjacent[5][4] = 5;
}

void Dijikstra(int here)
{
    struct VertexCost
    {
        int vertex;
        int cost;
    };

    list<VertexCost> discovered; // 발견 목록
    vector<int> best(6, INT32_MAX); // 각 정점별로 지금까지 발견한 최소 거리
    vector<int> parent(6, -1);

    // 초기화 0 부터 시작
    discovered.push_back(VertexCost{ here, 0 });
    best[here] = 0;
    parent[here] = here;

    while (discovered.empty() == false)
    {
        // 제일 좋은 후보를 찾는다 - 나중에 우선순위 큐를 사용해서 작업
        list<VertexCost>::iterator bestIt;
        int bestCost = INT32_MAX;

        for (auto it = discovered.begin() ; it != discovered.end(); it++)
        {
            if (it->cost < bestCost)
            {
                bestCost = it->cost;
                bestIt = it;
            }
        }

        int cost = bestIt->cost;
        here = bestIt->vertex; // 제일 좋은 후보 (비용이 제일 작은 경로)
        discovered.erase(bestIt);

        // 방문? 더 짧은 경로가 이미 세팅되어 있다면 스킵
        if (best[here] < cost)
            continue;

        // 방문
        for (int there = 0; there < 6; there++)
        {
            //  연결되지 않았으면 스킵.
            if (adjacent[here][there] == -1)
                continue;

            int nextCost = best[here] + adjacent[here][there]; // 현재 here까지 온 비용 + 그 노드가는 비용
            // 더 조좋은 경로를 과거에 찾았으면 스킵
            if (nextCost >= best[there])
                continue;

            best[there] = nextCost;
            parent[there] = here; // 부모 경로 세팅
            
            // 새로 찾은 경로 세팅
            // 참고) 이미 같은 정점 경로가 들어가 있다면 새로 안 넣고 코스트만 바까주는 경우도 있음
            discovered.push_back(VertexCost{ there, nextCost });
        }
    }
}


int main()
{
    CreateGraph();

    Dijikstra(0);

}
