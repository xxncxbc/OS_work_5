#include <windows.h>
#include <iostream>
#include <vector>
using namespace std;


int main(){
    HANDLE ReadPipe1, WritePipe1, ReadPipe2, WritePipe2; 

    // Создание анонимных каналов
    if (!CreatePipe(&ReadPipe1, &WritePipe1, NULL, 0)) {
        cerr << "Error creating channel number 1" << endl;
        return 1;
    }

    if (!CreatePipe(&ReadPipe2, &WritePipe2, NULL, 0)) {
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
    WaitForSingleObject(pi.hThread, INFINITE);

}

