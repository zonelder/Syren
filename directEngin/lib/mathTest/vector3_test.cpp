#include "pch.h"
#include "vector3.h"

TEST(testVector3Basic, CreateDefault)
{
	Vector3 v;
	EXPECT_EQ(v[0], 0.0f);
	EXPECT_EQ(v[1], 0.0f);
	EXPECT_EQ(v[2], 0.0f);
}


TEST(testVector3Basic, CreateComponent)
{
	Vector3 v(1.0f,2.0f,3.0f);
	EXPECT_EQ(v[0], 1.0f);
	EXPECT_EQ(v[1], 2.0f);
	EXPECT_EQ(v[2], 3.0f);
}

TEST(testVector3Basic, Equal)
{
	Vector3 v(1.0f, 2.0f, 3.0f);
	Vector3 v2(1.0f, 2.0f, 3.0f);
	EXPECT_EQ(v, v2);
	EXPECT_EQ(v2, v);
}

TEST(testVector3Basic, Negate)
{
	Vector3 v(1.0f, 2.0f, 3.0f);
	Vector3 result = Vector3(-1.0f, -2.0f, -3.0f);

	EXPECT_EQ(-v, result);
}

TEST(testVector3Basic, Add)
{
	Vector3 v(1.0f, 2.0f, 3.0f);
	Vector3 v2(0, 2.0f, -1.0f);
	Vector3 result = Vector3(1.0f, 4.0f, 2.0f);

	EXPECT_EQ(v + v2, result);
	EXPECT_EQ(v2 + v, result);
	v += v2;
	EXPECT_EQ(v, result);
}

TEST(testVector3Basic, Sub)
{
	Vector3 v(1.0f, 2.0f, 3.0f);
	Vector3 v2(0, 2.0f, -1.0f);
	Vector3 result(1.0f, 0.0f, 4.0f);


	EXPECT_EQ(v - v2, result);
	EXPECT_EQ(v2 - v, -result);
	v -= v2;
	EXPECT_EQ(v, result);
}

TEST(testVector3Basic, Mult)
{
	Vector3 v(1.0f, 2.0f, -3.0f);
	float d = 5.0f;
	Vector3 result(5.0f, 10.0f, -15.0f);
	EXPECT_EQ(v * d, result);
	EXPECT_EQ(d * v, result);
	v *= d;
	EXPECT_EQ(v, result);

}


TEST(testVector3Basic, Div)
{
	Vector3 v(9.0f, 18.0f, -3.0f);
	float d = 3.0f;
	Vector3 result(3.0f, 6.0f, -1.0f);
	EXPECT_EQ(v / d, result);
	v /= d;
	EXPECT_EQ(v, result);


	//low perfomance assert check
	EXPECT_DEATH(v / 0.0f,"");
	EXPECT_DEATH({ v /= 0.0f; }, "");
}


TEST(testVector3Function, Dot)
{
	Vector3 a(9.0f, 18.0f, -3.0f);
	Vector3 b(1.0f, 2.0f, -3.0f);
	float res = 54.0f;

	EXPECT_EQ(res, a.dot(b));
	EXPECT_EQ(res, Vector3::dot(a, b));
}

// Проверка векторного произведения
TEST(testVector3Function, Cross) {
	Vector3 a(1, 0, 0);
	Vector3 b(0, 1, 0);
	Vector3 result(0, 0, 1);

	EXPECT_EQ(a.cross(b), result);
	EXPECT_EQ(Vector3::cross(a, b), result);
}

TEST(testVector3Function, Magnitude)
{
	Vector3 a(-3.0f, 4.0f,5.0f);
	constexpr float e = 0.00001f;
	EXPECT_NEAR(a.magnitude(),7.07107f,e);
	EXPECT_EQ(a.sqrMagnitude(), 50.0f);
	
	auto b = a.clampMagnitude(5.0f);
	EXPECT_NEAR(b.magnitude(), 5.0f,e);
	EXPECT_NEAR(b.sqrMagnitude(), 25.0f,e);

}


TEST(testVector3Function, Normalized)
{
	Vector3 a(-3.0f, 4.0f, 5.0f);
	constexpr float e = 0.00001f;
	auto b = a.normalized();
	EXPECT_NEAR(b.sqrMagnitude(), 1.0f,e);
	const float mag = a.magnitude();
	EXPECT_NEAR(b[0], a[0] / mag,e);
	EXPECT_NEAR(b[1], a[1] / mag, e);
	EXPECT_NEAR(b[2], a[2] / mag, e);

	b = Vector3(0, 0, 0).normalized();
	EXPECT_NEAR(b.sqrMagnitude(), 0.0f, e);
}

TEST(testVector3Function, Distance) 
{
	Vector3 a(1, 2, 3);
	Vector3 b(4, 6, 3);
	float expected = 5.0f;

	EXPECT_FLOAT_EQ(Vector3::distance(a, b), expected);
}

TEST(testVector3Function, Lerp) 
{
	Vector3 a(1, 2, 3);
	Vector3 b(4, 5, 6);

	// Clamped
	EXPECT_EQ(Vector3::lerp(a, b, 0.5f), Vector3(2.5f, 3.5f, 4.5f));
	EXPECT_EQ(Vector3::lerp(a, b, 0.0f), a);
	EXPECT_EQ(Vector3::lerp(a, b, 1.0f), b);

	// Unclamped
	EXPECT_EQ(Vector3::lerpUnclamped(a, b, 2.0f), Vector3(7, 8, 9));
}
// Проверка отражения вектора
TEST(testVector3Function, Reflect) 
{
	Vector3 incident(1, -1, 0);
	Vector3 normal(0, 1, 0);
	Vector3 reflected = Vector3::reflect(incident, normal);

	EXPECT_EQ(reflected, Vector3(1, 1, 0));
}

// Проверка проекции вектора
TEST(testVector3Function, Project) {
	Vector3 v(5, 5, 0);
	Vector3 onto(10, 0, 0);
	Vector3 projection = Vector3::project(v, onto);

	EXPECT_EQ(projection, Vector3(5, 0, 0));
}

// Проверка угла между векторами
TEST(testVector3Function, Angle) {
	Vector3 a(1, 0, 0);
	Vector3 b(0, 1, 0);

	EXPECT_FLOAT_EQ(Vector3::angle(a, b), 90.0f);
}

/*
TEST(testVector3Function, Max)
{
	Vector3 a(-3.0f, 4.0f, 5.0f);
	Vector3 b(0.0f, -12.0f, 0.0f);
	Vector3 result(0.0f, 4.0f, 5.0f);
	EXPECT_EQ(result, Vector3::max(a, b));
	EXPECT_EQ(result, Vector3::max(b, a));
}

TEST(testVector3Function, Min)
{
	Vector3 a(-3.0f, 4.0f, 5.0f);
	Vector3 b(0.0f, -12.0f, 0.0f);
	Vector3 result(-3.0f, -12.0f, 0.0f);
	EXPECT_EQ(result, Vector3::min(a, b));
	EXPECT_EQ(result, Vector3::min(b, a));
}
*/
TEST(testVector3Function, Scale)
{
	Vector3 a(-3.0f, 4.0f, 5.0f);
	Vector3 b(0.0f, -12.0f, 0.0f);
	Vector3 result(0.0f, -48.0f, 0.0f);
	EXPECT_EQ(result, a.scale(b));
	EXPECT_EQ(result, Vector3::scale(b, a));
	EXPECT_EQ(result, Vector3::scale(a, b));
}

TEST(testVector3Function, MoveTowards) 
{
	Vector3 current(1, 2, 3);
	Vector3 target(4, 5, 6);
	Vector3 result = Vector3::moveToward(current, target, 3.0f);

	float distance = Vector3::distance(current, target); // ~5.196f
	EXPECT_FLOAT_EQ(result.magnitude(), (current + (target - current).normalized() * 3.0f).magnitude());
}