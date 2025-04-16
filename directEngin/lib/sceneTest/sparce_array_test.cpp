#include "pch.h"

#include "common/сontainers/sparse_array.h"
#include <unordered_set>
#include <unordered_map>

namespace SparceArray
{
    TEST(SparseArrayTest, AddAndContains) 
    {
        SparseArray<int, int, 100> arr;
        arr.add(42);
        EXPECT_TRUE(arr.contains(42));
        EXPECT_FALSE(arr.contains(99));
    }

    TEST(SparseArrayTest, RemoveElement) {
        SparseArray<std::string, int, 50> arr;
        arr.add(5) = "test";
        EXPECT_TRUE(arr.remove(5));
        EXPECT_FALSE(arr.contains(5));
    }

    TEST(SparseArrayTest, SubscriptOperator)
    {
        SparseArray<float, int, 10> arr;
        arr.add(3) = 3.14f;
        EXPECT_FLOAT_EQ(arr[3], 3.14f);
        arr[3] = 2.71f;
        EXPECT_FLOAT_EQ(arr[3], 2.71f);
    }

    TEST(SparseArrayTest, ConstAccess) 
    {
        SparseArray<double, int, 5> arr;
        arr.add(2) = 1.618;
        const auto& constArr = arr;
        EXPECT_DOUBLE_EQ(constArr[2], 1.618);
    }

    TEST(SparseArrayTest, IteratorTraversal) 
    {
        SparseArray<char, int, 26> arr;
        for (int i = 0; i < 5; ++i) arr.add(i) = 'a' + i;

        std::string result;
        for (auto it = arr.begin(); it != arr.end(); ++it) {
            result += *it;
        }
        EXPECT_EQ(result, "abcde");
    }

    TEST(SparseArrayTest, EntityIterators) 
    {
        SparseArray<int, int, 100> arr;
        arr.add(10) = 100;
        arr.add(20) = 200;

        std::unordered_set<int> entities;
        for (auto it = arr.ebegin(); it != arr.eend(); ++it) {
            entities.insert(*it);
        }
        EXPECT_TRUE(entities.contains(10) && entities.contains(20));
    }

    TEST(SparseArrayTest, MaxCapacity) 
    {
        SparseArray<int, int, 3> arr;
        arr.add(1); arr.add(2); arr.add(3);
        EXPECT_THROW(arr.add(4), std::out_of_range);
    }

    TEST(SparseArrayTest, DuplicateAdd) 
    {
        SparseArray<int, int, 10> arr;
        arr.add(5) = 100;
        auto& ref = arr.add(5); // Должно вернуть существующий
        EXPECT_EQ(ref, 100);
    }

    TEST(SparseArrayTest, EntityIteratorsUnordered) 
    {
        SparseArray<int, int, 10> arr;
        std::unordered_set<int> entities = { 5, 15, 25 };

        for (int id : entities) {
            arr.add(id);
        }

        // Удаляем один элемент
        arr.remove(15);
        entities.erase(15);

        // Проверяем через ebegin/eend
        std::unordered_set<int> foundEntities;
        for (auto it = arr.ebegin(); it != arr.eend(); ++it) {
            foundEntities.insert(*it);
        }

        EXPECT_EQ(foundEntities, entities);
    }

    TEST(SparseArrayTest, DataIntegrityAfterRemovals) 
    {
        SparseArray<std::string, int, 10> arr;
        std::unordered_map<int, std::string> testData = {
            {1, "A"}, {2, "B"}, {3, "C"}, {4, "D"}
        };

        // Добавляем данные
        for (auto& [id, val] : testData) {
            arr.add(id) = val;
        }

        // Удаляем два элемента
        arr.remove(2);
        arr.remove(3);
        testData.erase(2);
        testData.erase(3);

        // Проверяем оставшиеся
        for (auto& [id, expected] : testData) {
            EXPECT_EQ(arr[id], expected);
        }

        // Проверяем удаленные
        EXPECT_FALSE(arr.contains(2));
        EXPECT_FALSE(arr.contains(3));
    }

    TEST(SparseArrayTest, DataIteratorsCoverage) 
    {
        SparseArray<int, int, 10> arr;
        std::unordered_set<int> values = { 10, 20, 30, 40 };

        // Добавляем элементы
        for (int val : values) {
            arr.add(val) = val * 2;
        }

        // Удаляем один элемент
        arr.remove(20);
        values.erase(20);

        // Проверяем через итераторы
        std::unordered_set<int> foundValues;
        for (auto& elem : arr) {
            foundValues.insert(elem / 2); // elem = key * 2
        }

        EXPECT_EQ(foundValues, values);
    }

    TEST(SparseArrayTest, DataSwapOnRemove) 
    {
        SparseArray<int, int, 5> arr;
        arr.add(1) = 10;
        arr.add(2) = 20;
        arr.add(3) = 30;

        arr.remove(2);
        EXPECT_EQ(arr[3], 30);
        EXPECT_FALSE(arr.contains(2));
    }

    TEST(SparseArrayPerfTest, MillionInserts) 
    {
        SparseArray<int, int, 1'000'000> arr;
        for (int i = 0; i < 1'000'000; ++i) {
            arr.add(i) = i * 2;
        }
        EXPECT_EQ(arr.size(), 1'000'000);
    }

    TEST(SparseArrayTest, InvalidAccess)
    {
        SparseArray<int, int, 10> arr;
        EXPECT_THROW(arr[99], std::out_of_range);
    }

    TEST(SparseArrayTest, MoveSemantics) 
    {
        SparseArray<std::unique_ptr<int>, int, 5> arr;
        arr.add(1) = std::make_unique<int>(42);

        auto movedArr = std::move(arr);
        EXPECT_EQ(*movedArr[1], 42);
        EXPECT_TRUE(arr.size() == 0);
    }

}