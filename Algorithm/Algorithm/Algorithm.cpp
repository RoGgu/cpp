#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
using namespace std;

// Queue (FIFO First-In-First-Out 선입선출)

// front << [1][2][3][4] << rear(back)
// ex) 대기열


// vector를 순환 구조로 사용
// [front / back][][][][][][][][]
// [front ][back][][][][][][][][]
// front 앞에 붙이는 거랑 back 맨뒤 데이터 기준점을 둔다
// [][][back][][][][][front][][]
// 이런식으로 back이 front 앞으로 올 수 도있다.
// -그럼 front 에서부터 한바퀴 돌아서 0으로가서 back까지가 범위
template<typename T>
class ArrayQueue
{
public:
    ArrayQueue()
    {
        _container.resize(100);
    }

    void push(const T& value)
    {
        // 데이터 다 찾는지 체크
        if (_size == _container.size())
        {
            // 증설 작업
            int newSize = max(1, _size * 2); // _size가 처음에 0일 경우를 대비해서
            vector<T> newData;
            newData.resize(newSize);

            // 데이터 복사
            for (int i = 0; i < _size; i++)
            {
                int index = (_front + i) % _container.size(); // front부터 back까지의 데이터 새 데이터배열 0번부터 넣어줌
                newData[i] = _container[index];
            }

            _container.swap(newData);
            _front = 0;
            _back = _size;
        }

        _container[_back] = value;
        _back = (_back + 1) % _container.size(); // 나머지 연산으로 범위 빠져나가도 한바퀴 돌아서 앞으로 가게함
        _size++; // 들어가 있는 데이터의 개 수 size()랑 다르다
    }

    void pop()
    {
        if (_size == 0)
            return;

        _front = (_front + 1) % _container.size(); // 애도 한바퀴 돌면 앞으로 오게 나머지 연산
        _size--;

    }

    T& front()
    {
        return _container[_front];
    }

    bool empty() { return _size == 0; }
    bool size() { return _size; }


private:
    vector<T> _container;

    int _front = 0;
    int _back = 0;
    int _size = 0;
};


template<typename T>
class ListQueue
{
public:
    void push(const T& value)
    {
        _container.push_back(value);
    }

    void pop()
    {
        _container.erase(_container.begin());

    }

    T& front()
    {
        return _container.front();
    }

    bool empty() { return _container.empty(); }
    bool size() { return _container.size(); }


private:
    list<T> _container;
};

int main()
{
    ArrayQueue<int> q;

    for (int i = 0; i < 500; i++)
        q.push(i);

    while (q.empty() == false)
    {
        int value = q.front();
        q.pop();
        cout << value << endl;
    }

    int size = q.size();
}
