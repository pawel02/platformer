#pragma once
#include "Event.hpp"

class KeyPressedEvent : public BasicEvent
{
public:
	KeyPressedEvent(int keyCode) noexcept
		:keyCode{ keyCode }
	{}

	~KeyPressedEvent() = default;

	const int& getKeyCode() const noexcept { return keyCode; }

private:
	int keyCode;
};

class KeyReleasedEvent : public BasicEvent
{
public:
	KeyReleasedEvent(int keyCode) noexcept
		:keyCode{ keyCode }
	{}

	~KeyReleasedEvent() = default;

	const int& getKeyCode() const noexcept { return keyCode; }

private:
	int keyCode;
};