#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
using namespace std;

// 우선 순위 큐 구현

template<typename T, typename Container = vector<T>, typename Predicate = less<T>>
class Priority_queue
{
public:
    Priority_queue()
    {
        _heap.clear();
    }

    void push(const T& data)
    {
        // 우선 힙 구조부터 맞춰준다.
        _heap.push_back(data);

        // 도장 깨기 시작
        int now = static_cast<int>(_heap.size() - 1);
        
        // 루트 노드까지
        while (now > 0)
        {
            if ((now - 1) < 0)
                break;

            // 부모 노드의 데이터와 비교해서 더 작으면 패배
            int next = (now - 1) / 2;    
            if (_predicate(_heap[now], _heap[next])) // 기본 less로 되어 이써서 더 작은지 비교
                break;

            // 데이터 교체
            ::swap(_heap[now], _heap[next]);
            now = next;
        }
    }

    void pop()
    {
        _heap[0] = _heap.back();
        _heap.pop_back();

        int now = 0;

        while (true)
        {
            int left = (now * 2) + 1;
            int right = (now * 2) + 2;

            // 리프에 도달한 경우
            if (left >= _heap.size())
                break;

            int next = now;

            // 왼쪽과 비교
            if (_predicate(_heap[next], _heap[left]))
                next = left;
            
            // 둘 중 승자를 오른쪽과 비교
            if (right < (int)_heap.size() && _predicate(_heap[next], _heap[right]))
                next = right;

            // 왼쪽/오른쪽 둘 다 현재 값보다 작으면 종료
            if (next == now)
                break;

            ::swap(_heap[now], _heap[next]);
        }
    }

    T& top()
    {
        return _heap[0];
    }

    int size() { return _heap.size(); }
    int empty() { return _heap.empty() == true; }

private:
    Container _heap;
    Predicate _predicate = {}; // 기본 less로 되어 있다 왼쪽값이 더 작으면 true 리턴
};

int main()
{
    //priority_queue<int, vector<int>, greater<int>> pq; // 역순으로 만드는 방법 greater<T>를 넣어준다 , 기본 less<T>로 되어있음

    Priority_queue<int, vector<int>, greater<int>> pq;

    pq.push(100);
    pq.push(300);
    pq.push(200);
    pq.push(500);
    pq.push(400);

    while (pq.empty() == false)
    {
        int value = pq.top();
        pq.pop();

        cout << value << endl;
    }
}
