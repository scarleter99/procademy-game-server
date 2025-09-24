#include "Object.h"
#include <iostream>

using namespace std;

void OneStar::Update()
{
	_x += 1;
	if (_x >= 75) {
		_x = 0;
		_delete = true;
	}
}

void OneStar::Render()
{
	for (int i = 0; i < _x; i++) {
		cout << " ";
	}

	cout << "*";
}

void TwoStar::Update()
{
	_x += 2;
	if (_x >= 75) {
		_x = 0;
		_delete = true;
	}
}

void TwoStar::Render()
{
	for (int i = 0; i < _x; i++) {
		cout << " ";
	}

	cout << "**";
}

void ThreeStar::Update()
{
	_x += 3;
	if (_x >= 75) {
		_x = 0;
		_delete = true;
	}
}

void ThreeStar::Render()
{
	for (int i = 0; i < _x; i++) {
		cout << " ";
	}

	cout << "***";
}
