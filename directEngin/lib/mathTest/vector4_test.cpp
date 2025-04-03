#include "pch.h"
#include "vector4.h"


namespace TVector4
{
    // Тесты базовой функциональности
    TEST(Vector4Test, Constructors)
    {
        Vector4 v1;
        EXPECT_EQ(v1, Vector4::zero);

        Vector4 v2(1.5f, 2.5f, 3.5f, 4.5f);
        EXPECT_FLOAT_EQ(v2[0], 1.5f);
        EXPECT_FLOAT_EQ(v2[1], 2.5f);
        EXPECT_FLOAT_EQ(v2[2], 3.5f);
        EXPECT_FLOAT_EQ(v2[3], 4.5f);
    }

    // Тесты арифметических операций
    TEST(Vector4Test, ArithmeticOperations)
    {
        Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
        Vector4 b(4.0f, 5.0f, 6.0f, 7.0f);

        // Сложение
        Vector4 sum = a + b;
        EXPECT_EQ(sum, Vector4(5.0f, 7.0f, 9.0f, 11.0f));

        // Вычитание
        Vector4 diff = a - b;
        EXPECT_EQ(diff, Vector4(-3.0f, -3.0f, -3.0f, -3.0f));

        // Скалярное умножение
        Vector4 scaled1 = a * 2.0f;
        Vector4 scaled2 = 2.0f * a;
        EXPECT_EQ(scaled1, Vector4(2.0f, 4.0f, 6.0f, 8.0f));
        EXPECT_EQ(scaled2, Vector4(2.0f, 4.0f, 6.0f, 8.0f));

        // Деление
        Vector4 div = a / 2.0f;
        EXPECT_EQ(div, Vector4(0.5f, 1.0f, 1.5f, 2.0f));
    }

    // Тесты составных операторов
    TEST(Vector4Test, CompoundOperators)
    {
        Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
        a += Vector4(0.5f, 1.0f, 1.5f, 2.0f);
        EXPECT_EQ(a, Vector4(1.5f, 3.0f, 4.5f, 6.0f));

        a -= Vector4(0.5f, 1.0f, 1.5f, 2.0f);
        EXPECT_EQ(a, Vector4(1.0f, 2.0f, 3.0f, 4.0f));

        a *= 3.0f;
        EXPECT_EQ(a, Vector4(3.0f, 6.0f, 9.0f, 12.0f));

        a /= 3.0f;
        EXPECT_EQ(a, Vector4(1.0f, 2.0f, 3.0f, 4.0f));
    }

    // Тесты специальных методов
    TEST(Vector4Test, SpecialMethods)
    {
        Vector4 v(3.0f, 4.0f, 0.0f, 0.0f);

        // Длина
        EXPECT_FLOAT_EQ(v.magnitude(), 5.0f);
        EXPECT_FLOAT_EQ(v.sqrMagnitude(), 25.0f);

        // Нормализация
        Vector4 norm = v.normalized();
        EXPECT_NEAR(norm.magnitude(), 1.0f, Vector4::s_epsilon);

        // Ограничение длины
        Vector4 clamped = Vector4(10.0f, 0.0f, 0.0f, 0.0f).clampMagnitude(5.0f);
        EXPECT_FLOAT_EQ(clamped.magnitude(), 5.0f);
    }

    // Тесты векторных операций
    TEST(Vector4Test, VectorOperations)
    {
        Vector4 a(1.0f, 0.0f, 0.0f, 0.0f);
        Vector4 b(0.0f, 1.0f, 0.0f, 0.0f);

        // Скалярное произведение
        EXPECT_FLOAT_EQ(Vector4::dot(a, b), 0.0f);

        // Проекция
        Vector4 proj = Vector4::project(Vector4(2.0f, 2.0f, 0.0f, 0.0f), a);
        EXPECT_EQ(proj, Vector4(2.0f, 0.0f, 0.0f, 0.0f));

        // Отражение
        Vector4 refl = Vector4::reflect(Vector4(1.0f, -1.0f, 0.0f, 0.0f), b);
        EXPECT_NEAR(refl[0], 1.0f, 1e-5f);
        EXPECT_NEAR(refl[1], 1.0f, 1e-5f);
    }

    // Тесты сравнения и равенства
    TEST(Vector4Test, ComparisonOperations)
    {
        Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
        Vector4 b(1.0f + Vector4::s_epsilon / 2,
            2.0f + Vector4::s_epsilon / 2,
            3.0f + Vector4::s_epsilon / 2,
            4.0f + Vector4::s_epsilon / 2);

        EXPECT_TRUE(a == b);
        EXPECT_FALSE(Vector4::equal(a, b));
    }

    // Тесты граничных случаев
    TEST(Vector4Test, EdgeCases)
    {
        // Нулевой вектор
        Vector4 zero;
        EXPECT_FLOAT_EQ(zero.magnitude(), 0.0f);

        // Деление на ноль
        Vector4 v(1.0f, 2.0f, 3.0f, 4.0f);
        EXPECT_DEATH({ v = v / 0.0f; }, "");

        // Нормализация нулевого вектора
        EXPECT_NO_THROW(zero.normalized());
    }


    // Тесты оператора индексации
    TEST(Vector4Test, IndexOperator)
    {
        Vector4 v(1.1f, 2.2f, 3.3f, 4.4f);

        EXPECT_FLOAT_EQ(v[0], 1.1f);
        EXPECT_FLOAT_EQ(v[1], 2.2f);
        EXPECT_FLOAT_EQ(v[2], 3.3f);
        EXPECT_FLOAT_EQ(v[3], 4.4f);

        v[1] = 5.5f;
        EXPECT_FLOAT_EQ(v[1], 5.5f);

        EXPECT_DEATH({ v[2] = v[4]; }, "");
    }

    // Тесты конвертации в XMVECTOR
    TEST(Vector4Test, XMVECTOR_Conversion) {
        Vector4 v(1.0f, 2.0f, 3.0f, 4.0f);
        DirectX::XMVECTOR xm = v;

        EXPECT_FLOAT_EQ(DirectX::XMVectorGetX(xm), 1.0f);
        EXPECT_FLOAT_EQ(DirectX::XMVectorGetY(xm), 2.0f);
        EXPECT_FLOAT_EQ(DirectX::XMVectorGetZ(xm), 3.0f);
        EXPECT_FLOAT_EQ(DirectX::XMVectorGetW(xm), 4.0f);
    }

    // Тесты покомпонентного умножения
    TEST(Vector4Test, ComponentWiseScale)
    {
        Vector4 a(2.0f, 3.0f, 4.0f, 5.0f);
        Vector4 b(0.5f, 2.0f, 1.5f, 0.2f);

        Vector4 scaled = Vector4::scale(a, b);
        EXPECT_EQ(scaled, Vector4(1.0f, 6.0f, 6.0f, 1.0f));
    }

    // Тесты движения к цели
    TEST(Vector4Test, MoveTowards) 
    {
        Vector4 start(1.0f, 2.0f, 3.0f, 4.0f);
        Vector4 target(5.0f, 6.0f, 7.0f, 8.0f);

        // Движение на половину расстояния
        Vector4 result = Vector4::moveTowards(start, target, 0.5f);
        EXPECT_NEAR(result[0], 1.25f, 0.01f);
        EXPECT_NEAR(result[1], 2.25f, 0.01f);
        EXPECT_NEAR(result[2], 3.25f, 0.01f);
        EXPECT_NEAR(result[3], 4.25f, 0.01f);
    }

    // Тесты вычисления угла
    TEST(Vector4Test, AngleCalculation)
    {
        Vector4 a(1.0f, 0.0f, 0.0f, 0.0f);
        Vector4 b(0.0f, 1.0f, 0.0f, 0.0f);

        float angle = Vector4::angle(a, b);
        EXPECT_NEAR(angle, 90.0f, 0.01f);
    }

    // Расширенные тесты интерполяции
    TEST(Vector4Test, ExtendedLerp)
    {
        Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
        Vector4 b(5.0f, 6.0f, 7.0f, 8.0f);

        // t < 0
        Vector4 res1 = Vector4::lerp(a, b, -1.0f);
        EXPECT_EQ(res1, a);

        // t > 1
        Vector4 res2 = Vector4::lerp(a, b, 2.0f);
        EXPECT_EQ(res2, b);

        // Неклампнутая интерполяция
        Vector4 res3 = Vector4::lerpUnclamped(a, b, 0.5f);
        EXPECT_EQ(res3, Vector4(3.0f, 4.0f, 5.0f, 6.0f));
    }

    // Тесты max/min компонентов
    TEST(Vector4Test, ComponentMaxMin) {
        Vector4 a(1.0f, 5.0f, 3.0f, 7.0f);
        Vector4 b(4.0f, 2.0f, 6.0f, 5.0f);

        Vector4 maxRes = Vector4::max(a, b);
        EXPECT_EQ(maxRes, Vector4(4.0f, 5.0f, 6.0f, 7.0f));

        Vector4 minRes = Vector4::min(a, b);
        EXPECT_EQ(minRes, Vector4(1.0f, 2.0f, 3.0f, 5.0f));
    }


    // Тесты статических констант
    TEST(Vector4Test, StaticConstants)
    {
        EXPECT_EQ(Vector4::one, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
        EXPECT_EQ(Vector4::zero, Vector4(0.0f, 0.0f, 0.0f, 0.0f));
    }
}