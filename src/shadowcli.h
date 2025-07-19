#pragma once

#include <string>
#include <iostream>
#include <limits> 

class CLI {
private:
    std::string cmd;
    bool exit = false;
    std::string logline;

public:
    CLI() = default;

    void create();
    void menu();
    void clear();
    void dir();
    void cd(const std::string& pathRaw);
    void about();
    void watch();
    void theme();
    void applyTheme(const std::string& choice);
    void calculate();
    void deleteEntry(const std::string& target);
};


double response();
