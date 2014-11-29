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

void KeyBuffer::setSpecial(int key)
{
	this->keysSpecial[key] = true;
}

void KeyBuffer::unset(unsigned char key)
{
	this->keys[key] = false;
}

void KeyBuffer::unsetSpecial(int key)
{
	this->keysSpecial[key] = false;
}

void KeyBuffer::clear()
{
	this->keys.clear();
}

void KeyBuffer::clearSpecial()
{
	this->keysSpecial.clear();
}

bool KeyBuffer::isSet(unsigned char key)
{
	if (this->keys.find(key) != this->keys.end())
	{
		return this->keys[key];
	}
	return false;
}

bool KeyBuffer::isSetSpecial(int key)
{
	if (this->keysSpecial.find(key) != this->keysSpecial.end())
	{
		return this->keysSpecial[key];
	}
	return false;
}