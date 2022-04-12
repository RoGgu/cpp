#include "pch.h"
#include "Player.h"
#include "Board.h"
#include <stack>

void Player::Init(Board* board)
{
    _pos = board->GetEnterPos();
    _board = board;

    //RightHand();
    Bfs();
}

void Player::Update(uint64 deltaTick)
{
    if (_pathIndex >= _path.size())
        return;

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
    _path.push_back(pos);
}
