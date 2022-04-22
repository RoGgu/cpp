#include "pch.h"
#include "Player.h"
#include "Board.h"
#include <stack>

void Player::Init(Board* board)
{
    _pos = board->GetEnterPos();
    _board = board;

    //RightHand();
    //Bfs();
    AStar();
}

void Player::Update(uint64 deltaTick)
{
    // 목표치 도달하면 맵 새로만들어서 다시 탐색
    if (_pathIndex >= _path.size())
    {
        _board->GenerateMap();
        Init(_board);
        return;
    }

    _sumTick += deltaTick;
    if (_sumTick >= MOVE_TICK)
    {
        _sumTick = 0;

        _pos = _path[_pathIndex];
        _pathIndex++;
    }
}

bool Player::CanGo(Pos pos)
{
    TileType tileType = _board->GetTileType(pos);
    return tileType == TileType::EMPTY;
}

void Player::RightHand()
{
    Pos pos = _pos; // 이동 시뮬레이션 좌표

    _path.clear();
    _path.push_back(pos);

    // 목적지 도착하기 전에는 계속 실행
    Pos dest = _board->GetExitPos();

    Pos front[4] =
    {
        Pos {-1, 0},    // UP
        Pos {0, -1},    // LEFT
        Pos {1, 0},     // DOWN
        Pos {0, 1},     // RIGHT
    };

    while (pos != dest)
    {

        // 1. 현재 바라보는 방향을 기준으로 오른쪽으로 갈 수 있는지 확인.
        int32 newDir = (_dir - 1 + DIR_COUNT) % DIR_COUNT;
        if (CanGo(pos + front[newDir]))
        {
            // 오른쪽 방향으로 90도 회전
            _dir = newDir;

            // 앞으로 한 보 전진
            pos += front[_dir];

            _path.push_back(pos); // 전진 경로 저장
        }
        // 2). 현재 바라보는 방향을 기준으로 전진할 수 있는지 확인.
        else if (CanGo(pos + front[_dir]))
        {
            // 앞으로 한 보 전진
            pos += front[_dir];
            _path.push_back(pos);
        }
        else
        {
            // 왼쪽 방향으로 90도 회전.
            _dir = (_dir + 1) % DIR_COUNT;
        }
    }

    // 막다른길 가서 다시 되돌아오는 경로는 삭제한다
    stack<Pos> s;
    // 마지막 도착지 뺴고 다 검사
    for (int i = 0; i < _path.size() - 1; i++)
    {
        if (s.empty() == false && s.top() == _path[i + 1])
            s.pop();
        else
            s.push(_path[i]);
    }

    // 목적지 도착지 넣어줌
    if (s.empty() == false)
        s.push(_path.back());

    // 다시 되돌아오는 경우 삭제한 새로운 경로로 임시세팅
    vector<Pos> path;
    while (s.empty() == false)
    {
        path.push_back(s.top());
        s.pop();
    }
    // stack 에 있던걸 뺴면서 넣은거라 순서가 반대라 뒤집어줌
    std::reverse(path.begin(), path.end());
    // 새로운 경로로  세팅
    _path = path;
}

void Player::Bfs()
{
    Pos pos = _pos; // 이동 시뮬레이션 좌표

    // 목적지 도착하기 전에는 계속 실행
    Pos dest = _board->GetExitPos();

    Pos front[4] =
    {
        Pos {-1, 0},    // UP
        Pos {0, -1},    // LEFT
        Pos {1, 0},     // DOWN
        Pos {0, 1},     // RIGHT
    };

    const int32 size = _board->GetSize();
    vector<vector<bool>> discovered(size, vector<bool>(size, false));

    //vector<vector<Pos>> parent;
    // parent[A] = B; -> A는 B로 인해 발견함
    map<Pos, Pos> parent;

    queue<Pos> q;
    q.push(pos);
    discovered[pos.y][pos.x] = true;
    parent[pos] = pos;

    while (q.empty() == false)
    {
        pos = q.front();
        q.pop();

        // 방문!
        if (pos == dest) // 목적지 도달하면 그만함
            break;
     
        for (int32 dir = 0; dir < DIR_COUNT; dir++)
        {
            Pos nextPos = pos + front[dir]; // 다음 좌표
            // 갈 수 있는 지역은 맞는지 확인
            if (CanGo(nextPos) == false)
                continue;

            // 이미 발견한 지역인지 확인.
            if (discovered[nextPos.y][nextPos.x])
                continue;

            q.push(nextPos);
            discovered[nextPos.y][nextPos.x] = true;
            parent[nextPos] = pos;
        }
    }

    _path.clear();

    // 거꾸로 거슬러 올라간다.
    pos = dest;

    while (true)
    {
        _path.push_back(pos);

        // 시작점은 자신이 곧 부모이다.
        if (pos == parent[pos])
            break;

        pos = parent[pos];
    }

    // 부모로 꺼구로 들어가 있으니 반전시켜준다
    std::reverse(_path.begin(), _path.end());
}

struct PQNode
{
    bool operator<(const PQNode& other) const { return f < other.f; }
    bool operator>(const PQNode& other) const { return f > other.f; }

    int32 f; // f = g + h
    int32 g;
    Pos pos;
};

void Player::AStar()
{
    // 점수 매기기
    // F = G + H
    // F = 최종 점수 (작을 수록 좋음, 경로에 따라 달라짐)
    // G = 시작점에서 해당 좌표까지 이동하는데 드는 비용 (작을 수록 좋음, 경로에 따라 달라짐)
    // H = 목적지에서 얼마나 가까운지 (작을 수록 좋음, 고정)
    // - F가 가장 작은애가 좋은 경로라서 방문

    Pos start = _pos; // 이동 시뮬레이션 좌표
    // 목적지 도착하기 전에는 계속 실행
    Pos dest = _board->GetExitPos();

    enum // 대각선 포함할지 말지 결정
    {
        DIR_COUNT = 8
    };

    Pos front[] =
    {
        Pos {-1, 0},    // UP
        Pos {0, -1},    // LEFT
        Pos {1,  0},    // DOWN
        Pos {0,  1},    // RIGHT
        Pos {-1,-1},    // UP_LEFT,
        Pos {1, -1},    // DOWN_LEFT,
        Pos {1,  1},    // DOWN_RIGHT,
        Pos {-1, 1},    // UP_RIGHT,
    };

    // 이동하는데 드는 비용
    int32 cost[] =
    {
        10,     // UP
        10,     // LEFT
        10,     // DOWN
        10,     // RIGHT
        14,     // UP_LEFT,
        14,     // DOWN_LEFT,
        14,     // DOWN_RIGHT,
        14      // UP_RIGHT,
    };

    const int32 size = _board->GetSize();

    // ClosedList (옵션) 방문한 목록
    // closep[y][x] -> (y,x)에 방문을 했는지 여부
    vector<vector<bool>> closed(size, vector<bool>(size, false));

    // best[y][x] -> 지금까지 (y,x)에 대한 가장 좋은 비용 (작을 수록 좋음)
    vector<vector<int32>> best(size, vector<int32>(size, INT32_MAX));

    // 부모 추적 용도
    map<Pos, Pos> parent;
    
    // OpenList // 방문 예약 리스트 (방문은 하지 않았지만 발견한 애들)
    priority_queue<PQNode, vector<PQNode>, greater<PQNode>> pq;

    // 1) 예약(발견) 시스템 구현
    // - 이미 더 좋은 경로를 찾았다면 스킵
    // 2) 뒤늦게 더 좋은 경로가 발견될 수 있음 -> 예외 처리 필수
    // - openList에서 찾아서 제거한다거나,
    // - pq에서 pop한 다음에 무시하거나.

    // 초기값
    {
        int32 g = 0;
        int32 h = 10 * (abs(dest.y - start.y) + abs(dest.x - start.x)); // 임의 공식 -진짜 정교하게 할려면 피타고라스 공식으로 하는 경우도있지만, 이 방식이 간단해서 더 많이 쓰인다
        pq.push(PQNode{ (g + h), g, start });
        best[start.y][start.x] = g + h;
        parent[start] = start; // 시작 지점은 자기 자신이 부모
    }

    while (pq.empty() == false)
    {
        // 제일 좋은 후보를 찾는다
        PQNode node = pq.top();
        pq.pop();

        // 동일한 좌표를 여러 경로로 차자서, 더 빠른 경로로 인해서 이미 방문(closed)된 경우 스킵
        
        // [선택]
        // 이미 짧은경로로 발견했다고 체크되어 있을 경우 스킵
        if (closed[node.pos.y][node.pos.x] == true)
            continue;
        //// 이미 best에 더 좋은 값이 들어가 있을 경우 스킵
        //if (best[node.pos.y][node.pos.x] < node.f)
        //    continue;

        // 방문 - 수학적으로도 증명 가능한데 현재 경우에서 이미 방문 한 경우 더 짧게 갈 수있는 경우는 없다.
        closed[node.pos.y][node.pos.x] = true;

        // 목적지에 도착했으면 종료
        if (node.pos == dest)
            break;

        // 방문 했으면 현재 노드에서 갈 수 있는 노드 발견목록에 추가
        for (int32 dir = 0 ; dir < DIR_COUNT; dir++)
        {
            Pos nextPos = node.pos + front[dir];
            // 갈 수 있는 지역은 맞는지 확인
            if (CanGo(nextPos) == false)
                continue;

            // [선택] 이미 방문한 곳이면 스킵
            if (closed[nextPos.y][nextPos.x])
                continue;

            // 비용 계산
            int32 g = node.g + cost[dir]; // 이전 좌표비용에서 현재 좌표로 이동하는 비용
            int32 h = 10 * (abs(dest.y - nextPos.y) + abs(dest.x - nextPos.x)); 

            // 아예 위에서 꺼내서 체크할 필요 없이 넣지 않게 할 수 있다.
            // -다른 경로에서 더 빠른 길을 찾았으면 스킵
            // -- 방문은 하지 않았지만 다른데서 예약 해놓은거랑 비교
            // --- 다른데서 더 적은 비용으로 예약해놨을 수도 있다.
            if (best[nextPos.y][nextPos.x] <= g + h)
                continue;

            // 예약 진행
            best[nextPos.y][nextPos.x] = g + h;
            pq.push(PQNode{ g + h, g, nextPos });
            parent[nextPos] = node.pos; // 발견 부모 세팅
        }
    }

    // 거꾸로 거슬러 올라간다.
    Pos pos = dest;
    _path.clear();
    _pathIndex = 0;

    while (true)
    {
        _path.push_back(pos);

        // 시작점은 자신이 곧 부모이다.
        if (pos == parent[pos])
            break;

        pos = parent[pos];
    }

    // 부모로 꺼구로 들어가 있으니 반전시켜준다
    std::reverse(_path.begin(), _path.end());
}
