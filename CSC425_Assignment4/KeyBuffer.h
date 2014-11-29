#include <map>

#pragma once
class KeyBuffer
{
public:
	KeyBuffer();
	~KeyBuffer();
	void set(unsigned char key);
	void setSpecial(int key);
	void unsetSpecial(int key);
	void unset(unsigned char key);
	void clear();
	void clearSpecial();
	bool isSet(unsigned char key);
	bool isSetSpecial(int key);
private:
	std::map<unsigned char, bool> keys;
	std::map<int, bool>keysSpecial;
};

