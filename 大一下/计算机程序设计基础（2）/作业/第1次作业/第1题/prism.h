#include <iostream> 
using namespace std;
#ifndef PRISM_H
#define PRISM_H

class Prism {
private:
	int length, height, width;
public:
	void getData(){
		cout << "������ <��> <��> <��>���ո����)��";
		cin >> length >> width >> height;
	}
	int	getVolume(){
		return length * width * height;
	}
};

#endif
