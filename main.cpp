#include <iostream>
#include <stdexcept>
using namespace std;
//  Функторы-компараторы
template<typename T> // Наверху окажется максимум
struct Less {
    bool operator()(const T& a, const T& b)const { return a < b; }
};

template<typename T> // Наверху окажется минимум
struct Greater {
    bool operator()(const T& a, const T& b)const { return a > b; }
};

template<typename T, typename Compare = Less<T>>
class PriorityQueue {
private:
    T* heap; // Массив-куча
    int count; // Кол-во эл
    int capacity; // Выделено
    Compare comp; // Сравнивает приоритет
    // Меняет значения
    static void swapValue(T& a, T& b) {
        T tmp = a;
        a = b;
        b = tmp;
    }
    // Удваивает массив
    void resize(int newCapacity) {
        T* newHeap = new T[newCapacity];
        for (int i = 0; i <= count; ++i) newHeap[i] = heap[i];
        delete[] heap;
        heap = newHeap;
        capacity = newCapacity;
        cout << "Стек увеличен до " << capacity << " элементов" << endl;
    }
    // Эл-нт поднимается вверх, пока он приоритетнее
    void siftUp(int i) {
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (!comp(heap[parent], heap[i])) break;
            swapValue(heap[parent], heap[i]);
            i = parent;
        }
    }
    // Эл-нт опускается вниз, меняясь с приоритетным
    void siftDown(int i) {
        while (true) {
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            int best = i;
            if (left < count && comp(heap[best], heap[left])) best = left; // Выбирается лучший
            if (right < count && comp(heap[best], heap[right])) best = right;
            if (best == i) break;
            swapValue(heap[i], heap[best]);
            i = best;
        }
    }
public:
    // Порядок объявления полей
    explicit PriorityQueue(int cap = 10) : count(0), capacity(cap > 0 ? cap : 1), comp() {
        heap = new T[capacity];
    }
    // Вариант с явным компаратором
    PriorityQueue(Compare c, int cap = 10)
        : count(0), capacity(cap > 0 ? cap : 1), comp(c) {
        heap = new T[capacity];
    }
    // Освобождение массива
    ~PriorityQueue() {
        delete[] heap;
        cout << "Queue is empty" << endl;
    }
    // Запрещаем копирование 
    PriorityQueue(const PriorityQueue&) = delete;
    PriorityQueue& operator=(const PriorityQueue&) = delete;
    // Положили в конец + всплытие
    void push(const T& value) {
        if (count == capacity) resize(capacity * 2);
        heap[count] = value;
        ++count;
        siftUp(count - 1);
    }
    // Извлечение
    T pop() {
        if (count == 0)
            throw out_of_range("PriorityQueue is empty");
        T result = heap[0];
        --count;
        heap[0] = heap[count];
        if (count > 0) siftDown(0);
        return result;
    }

    bool isEmpty() const { return count == 0; }
    int size() const { return count; }
    // Просмотр верхнего
    const T& top() {
        if (count == 0) {
            throw out_of_range("PriorityQueue is empty");
        }
        return heap[0];
    }
    // Вывод массив-кучи
    void print() const {
        cout << "[";
        for (int i = 0; i < count; ++i) {
            if (i) cout << ", ";
            cout << heap[i];
        }
        cout << "]" << endl;
    }

};

int main() {
    setlocale(LC_ALL, "ru");
    cout << "Анализ сложности операций" << endl;
    cout << "push: O(log n)" << endl;
    cout << "pop: O(log n)" << endl;
    cout << "top: O(1)" << endl;
    cout << "isEmpty: O(1)" << endl;
    cout << "size: O(1)" << endl;

    PriorityQueue<int>s(4);
    for (int x : {5, 1, 9, 3, 7, 2, 8}) s.push(x);
    cout << "\nmax-heap, размер = " << s.size() << endl;
    cout << "top = " << s.top() << endl;
    cout << "Извлечение: ";
    while (!s.isEmpty()) {
        cout << s.pop() << " ";
    }
    cout << "\n";

    PriorityQueue<int, Greater<int>> s1(4);
    for (int x : {5, 1, 9, 3, 7, 2, 8}) s1.push(x);

    cout << "\nmin-heap, размер = " << s1.size() << endl;
    cout << "top = " << s1.top() << endl;
    cout << "Извлечение: ";
    while (!s1.isEmpty()) {
        cout << s1.pop() << " ";
    }
    cout << "\n";
    return 0;
}