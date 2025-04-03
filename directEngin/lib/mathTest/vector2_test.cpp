#include "pch.h"
#include "vector2.h"

namespace TVector2 {

    TEST(testVector2Basic, CreateDefault) {
        Vector2 v;
        EXPECT_FLOAT_EQ(v[0], 0.0f);
        EXPECT_FLOAT_EQ(v[1], 0.0f);
    }

    TEST(testVector2Basic, CreateComponent) {
        Vector2 v(1.5f, -2.8f);
        EXPECT_FLOAT_EQ(v[0], 1.5f);
        EXPECT_FLOAT_EQ(v[1], -2.8f);
    }

    TEST(testVector2Basic, Equal) {
        Vector2 v(2.0f, 1.0f / 3.0f);
        Vector2 v2(2.0f, 1.0f / 3.0f);
        EXPECT_EQ(v, v2);
        EXPECT_EQ(v2, v);
    }

    TEST(testVector2Basic, Negate) {
        Vector2 v(1.0f, -2.0f);
        Vector2 result(-1.0f, 2.0f);
        EXPECT_EQ(-v, result);
    }

    TEST(testVector2Basic, Add) {
        Vector2 v(1.0f, 2.0f);
        Vector2 v2(3.0f, -1.0f);
        Vector2 result(4.0f, 1.0f);

        EXPECT_EQ(v + v2, result);
        v += v2;
        EXPECT_EQ(v, result);
    }

    TEST(testVector2Basic, Sub) {
        Vector2 v(5.0f, 4.0f);
        Vector2 v2(2.0f, 3.0f);
        Vector2 result(3.0f, 1.0f);

        EXPECT_EQ(v - v2, result);
        v -= v2;
        EXPECT_EQ(v, result);
    }

    TEST(testVector2Basic, Mult) {
        Vector2 v(1.5f, -2.0f);
        float scalar = 3.0f;
        Vector2 result(4.5f, -6.0f);

        EXPECT_EQ(v * scalar, result);
        EXPECT_EQ(scalar * v, result);
        v *= scalar;
        EXPECT_EQ(v, result);
    }

    TEST(testVector2Basic, Div) {
        Vector2 v(9.0f, -6.0f);
        float divisor = 3.0f;
        Vector2 result(3.0f, -2.0f);

        EXPECT_EQ(v / divisor, result);
        v /= divisor;
        EXPECT_EQ(v, result);

        EXPECT_DEATH(v / 0.0f, "");
        EXPECT_DEATH({ v /= 0.0f; }, "");
    }

    TEST(testVector2Function, Dot) {
        Vector2 a(2.0f, 3.0f);
        Vector2 b(4.0f, 5.0f);
        float expected = 23.0f;

        EXPECT_FLOAT_EQ(a.dot(b), expected);
        EXPECT_FLOAT_EQ(Vector2::dot(a, b), expected);
    }


    TEST(testVector2Function, Magnitude) 
    {
        Vector2 v(3.0f, 4.0f);
        constexpr float e = 0.00001f;

        EXPECT_NEAR(v.magnitude(), 5.0f, e);
        EXPECT_FLOAT_EQ(v.sqrMagnitude(), 25.0f);

        auto clamped = v.clampMagnitude(3.0f);
        EXPECT_NEAR(clamped.magnitude(), 3.0f, e);
    }

    TEST(testVector2Function, Normalized)
    {
        Vector2 v(3.0f, 4.0f);
        constexpr float e = 0.0001f;
        auto norm = v.normalized();

        EXPECT_NEAR(norm.magnitude(), 1.0f, e);
        EXPECT_NEAR(norm[0], 0.6f, e);
        EXPECT_NEAR(norm[1], 0.8f, e);

        Vector2 zeroVec;
        auto zeroNorm = zeroVec.normalized();
        EXPECT_EQ(zeroNorm, Vector2::zero);
    }

    TEST(testVector2Function, Perpendicular) {
        Vector2 a(2.0f, 3.0f);
        Vector2 perp = a.perpendicular();
        EXPECT_EQ(perp, Vector2(-3.0f, 2.0f));
    }

    TEST(testVector2Function, Distance) {
        Vector2 a(1.0f, 2.0f);
        Vector2 b(4.0f, 6.0f);
        float expected = 5.0f;

        EXPECT_FLOAT_EQ(Vector2::distance(a, b), expected);
    }

    TEST(testVector2Function, Lerp) {
        Vector2 a(1.0f, 2.0f);
        Vector2 b(4.0f, 6.0f);

        // Clamped
        EXPECT_EQ(Vector2::lerp(a, b, 0.5f), Vector2(2.5f, 4.0f));
        EXPECT_EQ(Vector2::lerp(a, b, 0.0f), a);
        EXPECT_EQ(Vector2::lerp(a, b, 1.0f), b);

        // Unclamped
        EXPECT_EQ(Vector2::lerpUnclamped(a, b, 2.0f), Vector2(7.0f, 10.0f));
    }

    TEST(testVector2Function, Reflect) {
        Vector2 incident(1.0f, -1.0f);
        Vector2 normal(0.0f, 1.0f);
        Vector2 reflected = Vector2::reflect(incident, normal);

        EXPECT_EQ(reflected, Vector2(1.0f, 1.0f));
    }

    TEST(testVector2Function, Project) {
        Vector2 v(5.0f, 5.0f);
        Vector2 onto(10.0f, 0.0f);
        Vector2 projection = Vector2::project(v, onto);

        EXPECT_EQ(projection, Vector2(5.0f, 0.0f));
    }

    TEST(testVector2Function, Angle) {
        Vector2 a(1.0f, 0.0f);
        Vector2 b(0.0f, 1.0f);

        EXPECT_NEAR(Vector2::angle(a, b), 90.0f, 0.0001f);
    }

    TEST(testVector2Function, Max) {
        Vector2 a(-3.0f, 4.0f);
        Vector2 b(0.0f, -12.0f);
        Vector2 result(0.0f, 4.0f);

        EXPECT_EQ(Vector2::max(a, b), result);
    }

    TEST(testVector2Function, Min) {
        Vector2 a(-3.0f, 4.0f);
        Vector2 b(0.0f, -12.0f);
        Vector2 result(-3.0f, -12.0f);

        EXPECT_EQ(Vector2::min(a, b), result);
    }

    TEST(testVector2Function, Scale) {
        Vector2 a(2.0f, 3.0f);
        Vector2 b(-1.0f, 4.0f);
        Vector2 result(-2.0f, 12.0f);

        EXPECT_EQ(a.scale(b), result);
        EXPECT_EQ(Vector2::scale(a, b), result);
    }

    TEST(testVector2Function, MoveTowards) {
        Vector2 current(1.0f, 1.0f);
        Vector2 target(4.0f, 5.0f);
        float maxDelta = 2.5f;
        Vector2 result = Vector2::moveTowards(current, target, maxDelta);

        Vector2 direction = target - current;
        direction = direction.normalized() * maxDelta;
        EXPECT_EQ(result, current + direction);
    }

    TEST(testVector2Constant, AllConstants) {
        EXPECT_EQ(Vector2::up, Vector2(0.0f, 1.0f));
        EXPECT_EQ(Vector2::down, Vector2(0.0f, -1.0f));
        EXPECT_EQ(Vector2::right, Vector2(1.0f, 0.0f));
        EXPECT_EQ(Vector2::left, Vector2(-1.0f, 0.0f));
        EXPECT_EQ(Vector2::one, Vector2(1.0f, 1.0f));
        EXPECT_EQ(Vector2::zero, Vector2(0.0f, 0.0f));
    }

} // namespace TVector2