#include "pch.h"
#include "common/сontainers/sparse_page.h"
#include <random>

struct Position
{
	float x, y, z;
};

namespace TSparsePage
{

TEST(SparsePageTest, AddAndContains) {
    SparsePage<int, uint32_t> sp;
    const uint32_t id = 123;
    EXPECT_FALSE(sp.contains(id));
    sp.add(id);
    EXPECT_TRUE(sp.contains(id));
}

TEST(SparsePageTest, PageExist)
{
    SparsePage<int, uint32_t> sp;
    const uint32_t id = 456;
    sp.add(id);
    EXPECT_TRUE(sp.page_exist(id));
    EXPECT_TRUE(sp.contains(id));
    EXPECT_TRUE(sp.remove(id));
}

TEST(SparsePageTest, RemoveEntity) {
    SparsePage<int, uint32_t> sp;
    const uint32_t id = 456;
    sp.add(id);
    EXPECT_TRUE(sp.remove(id));
    EXPECT_FALSE(sp.contains(id));
    EXPECT_FALSE(sp.remove(id)); // Remove non-existent
}

TEST(SparsePageTest, MultiplePages) {
    SparsePage<int, uint32_t> sp;
    const uint32_t id1 = 0;     // Page 0
    const uint32_t id2 = 4096;  // Page 1
    sp.add(id1);
    sp.add(id2);
    EXPECT_TRUE(sp.contains(id1));
    EXPECT_TRUE(sp.contains(id2));
    EXPECT_TRUE(sp.remove(id1));
    EXPECT_FALSE(sp.contains(id1));
    EXPECT_TRUE(sp.contains(id2));
}

TEST(SparsePageTest, DataIntegrity) {
    SparsePage<int, uint32_t> sp;
    const uint32_t id1 = 5;
    const uint32_t id2 = 4096;
    sp.add(id1) = 42;
    sp.add(id2) = 100;
    EXPECT_EQ(sp.add(id1), 42);
    EXPECT_EQ(sp.add(id2), 100);
}

TEST(SparsePageTest, RemoveNonExistent) {
    SparsePage<int, uint32_t> sp;
    EXPECT_FALSE(sp.remove(999));
}

TEST(SparsePageTest, AddDuplicate) {
    SparsePage<int, uint32_t> sp;
    const uint32_t id = 10;
    int& data1 = sp.add(id);
    data1 = 55;
    int& data2 = sp.add(id);
    EXPECT_EQ(&data1, &data2);
    EXPECT_EQ(data1, 55);
}

TEST(SparsePageTest, EntityZero) {
    SparsePage<int, uint32_t> sp;
    const uint32_t id = 0;
    EXPECT_FALSE(sp.contains(id));
    sp.add(id);
    EXPECT_TRUE(sp.contains(id));
    EXPECT_TRUE(sp.remove(id));
    EXPECT_FALSE(sp.contains(id));
}

TEST(SparsePageTest, RemoveSwapsData) {
    SparsePage<int, uint32_t> sp;
    const uint32_t id1 = 1;
    const uint32_t id2 = 2;
    sp.add(id1) = 100;
    sp.add(id2) = 200;
    sp.remove(id1);
    EXPECT_TRUE(sp.contains(id2));
    EXPECT_EQ(sp.add(id2), 200);
    EXPECT_FALSE(sp.contains(id1));
}

TEST(SparsePageTest, FullPage) {
    SparsePage<int, uint32_t> sp;
    constexpr uint32_t PAGE_SIZE = SparsePage<int, uint32_t>::PAGE_SIZE;
    const uint32_t base_id = 0;
    // Fill a page
    for (uint32_t i = 0; i < PAGE_SIZE; ++i) {
        sp.add(base_id + i) = static_cast<int>(i);
    }
    // Verify all entries
    for (uint32_t i = 0; i < PAGE_SIZE; ++i) {
        EXPECT_TRUE(sp.contains(base_id + i));
        EXPECT_EQ(sp.add(base_id + i), static_cast<int>(i));
    }
    // Remove an entry and check
    const uint32_t removed_id = 100;
    sp.remove(removed_id);
    EXPECT_FALSE(sp.contains(removed_id));
}

TEST(SparsePageTest, baseIterate)
{
    SparsePage<int, uint32_t> sp;
    constexpr uint32_t PAGE_SIZE = 2*SparsePage<int, uint32_t>::PAGE_SIZE;
    for (uint32_t i = 0; i < PAGE_SIZE; ++i) 
    {
          sp.add(i) = static_cast<int>(i);
    }
    int i = 0;
    for (auto v : sp)
    {
        EXPECT_EQ(v, i);
        i++;
    }

}


TEST(SparsePageTest, EmptyIteration) 
{
    SparsePage<int, uint32_t> sp;
    EXPECT_EQ(sp.begin(), sp.end()); // Begin == end для пустой коллекции

    size_t count = 0;
    for (auto&& _ : sp) { ++count; } // Не должен заходить в цикл
    EXPECT_EQ(count, 0u);
}

TEST(SparsePageTest, SparseIteration) 
{
    SparsePage<int, uint32_t> sp;

    // Создаем разреженные данные с пропуском страниц
    sp.add(1) = 10;
    sp.add(4096 * 2) = 20; // Page 2
    sp.add(4096 * 5) = 30; // Page 5

    std::vector<int> values;
    for (auto&& v : sp) {
        values.push_back(v);
    }

    // Проверяем порядок и количество элементов
    ASSERT_EQ(values.size(), 3u);
    EXPECT_EQ(values, (std::vector<int>{10, 20, 30}));
}

TEST(SparsePageTest, IteratorAfterRemoval) 
{
    SparsePage<int, uint32_t> sp;

    // Добавляем и удаляем элементы
    sp.add(1) = 10;
    sp.add(2) = 20;
    sp.add(3) = 30;
    sp.remove(2);

    std::vector<int> values;
    for (auto&& v : sp) {
        values.push_back(v);
    }

    ASSERT_EQ(values.size(), 2u);
    EXPECT_TRUE(std::find(values.begin(), values.end(), 10) != values.end());
    EXPECT_TRUE(std::find(values.begin(), values.end(), 30) != values.end());
}

TEST(SparsePageTest, ConstIterator) 
{
    SparsePage<int, uint32_t> sp;
    sp.add(1) = 42;
    sp.add(2) = 24;

    const auto& csp = sp;
    size_t count = 0;
    for (auto it = csp.begin(); it != csp.end(); ++it) {
        EXPECT_TRUE(*it == 42 || *it == 24);
        ++count;
    }
    EXPECT_EQ(count, 2u);
}


TEST(SparsePageTest, ReversePagePopulation) 
{
    SparsePage<int, uint32_t> sp;
    constexpr uint32_t PAGE_SIZE = SparsePage<int, uint32_t>::PAGE_SIZE;

    // inverse order
    sp.add(PAGE_SIZE * 3) = 30; // Page 3
    sp.add(PAGE_SIZE * 1) = 10; // Page 1
    sp.add(PAGE_SIZE * 2) = 20; // Page 2

    std::vector<int> values;
    for (auto&& v : sp) {
        values.push_back(v);
    }

    // Проверяем порядок обхода страниц
    EXPECT_EQ(values, (std::vector<int>{10, 20, 30}));
}

TEST(SparsePageTest, PartialPageIteration) 
{
    SparsePage<int, uint32_t> sp;

    sp.add(5) = 50;
    sp.add(10) = 100;
    sp.add(15) = 150;
    sp.remove(10);

    std::vector<int> values;
    for (auto&& v : sp) {
        values.push_back(v);
    }

    ASSERT_EQ(values.size(), 2u);
    EXPECT_TRUE(std::find(values.begin(), values.end(), 50) != values.end());
    EXPECT_TRUE(std::find(values.begin(), values.end(), 150) != values.end());
}

TEST(SparsePageTest, IteratorWithEmptyPages) {
    SparsePage<int, uint32_t> sp;

    // Page 0: empty
    // Page 1: there is data
    sp.add(4096 + 5) = 100;
    // Page 2: empty
    // Page 3: there is data
    sp.add(4096 * 3 + 7) = 200;

    std::vector<int> values;
    for (auto&& v : sp) {
        values.push_back(v);
    }

    ASSERT_EQ(values.size(), 2u);
    EXPECT_EQ(values[0], 100);
    EXPECT_EQ(values[1], 200);
}

TEST(SparsePageTest, StdAlgorithmCompatibility) 
{
    SparsePage<int, uint32_t> sp;

    sp.add(1) = 10;
    sp.add(2) = 20;
    sp.add(3) = 30;

    // Проверяем работу с std алгоритмами
    auto it = std::find(sp.begin(), sp.end(), 20);
    ASSERT_NE(it, sp.end());
    EXPECT_EQ(*it, 20);

    size_t count = std::count(sp.begin(), sp.end(), 30);
    EXPECT_EQ(count, 1u);
}

TEST(SparsePageTest, IteratorInvalidation)
{
    SparsePage<int, uint32_t> sp;
    sp.add(1) = 10;
    sp.add(4096) = 20;

    auto it = sp.begin();
    sp.remove(1); // remove element from first page
    ++it; // iterator should continue from next paeg.
    EXPECT_EQ(*it, 20);
}

TEST(SparsePageTest, ReuseRemovedEntity) {
    SparsePage<std::string, uint32_t> sp;
    const uint32_t id = 100;

    sp.add(id) = "first";
    sp.remove(id);
    sp.add(id) = "second";

    EXPECT_EQ(sp.get(id), "second");
    EXPECT_EQ(sp.begin()->size(), 6); // "second"
}

TEST(SparsePageTest, StressTest) 
{
    SparsePage<int, uint32_t> sp;
    const uint32_t num_operations = 1'000'000;
    std::mt19937 rng;
    std::uniform_int_distribution<uint32_t> dist(0, 100'000);

    for (uint32_t i = 0; i < num_operations; ++i) {
        auto id = dist(rng);
        if (sp.contains(id)) {
            sp.remove(id);
        }
        else 
        {
            sp.add(id) = id;
        }
    }
}

using Entity = uint32_t;
using Data = int;

TEST(SparsePageIteratorTest, IteratorTraversal) 
{
    SparsePage<Data, Entity> page;
    page.add(1) = 10;
    page.add(4097) = 20;
    page.add(8193) = 30;

    std::vector<int> expected = { 10, 20, 30 };
    std::vector<int> result;

    for (auto it = page.begin(); it != page.end(); ++it) {
        result.push_back(*it);
    }

    EXPECT_EQ(result, expected);
}

TEST(SparsePageIteratorTest, ConstIteratorTraversal) 
{
    SparsePage<Data, Entity> page;
    page.add(1) = 10;
    page.add(4097) = 20;
    page.add(8193) = 30;

    const auto& const_page = page;
    std::vector<int> expected = { 10, 20, 30 };
    std::vector<int> result;

    for (auto it = const_page.begin(); it != const_page.end(); ++it) {
        result.push_back(*it);
    }

    EXPECT_EQ(result, expected);
}

TEST(SparsePageIteratorTest, IndexIteratorTraversal) 
{
    SparsePage<Data, Entity> page;
    page.add(1) = 10;
    page.add(4097) = 20;
    page.add(8193) = 30;

    std::vector<Entity> expected = { 1, 4097, 8193 }; // Локальные идентификаторы
    std::vector<Entity> result;

    for (auto it = page.index_begin(); it != page.index_end(); ++it) {
        result.push_back(*it);
    }
    EXPECT_EQ(result, expected);
}

TEST(SparsePageIteratorTest, ConstIndexIteratorTraversal) 
{
    SparsePage<Data, Entity> page;
    page.add(1) = 10;
    page.add(4097) = 20;
    page.add(8193) = 30;

    const auto& const_page = page;
    std::vector<Entity> expected = { 1, 4097, 8193 }; // Локальные идентификаторы
    std::vector<Entity> result;

    for (auto it = const_page.index_begin(); it != const_page.index_end(); ++it) {
        result.push_back(*it);
    }

    EXPECT_EQ(result, expected);
}

TEST(SparsePageIteratorTest, PairIteratorTraversal) 
{
    SparsePage<Data, Entity> page;
    page.add(1) = 10;
    page.add(4097) = 20;
    page.add(8193) = 30;

    std::vector<std::pair<Entity, int>> expected = { {1, 10}, {4097, 20}, {8193, 30} };
    std::vector<std::pair<Entity, int>> result;

    for (auto it = page.pair_begin(); it != page.pair_end(); ++it) {
        result.push_back(*it);
    }

    EXPECT_EQ(result, expected);
}

TEST(SparsePageIteratorTest, ConstPairIteratorTraversal) 
{
    SparsePage<Data, Entity> page;
    page.add(1) = 10;
    page.add(4097) = 20;
    page.add(8193) = 30;

    const auto& const_page = page;
    std::vector<std::pair<Entity, int>> expected = { {1, 10}, {4097, 20}, {8193, 30} };
    std::vector<std::pair<Entity, int>> result;

    for (auto it = const_page.pair_begin(); it != const_page.pair_end(); ++it) {
        result.push_back(*it);
    }

    EXPECT_EQ(result, expected);
}

}
