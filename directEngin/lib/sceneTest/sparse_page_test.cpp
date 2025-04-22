#include "pch.h"
#include "common/ñontainers/sparse_page.h"


struct Position
{
	float x, y, z;
};

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
