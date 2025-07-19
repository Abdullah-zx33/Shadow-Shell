
#define _CRT_SECURE_NO_WARNINGS
#include "shadowcli.h"

#include <iostream>
#include <fstream>
#include <ctime>
#include <Windows.h>
#undef byte
#include <filesystem>
#include <conio.h>
#include <direct.h>    
#include <cstddef>
#include <sstream>
#include <limits>

namespace fs = std::filesystem;
using namespace std;


string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\n\r");
    size_t end = s.find_last_not_of(" \t\n\r");
    return (start == string::npos) ? "" : s.substr(start, end - start + 1);
}

void CLI::clear() {
    system("cls");
}

void CLI::menu() {
    do {
        cout << endl << fs::current_path().string() << "> ";
        if (!getline(cin, cmd)) {
            cout << "\n[!] End Of Input (EOF) detected. Exiting Shadow CLI..." << endl;
            break;
        }

        istringstream iss(cmd);
        string command;
        iss >> command;
        string argument;
        getline(iss, argument);
        argument = trim(argument);

        if (command == "cls") {
            clear();
            cout << "Shadow Shell :: V0.1\nCurrent Dir: " << fs::current_path().string() << endl;
            cout << "====================\n";
        }
        else if (command == "help") {
            cout << "\n========== Shadow Shell Help ==========\n"
                << "  cls         - Clear the screen\n"
                << "  theme       - Theme Selection Menu\n"
                << "  calculate   - Open calculator\n"
                << "  log         - Enter raw text\n"
                << "  time        - Display current system time\n"
                << "  stop_watch  - Start stopwatch\n"
                << "  about       - About CLI\n"
                << "  exit        - Exit Shadow CLI\n"
                << "  cd <dir>    - Change directory\n"
                << "  create      - For Path Creation\n"
                << "  dir         - List current directory\n"
                << "  del <file>  - Delete file or folder\n"
                << "======================================\n"
                << "More functions coming soon!\n"
                << "Thanks for using Shadow Shell.\n" << endl;
        }
        else if (command == "about" || command == "version")
            this->about();

        else if (command == "exit") {
            cout << "\n-> Exiting" << endl;
            exit = true;
            break;
        }

        else if (command == "create")
            this->create();

        else if (command == "stop_watch" || command == "stopwatch" || (command == "stop" && argument == "watch"))
            this->watch();

        else if (command == "time") {
            time_t now = time(0);
            cout << "Time Right Now is!\n\t" << ctime(&now);
        }

        else if (command == "calculate" || command == "calculator")
            this->calculate();

        else if (command == "del" || command == "delete") {
            if (argument.empty()) {
                cout << "[!] Please provide a file or folder to delete.\n";
            }
            else {
                this->deleteEntry(argument);
            }
        }

        else if (command == "log") {
            ofstream file("log.txt", ios::app);
            if (!file.is_open()) {
                cout << "Error! Log Was not Saved! [Index: File Opening Problem]" << endl;
                continue;
            }

            cout << "[Log Mode] Type your message below.\n";
            cout << "Type [ 'end' in New Line ] to finish.\n\n=> ";

            string fullLog, logline;
            while (getline(cin, logline)) {
                if (logline == "end") break;
                fullLog += logline + "\n";
            }

            file << fullLog;
            cout << "====> Log Saved Successfully\n";
            file.close();
        }

        else if (command == "theme")
            this->theme();

        else if (command == "dir")
            this->dir();

        else if (command == "cd") {
            if (argument.empty()) {
                cout << "[!] Please specify a directory.\n";
            }
            else {
                this->cd(argument);
            }
        }

        else {
            cout << "[*] Unknown command. Run as system command? (y/n): ";
            string confirm;
            getline(cin, confirm);
            if (confirm == "y" || confirm == "Y") {
                int result = system(cmd.c_str());
                if (result == -1)
                    cout << "[ERROR] Failed to execute: '" << cmd << "'\n";
            }
        }

    } while (!exit);
}

void CLI::create() {
    string folderName;
    cout << "\nEnter Folder Name: ";
    getline(cin, folderName);

    fs::path path_final = fs::current_path() / folderName;

    try {
        if (!fs::exists(path_final)) {
            fs::create_directories(path_final);
            cout << "[+] Path Successfully Created: " << path_final << endl;
        }
        else {
            cout << "[!] Path Already Exists: " << path_final << endl;
        }
    }
    catch (const fs::filesystem_error& e) {
        cerr << "[!] Error creating path: " << e.what() << endl;
    }
}

void CLI::cd(const string& pathRaw) {
    string path = pathRaw;


    if (!path.empty() && path.front() == '"' && path.back() == '"')
        path = path.substr(1, path.size() - 2);

    try {
        fs::path resolved = fs::absolute(path);  
        if (!fs::exists(resolved) || !fs::is_directory(resolved)) {
            cout << "[!] Invalid directory: " << resolved << endl;
            return;
        }

        fs::current_path(resolved); 
        cout << "Changed directory to: " << fs::current_path().string() << endl;
    }
    catch (const fs::filesystem_error& e) {
        cerr << "[!] Failed to change directory: " << e.what() << endl;
    }
}


void CLI::dir() {
    string currentDir = fs::current_path().string();
    cout << "\nDirectory Listing: " << currentDir << "\n\n";

    for (const auto& entry : fs::directory_iterator(currentDir)) {
        cout << (entry.is_directory() ? "[DIR]  " : "       ");
        cout << entry.path().filename().string() << '\n';
    }
}

void CLI::watch() {
    cout << "\nPress Enter To Start Stop Watch! "; cin.ignore();
    time_t now = time(0);
    char buffer[99], buffer2[99];
    strftime(buffer, 99, "%H : %M : %S", localtime(&now));
    cout << "Time Right Now\n\t" << buffer << endl;
    cout << "\nPress Enter to Stop!" << endl << endl;

    for (int i = 0; i < 10000; ++i) {
        cout << "\rTime: " << i + 1 << " sec   "; cout.flush();
        Sleep(1000);
        if (_kbhit()) { (void)_getch(); break; }
    }

    time_t end = time(0);
    strftime(buffer2, 99, "%H : %M : %S", localtime(&end));
    cout << "\n\nTime Right Now\n\t" << buffer2 << endl;
}

void CLI::theme() {
    cout << "\nChoose a Theme:\n1- Green (Matrix)\n2- Red\n3- Blue\n4- Purple\n5- Aqua\n6- Yellow\n7- White\n8- Gray\n9- Pink\n0- Exit Theme Selection\n";
    string themeChoice;
    do {
        cout << "\nEnter theme number: ";
        getline(cin, themeChoice);
        applyTheme(themeChoice);
    } while (themeChoice != "0");
}

void CLI::applyTheme(const string& choice) {
    if (choice == "1") system("color 0A");
    else if (choice == "2") system("color 0C");
    else if (choice == "3") system("color 0B");
    else if (choice == "4") system("color 0D");
    else if (choice == "5") system("color 03");
    else if (choice == "6") system("color 0E");
    else if (choice == "7") system("color 0F");
    else if (choice == "8") system("color 08");
    else if (choice == "9") system("color 0D");
    else if (choice == "shell") system("color 90");
    else if (choice == "0") cout << "Done!\n";
    else cout << "[!] Invalid Theme Selection\n";
}

void CLI::about() {
    cout << "\nShadow Shell Version 0.1\nDeveloped by:  Muhammad Abdullah\nBuild Date:   19-July-2025\nCompiled with: Standard C++\n";
}

void CLI::calculate() {
    double num1, num2;
    char op;

    cout << "\n[Calculator Mode]\nEnter first number: ";
    num1 = response();

    cout << "\nEnter operator (+, -, *, /): ";
    cin >> op; cin.ignore(1000, '\n');

    cout << "\nEnter second number: ";
    num2 = response();

    double result;
    bool valid = true;

    switch (op) {
    case '+': result = num1 + num2; break;
    case '-': result = num1 - num2; break;
    case '*': result = num1 * num2; break;
    case '/':
        if (num2 == 0) {
            cout << "\nError: Division by zero is not allowed!\n";
            valid = false;
        }
        else result = num1 / num2;
        break;
    default:
        cout << "\nInvalid operator!\n"; valid = false;
    }

    if (valid)
        cout << "\n=> " << num1 << " " << op << " " << num2 << " = " << result << "\n";
}

void CLI::deleteEntry(const string& target) {
    try {
        cout << "Are you sure you want to delete '" << target << "'? (y/n): ";
        string confirm;
        getline(cin, confirm);
        if (confirm != "y" && confirm != "Y") {
            cout << "Aborted.\n";
            return;
        }

        fs::path p = fs::current_path() / target;
        if (!fs::exists(p)) {
            cout << "[!] Target not found: " << target << endl;
        }
        else if (fs::is_directory(p)) {
            fs::remove_all(p);
            cout << "[~] Folder deleted: " << p << endl;
        }
        else {
            fs::remove(p);
            cout << "[~] File deleted: " << p << endl;
        }
    }
    catch (const fs::filesystem_error& e) {
        cerr << "[!] Deletion failed: " << e.what() << endl;
    }
}

double response() {
    double temp;
    while (true) {
        cin >> temp;
        if (cin.fail()) {
            cout << "Please Enter a Numeric Value!\n";
            cin.clear();
            cin.ignore(1000, '\n');
        }
        else {
            cin.ignore(1000, '\n');
            return temp;
        }
    }
}
