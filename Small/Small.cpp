#include <windows.h>
#include <iostream>
#include <vector>
using namespace std;


int main(int argc, char* argv[]) {
    
    HANDLE ReadPipe1 = GetStdHandle(STD_INPUT_HANDLE); //канал для чтения
    HANDLE WritePipe2 = GetStdHandle(STD_OUTPUT_HANDLE); //канал для записи
    HANDLE EndEvent = OpenEvent(SYNCHRONIZE, FALSE, TEXT("EndEvent"));
    DWORD bytesRead, bytesWritten;

    int n = 0, k = 0;
    vector<double> arr, new_arr;
    double temp = 0;


    //получаем размер и элементы массива
    if (!ReadFile(ReadPipe1, &n, sizeof(n), &bytesRead, NULL)) {
        cerr << "Ошибка чтения размера массива" << std::endl;
        cerr << "Код ошибки: " << GetLastError() << std::endl;
        while (1) {}
    }


    cerr << "Got from server: " << '\n' << "array size: " << n << '\n' << "Elemets: " << '\n';
    for (int i = 0; i < n; ++i) {
        ReadFile(ReadPipe1, &temp, sizeof(temp), &bytesRead, NULL);
        cerr << temp << '\n';
        if (temp > 0) {
            new_arr.push_back(sqrt(temp));
            k++;
        }
        arr.push_back(temp);
    }

    cerr << endl << endl << "Sent to server: " << endl;
    for (double x : new_arr) cerr << x << endl;

    //передаем размер нового массива и его элементы
    WriteFile(WritePipe2, &k, sizeof(k), &bytesWritten, NULL);

    for (int i = 0; i < k; ++i) {
        WriteFile(WritePipe2, &new_arr[i], sizeof(new_arr[i]), &bytesWritten, NULL);
    }
    
    cerr << endl << "Program is done";

    WaitForSingleObject(EndEvent, INFINITE);
}