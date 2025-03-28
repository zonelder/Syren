#include "pch.h"
#include "quaternion.h"
#include <numbers>


namespace TQuaternion
{


TEST(QuaternionBasic, Identity) {
    Quaternion q;
    EXPECT_FLOAT_EQ(q[0], 0);
    EXPECT_FLOAT_EQ(q[1], 0);
    EXPECT_FLOAT_EQ(q[2], 0);
    EXPECT_FLOAT_EQ(q[3], 1);
    EXPECT_EQ(q, Quaternion::identity);
}

TEST(QuaternionOperations, Multiplication) {
    Quaternion q1(0.5, 0.5, 0.5, 0.5);
    Quaternion q2(0, 0, 0, 1);
    Quaternion result = q1 * q2;

    EXPECT_EQ(q1, result);
}

TEST(QuaternionConversions, AngleAxis) {
    Vector3 axis(0, 1, 0);
    Quaternion q = Quaternion::angleAxis(90, axis);

    EXPECT_NEAR(q.angle(), 90, 0.001f);
    Vector3 result_axis = q.axis().normalized();
    EXPECT_EQ(result_axis, axis);
}

TEST(QuaternionFunction, Inverse) {
    Quaternion q(0.5, 0.5, 0.5, 0.5);
    Quaternion inv = q.inverse();

    Quaternion result = q * inv;
    EXPECT_EQ(result, Quaternion::identity);
}

TEST(QuaternionEdgeCases, IndexOperator) {
    Quaternion q(1.1f, 2.2f, 3.3f, 4.4f);

    // Проверка корректности индексации
    EXPECT_FLOAT_EQ(q[0], 1.1f);
    EXPECT_FLOAT_EQ(q[1], 2.2f);
    EXPECT_FLOAT_EQ(q[2], 3.3f);
    EXPECT_FLOAT_EQ(q[3], 4.4f);

    // Проверка на выход за границы
    EXPECT_DEATH(q[4], "Quaternion index out of bound.");
}

TEST(QuaternionFunction, Normalization) {
    Quaternion q(1, 2, 3, 4);
    Quaternion normalized = q.normalized();

    const float expected_magnitude = 1.0f;
    const float sqr_sum = 1 * 1 + 2 * 2 + 3 * 3 + 4 * 4;
    const float scale = 1.0f / sqrt(sqr_sum);
    EXPECT_NEAR(normalized.magnitude(), expected_magnitude,0.0001f);
}

TEST(QuaternionOperations, BasicRotation) {
    // Поворот на 90 градусов вокруг оси Y
    Quaternion q = Quaternion::euler(0, 90, 0);
    Vector3 vec(0, 0, 1); // Вектор вперед

    Vector3 rotated = q * vec;
    EXPECT_EQ(rotated, Vector3(1, 0, 0));
}

TEST(QuaternionOperations, FullCircle) {
    // Поворот на 360 градусов вокруг произвольной оси
    Quaternion q = Quaternion::angleAxis(360, Vector3(1, 1, 1).normalized());
    Vector3 vec(2, 3, 5);

    Vector3 rotated = q * vec;
    EXPECT_EQ(rotated, vec);
}

TEST(QuaternionOperations, InverseRotation) {
    // Поворот вперед и обратный поворот
    Quaternion rot = Quaternion::euler(0, 45, 0);
    Quaternion inv_rot = rot.inverse();
    Vector3 vec(1, 2, 3);

    Vector3 rotated = rot * vec;
    Vector3 restored = inv_rot * rotated;

    EXPECT_TRUE(restored == vec);
}

TEST(QuaternionComparison, ExactEquality) {
    Quaternion q1(0.5f, 0.5f, 0.5f, 0.5f);
    Quaternion q2(0.5f, 0.5f, 0.5f, 0.5f);
    Quaternion q3(0.5f, 0.5f, 0.5f, -0.5f);

    EXPECT_TRUE(q1 == q2);
    EXPECT_FALSE(q1 == q3);
}

TEST(QuaternionComparison, NearEquality) {
    Quaternion q1 = Quaternion::euler(0, 45, 0);
    Quaternion q2 = Quaternion::euler(0, 45.0001f, 0);
    EXPECT_EQ(q1, q2);
}

TEST(QuaternionComparison, DoubleCover) {
    // Q и -Q представляют одинаковое вращение
    Quaternion q1 = Quaternion::euler(30, 15, 45);
    Quaternion q2(-q1[0], -q1[1], -q1[2], -q1[3]);

    // Но они не равны по компонентам
    EXPECT_FALSE(q1 == q2);

    // Проверка через вращение вектора
    Vector3 vec(1, 2, 3);
    Vector3 rotated1 = q1 * vec;
    Vector3 rotated2 = q2 * vec;

    EXPECT_TRUE(rotated1 == rotated2);
}

TEST(QuaternionConversions, EulerAnglesBasic) {
    // Тестирование базовых поворотов по каждой оси
    Quaternion qX = Quaternion::euler(90, 0, 0);
    Vector3 anglesX = qX.eulerAngles();
    EXPECT_NEAR(anglesX[0], 90, 0.001f);
    EXPECT_NEAR(anglesX[1], 0, 0.001f);
    EXPECT_NEAR(anglesX[2], 0, 0.001f);

    Quaternion qY = Quaternion::euler(0, 45, 0);
    Vector3 anglesY = qY.eulerAngles();
    EXPECT_NEAR(anglesY[1], 45, 0.001f);

    Quaternion qZ = Quaternion::euler(0, 0, 30);
    Vector3 anglesZ = qZ.eulerAngles();
    EXPECT_NEAR(anglesZ[2], 30, 0.001f);
}

TEST(QuaternionConversions, EulerAnglesCombined) {
    // Комбинированные повороты
    Quaternion q = Quaternion::euler(30, 45, 60);
    Vector3 angles = q.eulerAngles();

    EXPECT_NEAR(angles[0], 30, 0.5f);
    EXPECT_NEAR(angles[1], 45, 0.5f);
    EXPECT_NEAR(angles[2], 60, 0.5f);
}

TEST(QuaternionConversions, SIMD_EdgeCases) {
    // Граничные значения Pitch
    Quaternion q1 = Quaternion::euler(0, 89.9999f, 0);
    Vector3 a1 = q1.eulerAngles();
    EXPECT_NEAR(a1[1], 89.9999f, 0.001f);

    // Сингулярность
    Quaternion q2 = Quaternion::euler(30, 90, 60);
    Vector3 a2 = q2.eulerAngles();
    EXPECT_NEAR(a2[1], 90.0f, 0.001f);
    EXPECT_NEAR(a2[0] + a2[2], 90.0f, 0.5f);
}

TEST(QuaternionConversions, SIMD_Precision) {
    // Точность при малых углах
    Quaternion q = Quaternion::euler(0.001f, 0.002f, 0.003f);
    Vector3 angles = q.eulerAngles();
    EXPECT_EQ(angles,Vector3(0.001f, 0.002f, 0.003f));
}

TEST(QuaternionConversions, GimbalLockCase) {
    // Проверка сингулярности (gimbal lock)
    Quaternion q = Quaternion::euler(0, 90, 30);
    Vector3 angles = q.eulerAngles();

    // Ожидаем нормализацию углов
    EXPECT_NEAR(angles[1], 90, 0.001f);
    EXPECT_TRUE((angles[0] < 0.1f || angles[2] < 0.1f));
}

TEST(QuaternionConversions, NegativeAngles) {
    // Отрицательные углы
    Quaternion q = Quaternion::euler(-45, -30, 180);
    Vector3 angles = q.eulerAngles();

    EXPECT_NEAR(angles[0], 315, 0.1f);  // -45 + 360
    EXPECT_NEAR(angles[1], 330, 0.1f);  // -30 + 360
    EXPECT_NEAR(angles[2], 180, 0.1f);
}

TEST(QuaternionConversions, FullCircle) {
    // Поворот на 360 градусов
    Quaternion q = Quaternion::euler(360, 0, 360);
    Vector3 angles = q.eulerAngles();

    EXPECT_NEAR(angles[0], 0, 0.001f);
    EXPECT_NEAR(angles[1], 0, 0.001f);
    EXPECT_NEAR(angles[2], 0, 0.001f);
}

TEST(QuaternionConversions, QuaternionToEulerAndBack) {
    // Конвертация туда-обратно
    Vector3 original(30, 45, 60);
    Quaternion q = Quaternion::euler(original[0], original[1], original[2]);
    Vector3 converted = q.eulerAngles();

    // Допуск увеличен из-за потери точности при конвертациях
    EXPECT_EQ(converted,original);
}


}