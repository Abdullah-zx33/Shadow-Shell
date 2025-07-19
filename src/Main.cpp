
#include<iostream>
#include "Utilities.h"
#include "shadowcli.h"
#include<stdlib.h>
#include<filesystem>
using namespace std;

int main() {
	//system("color 0A");  // Uncomment If you Like the Matrix Black back and Green Text! 
	bool exit = false;
	string cmd;
	animate("Welcome To Shadow Shell\nVersion 0.1\nType 'help' to View Commands\n");
	cout << endl;
	CLI obj{};
	obj.menu();
}
