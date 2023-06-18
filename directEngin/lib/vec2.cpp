#include "vec2.h"


Vec2::Vec2(float x,float y){
	_value[0] = x;
	_value[1] = y;
}


float Vec2::getX() const noexcept {
	return _value[0];
}

float Vec2::getY() const noexcept {
	return _value[1];
}

float Vec2::operator[](unsigned int index) const {
	return _value[index];
}


std::ostream& operator<<(std::ostream&out,const Vec2& vec){

	for (unsigned int i = 0; i < sizeof(vec._value) / sizeof(float); ++i) {
		out<< vec._value[i] << " , ";
	}

	return out;
}
