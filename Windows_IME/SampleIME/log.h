#pragma once
#include <iostream>
#include <fstream>
#include <string>
template<typename T>
class CLog {
public:
    void TestLog(std::string name,const T& value) {
        std::ofstream ofs;
        ofs.open("C:\\Users\\tfa10\\OneDrive\\орн▒\\log.txt", std::ios::out | std::ios::app);
        if (!ofs.is_open()) {
            std::cout << "Failed to open file." << std::endl;
            return;
        }

        ofs << name<<value << std::endl;
        ofs.close();
    }
    void TestLog1(std::string name, const wchar_t* value) {
        std::wofstream wofs;
        wofs.open("C:\\Users\\USER\\Desktop\\log.txt", std::ios::out | std::ios::app);
        if (!wofs.is_open()) {
            std::wcout << L"Failed to open file." << std::endl;
            return;
        }

        wofs << std::wstring(name.begin(), name.end()) << value << std::endl;
        wofs.close();
    }
};