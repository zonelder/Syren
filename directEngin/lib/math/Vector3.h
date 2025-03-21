#ifndef __SYREN_VECTOR3_H__
#define __SYREN_VECTOR3_H__
#include <DirectXMath.h>


class Vector3
{
	union
	{
		alignas(16) struct { float, x, y, z, _pad; };
		__m128 simd; 
	};
public:
	Vector3() : simd(_mm_setzero_ps()) {}
	Vector3(float x, float y, float z) : simd(_mm_set_ps(0.0f, z, y, x)) {}
	explicit Vector3(__m128 reg) : simd(reg) {}

	Vector3(Vector3&& other) noexcept = default;
	Vector3& operator=(Vector3&& other) noexcept = default;

	friend inline Vector3 operator+(const Vector3& v1, const Vector3& v2);
	friend inline Vector3 operator-(const Vector3& v1, const Vector3& v2);
	friend inline Vector3 operator*(const Vector3& v,float m);
	friend inline Vector3 operator*(float m, const Vector3& v);
	friend inline Vector3 operator/(const Vector3& v, float d);
	friend inline Vector3 operator/(float d, const Vector3& v);


	static inline float dot(const Vector3& v1, const Vector3& v2) noexcept;
	inline float dot(const Vector3& v) const noexcept;


	static inline Vector3 cross(const Vector3& v1, const Vector3& v2) noexcept;
	inline Vector3 cross(const Vector3& v1) const noexcept;

	Vector3 normalized() const& noexcept;
	Vector3 normalized() const&& noexcept;


	float operator[](size_t index) const noexcept
	{
		assert(index < 3 && "Vector3 index out of bound.");
		return (&x)[index];
	}

	float& operator[](size_t index) noexcept
	{
		assert(index < 3 && "Vector3 index out of bound.");
		return (&x)[index];
	}



};



Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
	return Vector3(_mm_add_ps(v1.simd, v2.simd));
}

Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
	return Vector3(_mm_sub_ps(v1.simd, v2.simd));
}

Vector3 operator*(const Vector3& v, float d)
{
	return Vector3(_mm_mul_ps(v.simd, _mm_set1_ps(d)));
}

Vector3 operator*(float d, const Vector3& v)
{
	return v * d;
}

Vector3 operator/(const Vector3& v, float d)
{
	assert(d != 0.0f && "Vector3 division by zero");
	return Vector3(_mm_div_ps(v.simd, _mm_set1_ps(d)));
}

Vector3 operator/(float d, const Vector3& v)
{
	return v / d;
}

float Vector3::dot(const Vector3& v1, const Vector3& v2) noexcept
{
	return _mm_cvtss_f32(_mm_dp_ps(v1.simd, v2.simd, 0x7F));
}

float Vector3::dot(const Vector3& v) const noexcept
{
	return dot(*this, v);
}

Vector3 Vector3::cross(const Vector3& a, const Vector3& b) noexcept
{
	const __m128 a_shuf = _mm_shuffle_ps(a.simd, a.simd, _MM_SHUFFLE(3, 0, 2, 1));
	const __m128 b_shuf = _mm_shuffle_ps(b.simd, b.simd, _MM_SHUFFLE(3, 0, 2, 1));
	return Vector3(_mm_shuffle_ps(
		_mm_sub_ps(
			_mm_mul_ps(a.simd, b_shuf),
			_mm_mul_ps(b.simd, a_shuf)
		),
		_mm_sub_ps(
			_mm_mul_ps(a.simd, b_shuf),
			_mm_mul_ps(b.simd, a_shuf)
		),
		_MM_SHUFFLE(3, 0, 2, 1)
	));
}

Vector3 Vector3::cross(const Vector3& v1) const noexcept
{
	return cross(*this, v1);
}


#endif