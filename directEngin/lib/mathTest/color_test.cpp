#include "pch.h"
#include "color.h"


#include "Color.h"

using namespace DirectX;

namespace TColor
{

TEST(Color, DefaultConstructor) {
    Color c;
    XMVECTOR v = c;
    EXPECT_FLOAT_EQ(XMVectorGetX(v), 0.0f);
    EXPECT_FLOAT_EQ(XMVectorGetY(v), 0.0f);
    EXPECT_FLOAT_EQ(XMVectorGetZ(v), 0.0f);
    EXPECT_FLOAT_EQ(XMVectorGetW(v), 0.0f);
}

TEST(Color, ComponentConstructor) {
    Color c(0.5f, 0.6f, 0.7f, 0.8f);
    EXPECT_FLOAT_EQ(c[0], 0.5f);
    EXPECT_FLOAT_EQ(c[1], 0.6f);
    EXPECT_FLOAT_EQ(c[2], 0.7f);
    EXPECT_FLOAT_EQ(c[3], 0.8f);
}

TEST(Color, StaticConstants) {
    EXPECT_EQ(Color::black, Color(0, 0, 0, 1));
    EXPECT_EQ(Color::white, Color(1, 1, 1, 1));
    EXPECT_EQ(Color::red, Color(1, 0, 0, 1));
    EXPECT_EQ(Color::green, Color(0, 1, 0, 1));
    EXPECT_EQ(Color::blue, Color(0, 0, 1, 1));
    EXPECT_EQ(Color::yellow, Color(1, 1, 0, 1));
    EXPECT_EQ(Color::cyan, Color(0, 1, 1, 1));
    EXPECT_EQ(Color::magenta, Color(1, 0, 1, 1));
    EXPECT_EQ(Color::gray, Color(0.5f, 0.5f, 0.5f, 1));
}

TEST(Color, ComparisonOperators) {
    Color c1(0.1f, 0.2f, 0.3f);
    Color c2(0.1001f, 0.2001f, 0.3001f);
    Color c3(0.11f, 0.2f, 0.3f);

    EXPECT_TRUE(c1 == c2);
    EXPECT_FALSE(c1 == c3);
    EXPECT_TRUE(c1 != c3);
}

TEST(Color, ArithmeticOperations) {
    Color c1(0.2f, 0.4f, 0.6f);
    Color c2(0.1f, 0.3f, 0.5f);

    // Addition
    Color sum = c1 + c2;
    EXPECT_FLOAT_EQ(sum[0], 0.3f);
    EXPECT_FLOAT_EQ(sum[1], 0.7f);
    EXPECT_FLOAT_EQ(sum[2], 1.1f);

    // Subtraction
    Color diff = c1 - c2;
    EXPECT_NEAR(diff[0], 0.1f, 1e-6);
    EXPECT_NEAR(diff[1], 0.1f, 1e-6);
    EXPECT_NEAR(diff[2], 0.1f, 1e-6);

    // Multiplication
    Color prod = c1 * c2;
    EXPECT_FLOAT_EQ(prod[0], 0.02f);
    EXPECT_FLOAT_EQ(prod[1], 0.12f);
    EXPECT_FLOAT_EQ(prod[2], 0.30f);
}

TEST(Color, ScalarOperations) {
    Color c(0.4f, 0.6f, 0.8f);

    // Scalar multiplication
    Color scaled = c * 0.5f;
    EXPECT_FLOAT_EQ(scaled[0], 0.2f);
    EXPECT_FLOAT_EQ(scaled[1], 0.3f);
    EXPECT_FLOAT_EQ(scaled[2], 0.4f);

    // Scalar division
    Color divided = c / 2.0f;
    EXPECT_FLOAT_EQ(divided[0], 0.2f);
    EXPECT_FLOAT_EQ(divided[1], 0.3f);
    EXPECT_FLOAT_EQ(divided[2], 0.4f);
}

TEST(Color, Lerp) {
    Color black = Color::black;
    Color white = Color::white;

    Color result = Color::lerp(black, white, 0.5f);
    EXPECT_EQ(result, Color::gray);

    Color unclamped = Color::lerpUnclamped(black, white, 1.5f);
    EXPECT_GT(unclamped[0], 1.0f);
    EXPECT_GT(unclamped[1], 1.0f);
    EXPECT_GT(unclamped[2], 1.0f);
}

TEST(Color, HSVConversion) {
    // Red
    Color red = Color::HSVToRGB(0.0f, 1.0f, 1.0f);
    EXPECT_EQ(red, Color::red);

    // Green
    Color green = Color::HSVToRGB(120.0f / 360.0f, 1.0f, 1.0f);
    EXPECT_EQ(green, Color::green);

    // Round trip test
    Color original(0.75f, 0.35f, 0.85f);
    float H, S, V;
    Color::RGBToHSV(original, H, S, V);
    Color converted = Color::HSVToRGB(H, S, V);
    EXPECT_TRUE(original == converted);
}

TEST(Color, GammaLinear) {
    Color c(0.5f, 0.5f, 0.5f);
    Color gamma = c.gamma();
    Color linear = gamma.linear();

    EXPECT_NEAR(c[0], linear[0], 1e-4);
    EXPECT_NEAR(c[1], linear[1], 1e-4);
    EXPECT_NEAR(c[2], linear[2], 1e-4);
}

TEST(Color, Grayscale) {
    Color c(0.3f, 0.5f, 0.2f);
    float gray = c.grayscale();
    float expected = 0.3f * 0.299f + 0.5f * 0.587f + 0.2f * 0.114f;
    EXPECT_NEAR(gray, expected, 1e-6);
}

TEST(Color, ComponentFunctions)
{
    Color c(0.1f, 0.7f, 0.3f, 0.9f);
    EXPECT_FLOAT_EQ(c.maxComponent(), 0.9f);
    EXPECT_FLOAT_EQ(c.minComponent(), 0.1f);
}


}
