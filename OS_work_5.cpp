#include <windows.h>
#include <iostream>
#include <vector>
#include <conio.h>
using namespace std;


int main() {
    HANDLE ReadPipe1, WritePipe1, ReadPipe2, WritePipe2;
    HANDLE EndEvent = CreateEvent(NULL, FALSE, FALSE, TEXT("EndEvent"));


    SECURITY_ATTRIBUTES saAttr;

    // Настройка безопасности канала
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.lpSecurityDescriptor = NULL; //снимаем защиту
    saAttr.bInheritHandle = TRUE; //разрешаем наследование

    // Создание анонимных каналов
    if (!CreatePipe(&ReadPipe1, &WritePipe1, &saAttr, 0)) {
        cerr << "Error creating channel number 1" << endl;
        return 1;
    }

    if (!CreatePipe(&ReadPipe2, &WritePipe2, &saAttr, 0)) {
        cerr << "Error creating channel number 2" << endl;
        return 1;
    }

    //основной процесс читает со второго канала, отправляет в первый
    // Настройка дочернего процесса
    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.hStdInput = ReadPipe1; //дочерний процесс читает с первого канала
    si.hStdOutput = WritePipe2; //отправляет во второй
    si.dwFlags |= STARTF_USESTDHANDLES; //перенаправление стандатрных потоков на два выше

    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));

    int n;
    vector<double> arr;
    cout << "Enter array size: ";
    cin >> n;
    arr.resize(n);
    cout << "Enter array elements: " << endl;
    for (int i = 0; i < n; ++i) cin >> arr[i];

    wstring Arg = L"Small.exe";

    //запуск Small
    CreateProcess(NULL, &Arg[0], NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);

    //передаем размер массива и элементы
    DWORD bytesWritten, bytesRead;

    WriteFile(WritePipe1, &n, sizeof(n), &bytesWritten, NULL);
    for (int i = 0; i < n; ++i) {
        WriteFile(WritePipe1, &arr[i], sizeof(arr[i]), &bytesWritten, NULL);
    }

    //Получаем новый массив

    int k = 0; 
    double temp;
    vector<double> new_arr;

    ReadFile(ReadPipe2, &k, sizeof(k), &bytesRead, NULL);
    
    for (int i = 0; i < k; ++i) {
        ReadFile(ReadPipe2, &temp, sizeof(temp), &bytesWritten, NULL);
        new_arr.push_back(temp);
    }

    cout << endl << endl << "Got from Small: " << endl << "Size of new array: " << k << endl << "Elements: " << endl;

    for (double x : new_arr) cout << x << endl;

    cout << endl << "Program is done";
    _getch();
    SetEvent(EndEvent);
}

