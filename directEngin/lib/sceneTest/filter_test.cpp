#include "pch.h"
#include "filters.h"


using namespace filters;

// Dummy component types for testing
struct Position { float x, y; };
struct Velocity { float dx, dy; };
struct Health { int hp; };

template<typename T>
void add(ComponentManager& mgr, const EntityID& id, const T& comp) 
{
    auto pool = mgr.getPool<T>();
    auto& ref = pool->add(id);
    ref = comp;
}

struct EntityHandler
{
    EntityID next() noexcept { return _last++; }

private:
    EntityID _last{ 0 };
};

TEST(ComponentViewTest, WithOnlyFilter) 
{
    EntityHandler eh{};
    ComponentManager mgr;
    // Create entities
    EntityID e1 = eh.next();
    EntityID e2 = eh.next();

    // Add components
    add<Position>(mgr, e1, { 1.0f, 2.0f });
    add<Velocity>(mgr, e1, { 0.1f, 0.2f });
    add<Position>(mgr, e2, { 3.0f, 4.0f });
    // e2 has no Velocity

    // View entities with both Position and Velocity
    ComponentView<With<Position, Velocity>, Without<>> view(mgr);
    std::vector<EntityID> found;


    for (auto [id, pos, vel] : view) {
        found.push_back(id);
        EXPECT_EQ(pos.x, 1.0f);
        EXPECT_EQ(pos.y, 2.0f);
        EXPECT_EQ(vel.dx, 0.1f);
        EXPECT_EQ(vel.dy, 0.2f);
    }
    EXPECT_EQ(found.size(), 1);
    EXPECT_EQ(found[0], e1);

}

TEST(ComponentViewTest, WithoutOnlyFilter) 
{
    EntityHandler eh{};
    ComponentManager mgr;
    EntityID e1 = eh.next();
    EntityID e2 = eh.next();
    EntityID e3 = eh.next();

    add<Position>(mgr, e1, { 5.0f, 6.0f });
    add<Position>(mgr, e2, { 7.0f, 8.0f });
    add<Health>(mgr, e2, { 100 });
    add<Position>(mgr, e3, { 9,10 });
    // View entities with Position but without Health
    ComponentView<With<Position>, Without<Health>> view(mgr);

    EXPECT_EQ(view.contains(e1), true);
    EXPECT_EQ(view.contains(e2), false);

    auto it = view.begin();
    auto first_ent = it.current();
    EXPECT_EQ(*first_ent, e1);
    ++it;
    auto second_ent = it.current();
    EXPECT_EQ(*second_ent, e3);

    std::vector<EntityID> found;
    
    for (auto [id, pos] : view)
    {
        found.push_back(id);
        EXPECT_EQ(view.contains(id), true);
        EXPECT_EQ(pos.x, view.get<Position>(id).x);
        EXPECT_EQ(pos.y, view.get<Position>(id).y);
    }
    EXPECT_EQ(found.size(), 2);
    EXPECT_EQ(found[0], e1);
    EXPECT_EQ(found[1], e3);
}


TEST(ComponentViewTest, WithAndWithoutCombined) 
{
    EntityHandler eh{};
    ComponentManager mgr;
    EntityID e1 = eh.next();
    EntityID e2 = eh.next();
    EntityID e3 = eh.next();

    add<Position>(mgr, e1, { 0.0f, 0.0f });
    add<Velocity>(mgr, e1, { 0.0f, 1.0f });

    add<Position>(mgr, e2, { 1.0f, 1.0f });
    add<Velocity>(mgr, e2, { 1.0f, 0.0f });
    add<Health>(mgr, e2, { 50 });

    // e3 has only Velocity
    add<Velocity>(mgr, e3, { 2.0f, 2.0f });

    // View entities with Position & Velocity but without Health
    ComponentView<With<Position, Velocity>, Without<Health>> view(mgr);
    std::vector<EntityID> found;
    for (auto [id, pos, vel] : view) {
        found.push_back(id);
    }
    // Only e1 matches all_of(Position, Velocity) and none_of(Health)
    EXPECT_EQ(found.size(), 1);
    EXPECT_EQ(found[0], e1);
}

TEST(ComponentViewTest, ContainsAndHasMethods) 
{
    EntityHandler eh{};
    ComponentManager mgr;
    EntityID e = eh.next();
    add<Position>(mgr, e, { 9.0f, 9.0f });

    ComponentView<With<Position>, Without<Health>> view(mgr);
    EXPECT_TRUE(view.contains(e));
    EXPECT_TRUE(view.has<Position>(e));
    EXPECT_TRUE(view.contains(e));

    // Add Health and re-check
    add<Health>(mgr, e, { 75 });
    EXPECT_FALSE(view.contains(e));
}

TEST(ComponentViewTest, EmptyResultView)
{
    ComponentManager mgr;
    EntityHandler eh{};
    EntityID e = eh.next();
    ComponentView<With<Health>, Without<>> view(mgr);
    EXPECT_EQ(view.begin(), view.end());
}

TEST(ComponentViewTest, ContainsFalseForMissingInclude) 
{
    ComponentManager mgr;
    EntityHandler eh{};
    EntityID e = eh.next();
    add<Velocity>(mgr, e, { 0.0f, 1.0f });
    ComponentView<With<Position>, Without<>> view(mgr);
    EXPECT_FALSE(view.contains(e));
}

TEST(ComponentViewTest, MultipleWithoutArgs) 
{
    ComponentManager mgr;
    EntityHandler eh{};
    EntityID e1 = eh.next();
    EntityID e2 = eh.next();
    add<Position>(mgr, e1, { 1.0f, 1.0f });
    add<Velocity>(mgr, e1, { 1.0f, 1.0f });
    add<Position>(mgr, e2, { 2.0f, 2.0f });
    add<Velocity>(mgr, e2, { 2.0f, 2.0f });
    add<Health>(mgr, e2, { 100 });

    ComponentView<With<Position>, Without<Velocity, Health>> view(mgr);
    std::vector<EntityID> found;
    for (auto [id, pos] : view) {
        found.push_back(id);
    }
    EXPECT_EQ(found.size(), 0);
}