// Task 3: print server using a circular queue with dynamic resizing
#include <iostream>
using namespace std;

struct Request { int id, arrival, processing; };

class CircularQueue {
    Request* data;
    int cap, count, frontIdx;
public:
    CircularQueue(int c = 4) : cap(c), count(0), frontIdx(0) { data = new Request[cap]; }
    ~CircularQueue() { delete[] data; }

    bool isEmpty() { return count == 0; }
    bool isFull() { return count == cap; }
    int capacity() { return cap; }

    void resize(int newCap) {                       // copy in order so nothing is lost
        Request* nd = new Request[newCap];
        for (int i = 0; i < count; i++) nd[i] = data[(frontIdx + i) % cap];
        delete[] data;
        data = nd; cap = newCap; frontIdx = 0;
        cout << "   [resized to capacity " << cap << "]\n";
    }
    void enqueue(Request x) {
        if (isFull()) resize(cap * 2);
        data[(frontIdx + count) % cap] = x;
        count++;
    }
    Request front() { return data[frontIdx]; }
    Request dequeue() {
        Request x = data[frontIdx];
        frontIdx = (frontIdx + 1) % cap;
        count--;
        if (cap > 4 && count <= cap / 4) resize(cap / 2);   // too sparse
        return x;
    }
    void show() {
        cout << "   Queue (cap=" << cap << ", size=" << count << "): ";
        if (isEmpty()) cout << "empty";
        for (int i = 0; i < count; i++) cout << "[ID" << data[(frontIdx + i) % cap].id << "] ";
        cout << endl;
    }
};

int main() {
    CircularQueue q(4);
    Request reqs[] = {{1, 0, 5}, {2, 1, 3}, {3, 2, 4}, {4, 3, 2}, {5, 4, 6}, {6, 5, 1}, {7, 6, 3}};
    int n = 7;

    for (int i = 0; i < n; i++) {
        cout << "Enqueue request " << reqs[i].id << " (arrival " << reqs[i].arrival
             << ", processing " << reqs[i].processing << ")\n";
        q.enqueue(reqs[i]);
        q.show();
    }

    int clock = 0, totalWait = 0, processed = 0;
    cout << "\n Processing \n";
    while (!q.isEmpty()) {
        Request r = q.dequeue();
        if (clock < r.arrival) clock = r.arrival;
        int wait = clock - r.arrival;
        cout << "Dequeue request " << r.id << " (starts at " << clock << ", waited " << wait << ")\n";
        clock += r.processing;
        totalWait += wait; processed++;
        q.show();
    }
    cout << "\nAverage waiting time = " << (double)totalWait / processed << endl;
    return 0;
}