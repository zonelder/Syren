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

        // �������� ������������ ����������
        EXPECT_FLOAT_EQ(q[0], 1.1f);
        EXPECT_FLOAT_EQ(q[1], 2.2f);
        EXPECT_FLOAT_EQ(q[2], 3.3f);
        EXPECT_FLOAT_EQ(q[3], 4.4f);

        // �������� �� ����� �� �������
        EXPECT_DEATH(q[4], "Quaternion index out of bound.");
    }

    TEST(QuaternionFunction, Normalization) {
        Quaternion q(1, 2, 3, 4);
        Quaternion normalized = q.normalized();

        const float expected_magnitude = 1.0f;
        const float sqr_sum = 1 * 1 + 2 * 2 + 3 * 3 + 4 * 4;
        const float scale = 1.0f / sqrt(sqr_sum);
        EXPECT_NEAR(normalized.magnitude(), expected_magnitude, 0.0001f);
    }

    TEST(QuaternionOperations, BasicRotation) {
        // ������� �� 90 �������� ������ ��� Y
        Quaternion q = Quaternion::euler(0, 90, 0);
        Vector3 vec(0, 0, 1); // ������ ������

        Vector3 rotated = q * vec;
        EXPECT_EQ(rotated, Vector3(1, 0, 0));
    }

    TEST(QuaternionOperations, FullCircle) {
        // ������� �� 360 �������� ������ ������������ ���
        Quaternion q = Quaternion::angleAxis(360, Vector3(1, 1, 1).normalized());
        Vector3 vec(2, 3, 5);

        Vector3 rotated = q * vec;
        EXPECT_EQ(rotated, vec);
    }

    TEST(QuaternionOperations, InverseRotation) {
        // ������� ������ � �������� �������
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
        // Q � -Q ������������ ���������� ��������
        Quaternion q1 = Quaternion::euler(30, 15, 45);
        Quaternion q2(-q1[0], -q1[1], -q1[2], -q1[3]);

        // �� ��� �� ����� �� �����������
        EXPECT_FALSE(q1 == q2);

        // �������� ����� �������� �������
        Vector3 vec(1, 2, 3);
        Vector3 rotated1 = q1 * vec;
        Vector3 rotated2 = q2 * vec;

        EXPECT_TRUE(rotated1 == rotated2);
    }
    TEST(QuaternionConversions, EulerAnglesBasic) 
    {
        // ������� �������� (��������� ���� ��������������)
        EXPECT_EQ(Quaternion::euler(90, 0, 0), Quaternion::euler(Quaternion::euler(90, 0, 0).eulerAngles()));
        EXPECT_EQ(Quaternion::euler(0, 45, 0), Quaternion::euler(Quaternion::euler(0, 45, 0).eulerAngles()));
        EXPECT_EQ(Quaternion::euler(0, 0, 30), Quaternion::euler(Quaternion::euler(0, 0, 30).eulerAngles()));
    }

    TEST(QuaternionConversions, EulerAnglesCombined) 
    {
        // ��������������� �������
        Quaternion q = Quaternion::euler(30, 45, 60);
        auto q1 = Quaternion::euler(q.eulerAngles());
        EXPECT_NEAR(q[0],q1[0],0.01f);
        EXPECT_NEAR(q[1], q1[1], 0.01f);
        EXPECT_NEAR(q[2], q1[2], 0.01f);
        EXPECT_NEAR(q[3], q1[3], 0.01f);
    }

    TEST(QuaternionConversions, SIMD_EdgeCases) 
    {
        // ��������� ��������
        EXPECT_EQ(Quaternion::euler(0, 89.9999f, 0), Quaternion::euler(Quaternion::euler(0, 89.9999f, 0).eulerAngles()));

        // ������������� (gimbal lock)
        Quaternion q = Quaternion::euler(30, 90, 60);
        EXPECT_EQ(q, Quaternion::euler(q.eulerAngles()));
    }

    TEST(QuaternionConversions, SIMD_Precision) {
        // ��������� ����
        Quaternion q = Quaternion::euler(0.001f, 0.002f, 0.003f);
        EXPECT_EQ(q, Quaternion::euler(q.eulerAngles()));
    }

    TEST(QuaternionConversions, GimbalLockCase) {
        Quaternion q = Quaternion::euler(0, 90, 30);
        EXPECT_EQ(q, Quaternion::euler(q.eulerAngles()));
    }

    TEST(QuaternionConversions, NegativeAngles) {
        Quaternion q = Quaternion::euler(-45, -30, 180);
        EXPECT_EQ(q, Quaternion::euler(q.eulerAngles()));
    }

    TEST(QuaternionConversions, FullCircle)
    {
        Quaternion q = Quaternion::euler(360, 0, 360);
        EXPECT_EQ(q, Quaternion::euler(q.eulerAngles()));

    }
}