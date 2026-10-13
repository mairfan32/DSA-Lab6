// Task 4: emergency ward waiting room using an array-based circular queue
#include <iostream>
using namespace std;

const int MAX = 100;

class WaitingRoom {
    int q[MAX];
    int frontIdx = 0, count = 0, nextId = 1;
public:
    void RegisterPatient() {
        if (count == MAX) { cout << "Waiting room is full!\n"; return; }
        q[(frontIdx + count) % MAX] = nextId;
        count++;
        cout << "Patient registered with ID " << nextId++ << endl;
    }
    void ServePatient() {
        if (count == 0) { cout << "No patients waiting.\n"; return; }
        cout << "Serving patient ID " << q[frontIdx] << endl;
        frontIdx = (frontIdx + 1) % MAX;
        count--;
    }
    void CancelAll() {
        frontIdx = 0; count = 0;
        cout << "All appointments cancelled. Doctor can go to lunch.\n";
    }
    bool CanDoctorGoHome() { return count == 0; }
    void ShowAllPatient() {
        if (count == 0) { cout << "No patients waiting.\n"; return; }
        int tmp[MAX];
        for (int i = 0; i < count; i++) tmp[i] = q[(frontIdx + i) % MAX];
        for (int i = 1; i < count; i++) {               // insertion sort (IDs are already ascending, kept for safety)
            int k = tmp[i], j = i - 1;
            while (j >= 0 && tmp[j] > k) { tmp[j + 1] = tmp[j]; j--; }
            tmp[j + 1] = k;
        }
        cout << "Waiting patients: ";
        for (int i = 0; i < count; i++) cout << tmp[i] << " ";
        cout << endl;
    }
};

int main() {
    WaitingRoom w;
    int choice;
    do {
        cout << "\n1.Register  2.Serve  3.Cancel All  4.Can doctor go home?  5.Show all  0.Exit\nChoice: ";
        cin >> choice;
        switch (choice) {
            case 1: w.RegisterPatient(); break;
            case 2: w.ServePatient(); break;
            case 3: w.CancelAll(); break;
            case 4: cout << (w.CanDoctorGoHome() ? "true" : "false") << endl; break;
            case 5: w.ShowAllPatient(); break;
        }
    } while (choice != 0 && cin);
    return 0;
}