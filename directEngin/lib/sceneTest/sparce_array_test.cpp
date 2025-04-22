#include "pch.h"

#include "common/�ontainers/sparse_array.h"
#include <unordered_set>
#include <unordered_map>

namespace SparceArray
{
    TEST(SparseArrayTest, AddAndContains) 
    {
        SparseArray<int, unsigned, 100> arr;
        arr.add(42);
        EXPECT_TRUE(arr.contains(42));
        EXPECT_FALSE(arr.contains(99));
    }

    TEST(SparseArrayTest, RemoveElement) {
        SparseArray<std::string, unsigned, 50> arr;
        arr.add(5) = "test";
        EXPECT_TRUE(arr.remove(5));
        EXPECT_FALSE(arr.contains(5));
    }

    TEST(SparseArrayTest, SubscriptOperator)
    {
        SparseArray<float, unsigned, 10> arr;
        arr.add(3) = 3.14f;
        EXPECT_FLOAT_EQ(arr[3], 3.14f);
        arr[3] = 2.71f;
        EXPECT_FLOAT_EQ(arr[3], 2.71f);
    }

    TEST(SparseArrayTest, ConstAccess) 
    {
        SparseArray<double, unsigned, 5> arr;
        arr.add(2) = 1.618;
        const auto& constArr = arr;
        EXPECT_DOUBLE_EQ(constArr[2], 1.618);
    }

    TEST(SparseArrayTest, IteratorTraversal) 
    {
        SparseArray<char, unsigned, 26> arr;
        for (int i = 0; i < 5; ++i) arr.add(i) = 'a' + i;

        std::string result;
        for (auto it = arr.begin(); it != arr.end(); ++it) {
            result += *it;
        }
        EXPECT_EQ(result, "abcde");
    }

    TEST(SparseArrayTest, EntityIterators) 
    {
        SparseArray<int, unsigned, 100> arr;
        arr.add(10) = 100;
        arr.add(20) = 200;

        std::unordered_set<int> entities;
        for (auto it = arr.index_begin(); it != arr.index_end(); ++it) 
        {
            entities.insert(*it);
        }
        EXPECT_TRUE(entities.contains(10) && entities.contains(20));
    }

    TEST(SparseArrayTest, MaxCapacity) 
    {
        SparseArray<int, unsigned, 3> arr;
        arr.add(1); arr.add(2); arr.add(3);
        EXPECT_THROW(arr.add(4), std::out_of_range);
    }

    TEST(SparseArrayTest, DuplicateAdd) 
    {
        SparseArray<int, unsigned, 10> arr;
        arr.add(5) = 100;
        auto& ref = arr.add(5); // ������ ������� ������������
        EXPECT_EQ(ref, 100);
    }

    TEST(SparseArrayTest, EntityIteratorsUnordered) 
    {
        SparseArray<int, unsigned, 10> arr;
        std::unordered_set<int> entities = { 5, 15, 25 };

        for (int id : entities) {
            arr.add(id);
        }

        // ������� ���� �������
        arr.remove(15);
        entities.erase(15);

        // ��������� ����� ebegin/eend
        std::unordered_set<int> foundEntities;
        for (auto it = arr.index_begin(); it != arr.index_end(); ++it) {
            foundEntities.insert(*it);
        }

        EXPECT_EQ(foundEntities, entities);
    }

    TEST(SparseArrayTest, DataIntegrityAfterRemovals) 
    {
        SparseArray<std::string, unsigned, 10> arr;
        std::unordered_map<int, std::string> testData = {
            {1, "A"}, {2, "B"}, {3, "C"}, {4, "D"}
        };

        // ��������� ������
        for (auto& [id, val] : testData) {
            arr.add(id) = val;
        }

        // ������� ��� ��������
        arr.remove(2);
        arr.remove(3);
        testData.erase(2);
        testData.erase(3);

        // ��������� ����������
        for (auto& [id, expected] : testData) {
            EXPECT_EQ(arr[id], expected);
        }

        // ��������� ���������
        EXPECT_FALSE(arr.contains(2));
        EXPECT_FALSE(arr.contains(3));
    }

    TEST(SparseArrayTest, DataIteratorsCoverage) 
    {
        SparseArray<int, unsigned, 10> arr;
        std::unordered_set<int> values = { 10, 20, 30, 40 };

        // ��������� ��������
        for (int val : values) {
            arr.add(val) = val * 2;
        }

        // ������� ���� �������
        arr.remove(20);
        values.erase(20);

        // ��������� ����� ���������
        std::unordered_set<int> foundValues;
        for (auto& elem : arr) {
            foundValues.insert(elem / 2); // elem = key * 2
        }

        EXPECT_EQ(foundValues, values);
    }

    TEST(SparseArrayTest, DataSwapOnRemove) 
    {
        SparseArray<int, unsigned, 5> arr;
        arr.add(1) = 10;
        arr.add(2) = 20;
        arr.add(3) = 30;

        arr.remove(2);
        EXPECT_EQ(arr[3], 30);
        EXPECT_FALSE(arr.contains(2));
    }

    TEST(SparseArrayPerfTest, MillionInserts) 
    {
        SparseArray<int, unsigned, 1'000'000> arr;
        for (int i = 0; i < 1'000'000; ++i) {
            arr.add(i) = i * 2;
        }
        EXPECT_EQ(arr.size(), 1'000'000);
    }

    TEST(SparseArrayTest, InvalidAccess)
    {
        SparseArray<int, unsigned, 10> arr;
        EXPECT_THROW(arr[99], std::out_of_range);
    }

}