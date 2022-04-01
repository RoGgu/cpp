#include <iostream>
#include <vector>
using namespace std;

template<typename T>
class Vector
{
public :
    Vector()
    {

    }

    ~Vector()
    {
        if (_data)
            delete[] _data;
    }

    void push_back(const T& value)
    {
        if (_size == _capacity)
        {
            // 증설 작업
            int newCapacity = static_cast<int>((_capacity * 1.5));
            if (newCapacity == _capacity) // 기존 capacity가 0일경우를 대비
                newCapacity++;

            reserve(newCapacity);
        }

        // 데이터 저장
        _data[_size] = value;

        // 데이터 개수 증가
        _size++;
    }

    void reserve(int capacity)
    {
        if (_capacity >= capacity) // 새로 늘릴려는게 기존보다 작으면 할 필요 없다.
            return;

        _capacity = capacity;

        T* newData = new T[_capacity];

        // 데이터 복사
        for (int i = 0; i < _size; i++)
            newData[i] = _data[i];

        if (_data)
            delete[] _data;

        //교체
        _data = newData;
    }

    T& operator[](const int pos) { return _data[pos]; }

    int size() { return _size; }
    int capacity() { return _capacity; }

    void clear()
    {
        if (_data)
        {
            delete[] _data;
            _data = new T[_capacity];
        }

        _size = 0;
    }

private:
    T*  _data = nullptr; // 가지고 있는 데이터 배열
    int _size = 0; // 현재 데이터 수
    int _capacity = 0; // 할당받은 총 데이터 크기
};

int main()
{
    Vector<int> v;

    v.reserve(100); // 만약 배열의 대략적인 크기를 안다면 처음부터 capacity즉 여유값을 미리 할당해주면 좋다

    for (int i = 0; i < 100; i++)
    {
        v.push_back(i);
        cout << v[i]<< " " << v.size() << " " << v.capacity() << endl;
    }

    v.clear();
    cout << v.size() << " " << v.capacity() << endl;
}
