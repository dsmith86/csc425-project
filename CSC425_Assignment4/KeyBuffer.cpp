#include "KeyBuffer.h"


KeyBuffer::KeyBuffer()
{
	this->keys = std::map<unsigned char, bool>();
}


KeyBuffer::~KeyBuffer()
{
}


void KeyBuffer::set(unsigned char key)
{
	this->keys[key] = true;
}

void KeyBuffer::unset(unsigned char key)
{
	this->keys[key] = false;
}

void KeyBuffer::clear()
{
	this->keys.clear();
}

bool KeyBuffer::isSet(unsigned char key)
{
	if (this->keys.find(key) != this->keys.end())
	{
		return this->keys[key];
	}
	return false;
}