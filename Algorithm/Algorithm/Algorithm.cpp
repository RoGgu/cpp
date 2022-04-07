#include <iostream>
#include <vector>
#include <list>
#include <stack>
using namespace std;

// Statck (LIFO Last-In-Frist-Out 후입선출 (뒤 늦게 들어온애가 가장 만저 나간다))

// [1][2][3][4] << 밀어넣음
// 되돌리기 (Ctrl+Z)

template<typename T, typename Contatiner = vector<T>> // 두번쨰 type로 컨테이너 타입 지정해줄 수 있다.
class Stack
{
public:
    void push(const T& value)
    {
        _container.push_back(value);
    }

    void pop()
    {
        _container.pop_back();
    }

    T& top()
    {
        return _container.back(); // 마지막 데이터 뱉어줌
    }


    int size() { return _container.size(); }
    bool empty() { return _container.empty(); }

private:
    //vector<T> _container;
    //list<T> _container; // list로 바꿔도 둘에 함수 호출이나 그런게 똑같아서 바로 작동된다.
    Contatiner _container; // 어떤 타입으로할지도 templaye인자로 받아서 작업도 가능하다
};


int main()
{
    Stack<int, vector<int>> s;

    // 삽입
    s.push(1);
    s.push(2);
    s.push(3);

    while (s.empty() == false) 
    {
        // 최상위 원소
        int data = s.top();

        // 최상위 원소 삭제
        s.pop();
        cout << data << endl;
    }

    int size = s.size();
}
