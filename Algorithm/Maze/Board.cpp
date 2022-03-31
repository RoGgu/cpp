#include "pch.h"
#include "Board.h"
#include "Player.h"

const char* TILE = "■";

Board::Board()
{
}

Board::~Board()
{
}

void Board::Init(int32 size, Player* player)
{
    _size = size;
    _player = player;

    GenerateMap();
}

void Board::Render()
{
    ConsoleHelper::SetCursorPosition(0, 0);
    ConsoleHelper::ShowConsoleCursor(false);

    for (int32 y = 0; y < 25; y++)
    {
        for (int32 x = 0; x < 25; x++)
        {
            ConsoleColor color = GetTileColor(Pos{ y,x });
            ConsoleHelper::SetCursorColor(color);
            cout << TILE;
        }
        cout << endl;
    }
}

// Binary Tree 미로 생성 알고리즘
// - Mazes For Programmers
// 2로 나눠지는 곳은 막고 아닌 곳은 뚥고
// 이제 막히지 않은곳에서 오른쪽이랑 아래를 2분의 1 확률로 열어줌
// 현재 테투리는 다 막혀야하기 떄문에 사이즈가 홀수일 때만 작동함

void Board::GenerateMap()
{
    for (int32 y = 0; y < _size; y++)
    {
        for (int32 x = 0; x < _size; x++)
        {
            if (x % 2 == 0 || y % 2 == 0)
                _tile[y][x] = TileType::WALL;
            else
                _tile[y][x] = TileType::EMPTY;
        }
    }

    // 랜덤으로 우측 혹은 아래로 길을 뚫는 작업
    for (int32 y = 0; y < _size; y++)
    {
        for (int32 x = 0; x < _size; x++)
        {
            if (x % 2 == 0 || y % 2 == 0) // 기존 막힌덴 냅둠
                continue;

            if (y == _size - 2 && x == _size - 2) // 마지막 끝자리 벽으로 막아준다
                continue;

            if (y == _size - 2) // y가 벽바로 위 즉 마지막 공간일떄 오른쪾으로 뚫어줌
            {
                _tile[y][x + 1] = TileType::EMPTY;
                continue;
            }

            if (x == _size - 2) // x도 오른쪽 벽 전 끝일떄 아래로 뚫어줌
            {
                _tile[y + 1][x] = TileType::EMPTY;
                continue;
            }

            const int32 randValue = ::rand() % 2;
            if (randValue == 0)
            {
                _tile[y][x + 1] = TileType::EMPTY;
            }
            else
            {
                _tile[y + 1][x] = TileType::EMPTY;
            }
        }
    }

}

TileType Board::GetTileType(Pos pos)
{
    if (pos.x < 0 || pos.x >= _size)
        return TileType::NONE;

    if (pos.y < 0 || pos.y >= _size)
        return TileType::NONE;

    return _tile[pos.y][pos.x];
}

ConsoleColor Board::GetTileColor(Pos pos)
{
    if (_player && _player->GetPos() == pos) // 플레이어 색 표시
        return ConsoleColor::YELLOW;

    if (GetExitPos() == pos) // 출구 파란색으로 설정
        return ConsoleColor::BLUE;

    TileType tileType = GetTileType(pos);

    switch (tileType)
    {
    case TileType::EMPTY:
        return ConsoleColor::GREEN;
    case TileType::WALL:
        return ConsoleColor::RED;
    }

    return ConsoleColor::WHITE;
}
