#pragma once
#include <string>
#include <iostream>

class Vec2
{
public:
	Vec2(float x, float y);

	float getX() const noexcept;
	float getY() const noexcept;
	float operator[](unsigned int) const;
	friend std::ostream& operator<<(std::ostream&, const Vec2&);

private:
	float _value[2];
};

