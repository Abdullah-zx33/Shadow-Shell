
#pragma once

#include<iostream>
#include<string>
#include<Windows.h>
using namespace std;

void animate(const char str[]) {

	for (int i = 0; str[i] != '\0'; i++) {
		cout << str[i];
		cout.flush();
		Sleep(40);
	}
}

void animate(const char str[], auto var) {

	for (int i = 0; str[i] != '\0'; i++) {
		cout << str[i];
		cout.flush();
		Sleep(40);
	}
	cout << var;
}

double input() {
	double temp;
	while (true) {
		cin >> temp;
		if (cin.fail()) {
			cout << "Please Enter a Numeric Value!";
			cin.clear();
			cin.ignore(1000, '\n');
		}
		else
			return temp;
	}
}
