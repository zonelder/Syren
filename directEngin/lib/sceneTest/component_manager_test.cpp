#include "pch.h"
#include "component_manager.h"

#include <numeric>

class ComponentManagerTest : public ::testing::Test
{
protected:
	static ComponentManager componentManager;
};


ComponentManager ComponentManagerTest::componentManager;


struct TestComponent1 { int x; };

TEST_F(ComponentManagerTest, AddPoolCreatesNewePool)
{
	auto* pPool = componentManager.addPool<TestComponent1>();

	ASSERT_NE(pPool, nullptr);

	auto* pFetchPool = componentManager.getPool<TestComponent1>();
	ASSERT_EQ(pPool, pFetchPool);
}

TEST_F(ComponentManagerTest, GetPoolCreatesPoolIfMissing)
{
	auto* pPool = componentManager.getPool<TestComponent1>();
	ASSERT_NE(pPool, nullptr);
	ASSERT_EQ(pPool, componentManager.getPool<TestComponent1>());
}
TEST_F(ComponentManagerTest, AddComponentInsertsIntoPool) 
{
	EntityID id = 1;
	auto& component = componentManager.addComponent<TestComponent1>(id);
	component.x = 42;
	auto* pool = componentManager.getPool<TestComponent1>();
	ASSERT_EQ(pool->getComponent(id).x, 42);
	ASSERT_EQ(componentManager.getComponent<TestComponent1>(id).x, 42);
}
TEST_F(ComponentManagerTest, RemoveComponentWorks) 
{
	EntityID id = 2;
	componentManager.addComponent<TestComponent1>(id);
	ASSERT_TRUE(componentManager.removeComponent<TestComponent1>(id));
	ASSERT_FALSE(componentManager.removeComponent<TestComponent1>(id));
}


TEST_F(ComponentManagerTest, AddPoolAssertOnDuplicate) 
{
	ASSERT_DEATH({
		componentManager.addPool<TestComponent1>();
		componentManager.addPool<TestComponent1>();
		}, ".*");
}

TEST_F(ComponentManagerTest, DestructorDeletesAllPools) 
{
	auto* manager = new ComponentManager();
	struct TempComponent1 {};
	struct TempComponent2 {};
	manager->addPool<TempComponent1>();
	manager->addPool<TempComponent2>();
	ASSERT_NO_FATAL_FAILURE(delete manager);
}

TEST_F(ComponentManagerTest, AddRemoveEquavalence)
{
	for (EntityID e = 0; e < MAX_ENTITY - 1; e++)
	{
		auto& test = componentManager.addComponent<TestComponent1>(e);
		test.x = (int)e;
	}

	for (EntityID e = 0; e < MAX_ENTITY - 1; e += 2)
	{
		componentManager.removeComponent<TestComponent1>(e);
	}

	auto pool = componentManager.getPool<TestComponent1>();


	for (auto it = pool->ebegin();it != pool->eend();++it)
	{
		auto e = *it;
		EXPECT_FALSE(e % 2 == 0);//all even entities was erased

		auto& comp = pool->getComponent(*it);
		EXPECT_EQ(comp.x, (int)e);
	}

}

TEST_F(ComponentManagerTest, MassRemoveDataIntegrity)
{
	constexpr EntityID ENTITY_COUNT = 1000;
	std::vector<EntityID> entities(ENTITY_COUNT);
	std::iota(entities.begin(), entities.end(), 0);

	struct MessComponent
	{
		int x;
	};

	for (auto id : entities) 
	{
		componentManager.addComponent<MessComponent>(id).x = id * 10;
	}


	// Remove every 3rd entity
	for (size_t i = 0; i < entities.size(); i += 3) 
	{
		componentManager.removeComponent<MessComponent>(entities[i]);
	}

	auto* pool = componentManager.getPool<MessComponent>();

	for (auto id : entities) 
	{
		if (id % 3 == 0) 
		{
			EXPECT_FALSE(pool->contains(id));
		}
		else {
			EXPECT_EQ(pool->getComponent(id).x, id * 10);
		}
	}
}

TEST_F(ComponentManagerTest, CrossPoolInteraction) 
{
	struct TestComponent2 { float y; };

	const EntityID sharedEntity = 100;

	// Add to both pools
	auto& c1 = componentManager.addComponent<TestComponent1>(sharedEntity);
	auto& c2 = componentManager.addComponent<TestComponent2>(sharedEntity);

	c1.x = 42;
	c2.y = 3.14f;

	// Remove from one pool
	componentManager.removeComponent<TestComponent1>(sharedEntity);

	// Verify cross-pool independence
	EXPECT_FALSE(componentManager.getPool<TestComponent1>()->contains(sharedEntity));
	EXPECT_TRUE(componentManager.getPool<TestComponent2>()->contains(sharedEntity));
	EXPECT_EQ(componentManager.getComponent<TestComponent2>(sharedEntity).y, 3.14f);
}
