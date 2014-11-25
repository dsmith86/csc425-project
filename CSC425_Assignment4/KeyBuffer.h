#include <map>

#pragma once
class KeyBuffer
{
public:
	KeyBuffer();
	~KeyBuffer();
	void set(unsigned char key);
	void unset(unsigned char key);
	void clear();
	bool isSet(unsigned char key);
private:
	std::map<unsigned char, bool> keys;
};

