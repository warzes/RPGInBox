#pragma once

// Base class with copy constructor and assignment disabled. Polymorphic deletion via pointer to this class is forbidden -
// virtual destructor is omitted for performance reasons.
class NonCopyable
{
public:
	NonCopyable(const NonCopyable&) = delete;
	NonCopyable& operator=(const NonCopyable&) = delete;

protected:
	NonCopyable() = default;
	~NonCopyable() = default;
};

// Base class with move constructor and assignment disabled. Polymorphic deletion via pointer to this class is forbidden -
// virtual destructor is omitted for performance reasons
class NonMovable
{
public:
	NonMovable(NonMovable&&) = delete;
	NonMovable& operator=(NonMovable&&) = delete;

protected:
	NonMovable() = default;
	~NonMovable() = default;
};