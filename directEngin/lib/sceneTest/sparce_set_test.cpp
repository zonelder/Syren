#include "pch.h"
#include "common/ñontainers/sparse_set.h"


namespace TSparseSet
{
    TEST(Basic, InitialState) 
    {
        constexpr unsigned N = 10;
        SparseSet<unsigned> set;
        EXPECT_EQ(set.size(), 0);
        for (unsigned i = 0; i < N; ++i)
        {
            EXPECT_FALSE(set.contains(i));
        }
    }

    TEST(Basic, AddSingleElement) 
    {
        constexpr unsigned N = 10;
        SparseSet<unsigned> set;
        set.add(5);
        EXPECT_EQ(set.size(), 1);
        EXPECT_TRUE(set.contains(5));

        auto it = set.begin();
        EXPECT_EQ(*it, 5);
        ++it;
        EXPECT_TRUE(it == set.end());
    }

    TEST(Basic, AddMultipleElements) 
    {
        constexpr unsigned N = 10;
        SparseSet<unsigned> set;
        set.add(5);
        set.add(3);
        set.add(7);

        EXPECT_EQ(set.size(), 3);
        std::vector<unsigned> elements;
        for (auto e : set) elements.push_back(e);

        ASSERT_EQ(elements.size(), 3);
        EXPECT_EQ(elements[0], 5);
        EXPECT_EQ(elements[1], 3);
        EXPECT_EQ(elements[2], 7);
    }

    TEST(Basic, AddDuplicate)
    {
        constexpr unsigned N = 10;
        SparseSet<unsigned> set;
        set.add(5);
        set.add(5);
        EXPECT_EQ(set.size(), 1);
    }

    TEST(Basic, RemoveExisting) 
    {
        constexpr unsigned N = 10;
        SparseSet<unsigned> set;
        set.add(5);
        set.add(3);
        set.add(7);

        bool removed = set.remove(3);
        EXPECT_TRUE(removed);
        EXPECT_EQ(set.size(), 2);

        std::vector<unsigned> elements;
        for (auto e : set) elements.push_back(e);

        ASSERT_EQ(elements.size(), 2);
        EXPECT_EQ(elements[0], 5);
        EXPECT_EQ(elements[1], 7);
    }

    TEST(Basic, RemoveNonExisting)
    {
        constexpr unsigned N = 10;
        SparseSet<unsigned> set;
        set.add(5);
        bool removed = set.remove(6);
        EXPECT_FALSE(removed);
        EXPECT_EQ(set.size(), 1);
    }

    TEST(Basic, Iteration)
    {
        constexpr unsigned N = 10;
        SparseSet<unsigned> set;
        set.add(5);
        set.add(3);
        set.add(7);

        std::vector<unsigned> expected = { 5, 3, 7 };
        std::vector<unsigned> actual;
        for (auto e : set) actual.push_back(e);

        EXPECT_EQ(actual, expected);
    }

    TEST(Basic, ConstIteration) 
    {
        constexpr unsigned N = 10;
        SparseSet<unsigned> set;
        set.add(5);
        set.add(3);

        const auto& constSet = set;
        std::vector<unsigned> expected = { 5, 3 };
        std::vector<unsigned> actual;
        for (auto e : constSet) actual.push_back(e);

        EXPECT_EQ(actual, expected);
    }

    TEST(Basic, SizeChanges)
    {
        constexpr unsigned N = 10;
        SparseSet<unsigned> set;

        EXPECT_EQ(set.size(), 0);
        set.add(1);
        EXPECT_EQ(set.size(), 1);
        set.add(2);
        EXPECT_EQ(set.size(), 2);
        set.remove(1);
        EXPECT_EQ(set.size(), 1);
        set.remove(2);
        EXPECT_EQ(set.size(), 0);
    }

    TEST(Basic, RemoveFromEmpty)
    {
        constexpr unsigned N = 10;
        SparseSet<unsigned> set;
        EXPECT_FALSE(set.remove(0));
    }

    TEST(Basic, AddAfterRemove)
    {
        constexpr unsigned N = 10;
        SparseSet<unsigned> set;

        set.add(1);
        set.remove(1);
        set.add(2);

        EXPECT_EQ(set.size(), 1);
        EXPECT_TRUE(set.contains(2));
        EXPECT_FALSE(set.contains(1));
    }

    TEST(Basic, RemoveSwapsWithLast)
    {

        constexpr unsigned N = 10;
        constexpr auto v = sizeof(unsigned);
        SparseSet<unsigned> set;

        set.add(1);
        set.add(2);
        set.add(3);
        set.remove(2);

        std::vector<unsigned> elements;
        for (auto e : set) elements.push_back(e);

        ASSERT_EQ(elements.size(), 2);
        EXPECT_EQ(elements[0], 1);
        EXPECT_EQ(elements[1], 3);
    }

    TEST(Basic, EmptyIteration)
    {
        constexpr unsigned N = 10;
        SparseSet<unsigned> set;

        std::vector<unsigned> elements;
        for (auto e : set) elements.push_back(e);

        EXPECT_TRUE(elements.empty());
    }

    TEST(Basic, TombstoneHandling)
    {
        constexpr unsigned N = 10;
        constexpr auto tombstone = std::numeric_limits<unsigned>::max();
        SparseSet<unsigned> set;

        set.add(tombstone);
        EXPECT_EQ(set.size(), 1);
        EXPECT_TRUE(set.contains(tombstone));

        std::vector<unsigned> elements;
        for (auto e : set) elements.push_back(e);

        EXPECT_TRUE(elements.empty());
    }

}