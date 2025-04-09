#include "pch.h"
#include "matrix4x4.h"

using namespace DirectX;
namespace TMatrix
{
    const float EPSILON = 1e-4f;

    TEST(Matrix4x4, Constructors) 
    {
        Matrix4x4 m;
        EXPECT_TRUE(m.isIdentity());

        XMMATRIX dxMat = XMMatrixTranslation(1, 2, 3);
        Matrix4x4 m2(dxMat);
        EXPECT_FALSE(m2.isIdentity());
    }

    TEST(Matrix4x4, Identity) 
    {
        Matrix4x4 m;
        EXPECT_EQ(m, Matrix4x4::identity);

        EXPECT_TRUE(m.isIdentity());
        m.setRow(0, Vector4(2, 0, 0, 0));
        EXPECT_FALSE(m.isIdentity());
    }

    TEST(Matrix4x4, BasicOperations) 
    {
        Matrix4x4 m1 = Matrix4x4::translate(Vector3(1, 2, 3));
        Matrix4x4 m2 = Matrix4x4::rotate(Quaternion::identity);
        Matrix4x4 m3 = m1 * m2;

        EXPECT_EQ(m3, m1);
    }

    TEST(Matrix4x4, Translation) 
    {
        Vector3 trans(5, -3, 2.5f);
        Matrix4x4 m = Matrix4x4::translate(trans);

        EXPECT_EQ(m.translation(), trans);

        Vector3 point(1, 2, 3);
        Vector3 transformed = m.multiplyPoint(point);
        EXPECT_EQ(transformed, point + trans);
    }

    TEST(Matrix4x4, Scale) 
    {
        Vector3 scale(2, 3, 4);
        Matrix4x4 m = Matrix4x4::scale(scale);

        EXPECT_EQ(m.scale(), scale);

        Vector3 vec(1, 1, 1);
        Vector3 transformed = m.multiplyVector(vec);
        EXPECT_EQ(transformed, scale);
    }

    TEST(Matrix4x4, Rotation) 
    {
        Quaternion rot = Quaternion::angleAxis(90.0f, Vector3::up);
        Matrix4x4 m = Matrix4x4::rotate(rot);

        Vector3 point(1, 0, 0);
        Vector3 transformed = m.multiplyVector(point);
        EXPECT_EQ(transformed, Vector3(0, 0, -1));
    }

    TEST(Matrix4x4, TRS) 
    {
        Vector3 t(1, 2, 3);
        Vector3 s(2, 3, 4);
        Quaternion q = Quaternion::euler(45.0f, 0, 0);

        Matrix4x4 m = Matrix4x4::TRS(t, q, s);

        EXPECT_EQ(m.translation(), t);
        EXPECT_EQ(m.scale(), s);

        Matrix4x4 m2;
        m2.setTRS(t, q, s);
        EXPECT_EQ(m, m2);
    }

    TEST(Matrix4x4, Inverse) 
    {
        Matrix4x4 m = Matrix4x4::translate(Vector3(5, 3, 2)) *
            Matrix4x4::rotate(Quaternion::euler(50.0f, 90.0f, 175.0f)) *
            Matrix4x4::scale(Vector3(2, 3, 4));

        Matrix4x4 inv = m.inverse();
        Matrix4x4 identity = m * inv;

        EXPECT_TRUE(identity.isIdentity());
    }

    TEST(Matrix4x4, RowColumnOperations) 
    {
        Matrix4x4 m;
        m.setRow(0, Vector4(1, 2, 3, 4));
        EXPECT_EQ(m.getRow(0), Vector4(1, 2, 3, 4));

        m.setColumn(1, Vector4(5, 6, 7, 8));
        EXPECT_EQ(m.getColumn(1), Vector4(5, 6, 7, 8));
    }

    TEST(Matrix4x4, MultiplyPointVector) 
    {
        Matrix4x4 m = Matrix4x4::translate(Vector3(2, 3, 4));

        Vector3 point(1, 1, 1);
        Vector3 resPoint = m.multiplyPoint(point);
        EXPECT_EQ(resPoint, Vector3(3, 4, 5));

        Vector3 vec(1, 1, 1);
        Vector3 resVec = m.multiplyVector(vec);
        EXPECT_EQ(resVec, vec);
    }

    TEST(Matrix4x4, ProjectionMatrices) 
    {
        Matrix4x4 persp = Matrix4x4::perspective(45.0f, 1.0f, 0.1f, 100.0f);
        Vector4 point(0, 0, -5, 1);
        Vector4 proj = persp * point;
        proj /= proj[3];
        EXPECT_NEAR(proj[2], 0.95f, 0.01f);

        Matrix4x4 ortho = Matrix4x4::ortho(-10, 10, -10, 10, 0.1f, 100.0f);
        Vector4 orthoPoint(5, 5, -50, 1);
        Vector4 orthoProj = ortho * orthoPoint;
        EXPECT_NEAR(orthoProj[0], 0.5f, EPSILON);
    }

    TEST(Matrix4x4, LookAt) 
    {
        Vector3 eye(0, 0, 5);
        Vector3 target(0, 0, 0);
        Vector3 up(0, 1, 0);

        Matrix4x4 view = Matrix4x4::lookAt(eye, target, up);
        Vector3 point(0, 0, 0);
        Vector3 viewSpace = view.multiplyPoint(point);
        EXPECT_EQ(viewSpace, Vector3(0, 0, -5));
    }

    TEST(Matrix4x4, Determinant) 
    {
        Matrix4x4 m = Matrix4x4::scale(Vector3(2, 3, 4));
        EXPECT_NEAR(m.determinant(), 24.0f, EPSILON);
    }

    TEST(Matrix4x4, Inverse3DAffine) 
    {
        Matrix4x4 m = Matrix4x4::TRS(
            Vector3(5, 3, 2),
            Quaternion::euler(0.5f, 0.2f, 0.3f),
            Vector3(2, 3, 4)
        );

        Matrix4x4 inv;
        bool result = Matrix4x4::inverse3DAffine(m, inv);

        EXPECT_TRUE(result);
        Matrix4x4 identity = m * inv;
        EXPECT_EQ(identity,Matrix4x4::identity);
    }

    TEST(Matrix4x4, ValidityChecks) 
    {
        Matrix4x4 validTRS = Matrix4x4::TRS(
            Vector3(1, 2, 3),
            Quaternion::identity,
            Vector3(1, 1, 1)
        );
        EXPECT_TRUE(validTRS.isValidTRS());

        Matrix4x4 invalidTRS = validTRS;
        invalidTRS.setRow(3, Vector4(0, 0, 0, 2));
        EXPECT_FALSE(invalidTRS.isValidTRS());
    }

    TEST(Matrix4x4, ZeroMatrix) 
    {
        const Matrix4x4 zero = Matrix4x4::zero;

        for (int row = 0; row < 4; ++row) {
            Vector4 rowVec = zero.getRow(row);
            EXPECT_EQ(rowVec, Vector4::zero);
        }

        EXPECT_EQ(zero.scale(), Vector3(0, 0, 0));
    }
}