#include <memory>

#include <gtest/gtest.h>

#include "../src/engine/resources/ResourceManager.hpp"

class TestResource {
    std::string name;
public:
    TestResource(const std::string& name) : name(name) {}
};

struct TestLoader {
    TestLoader() = default;
    std::shared_ptr<TestResource> load(const std::string& name) const {
        return std::make_shared<TestResource>(name);
    }
};

using TestManager = ResourceManager<TestResource, TestLoader>;

class TransparentManager : public TestManager {
public:
    TransparentManager() : TestManager(TestLoader()) {}

    bool has_resource(const std::string& name) const {
        auto it = resources_.find(name);
        return it != resources_.end();
    }

    bool resource_is_active(const std::string& name) const {
        if (has_resource(name)) {
            auto resource = resources_[name];
            return !resource.expired();
        } else {
            return false;
        }
    }

    unsigned int count_tracked_resources() const {
        return resources_.size();
    }

    unsigned int count_active_resources() const {
        unsigned int sum = 0;
        for (auto it : resources_) {
            if (!it.second.expired()) {
                sum += 1;
            }
        }
        return sum;
    }
};

struct TestOwner {
    std::shared_ptr<TestResource> resource;
};

TEST(ResourceTest, ResourceManagerShouldTrackResources) {
    TestOwner* owner1 = new TestOwner;
    TestOwner* owner2 = new TestOwner;
    TestOwner* owner3 = new TestOwner;
    TransparentManager manager{};

    EXPECT_EQ(manager.count_tracked_resources(), 0);
    EXPECT_EQ(manager.count_active_resources(), 0);

    owner1->resource = manager.get("foo");

    EXPECT_EQ(manager.count_tracked_resources(), 1);
    EXPECT_EQ(manager.count_active_resources(), 1);
    EXPECT_TRUE(manager.has_resource("foo"));
    EXPECT_TRUE(manager.resource_is_active("foo"));

    owner2->resource = manager.get("bar");

    EXPECT_EQ(manager.count_tracked_resources(), 2);
    EXPECT_EQ(manager.count_active_resources(), 2);
    EXPECT_TRUE(manager.has_resource("foo"));
    EXPECT_TRUE(manager.resource_is_active("foo"));
    EXPECT_TRUE(manager.has_resource("bar"));
    EXPECT_TRUE(manager.resource_is_active("bar"));

    owner3->resource = manager.get("foo");

    EXPECT_EQ(manager.count_tracked_resources(), 2);
    EXPECT_EQ(manager.count_active_resources(), 2);
    EXPECT_TRUE(manager.has_resource("foo"));
    EXPECT_TRUE(manager.resource_is_active("foo"));
    EXPECT_TRUE(manager.has_resource("bar"));
    EXPECT_TRUE(manager.resource_is_active("bar"));

    delete owner2;

    EXPECT_EQ(manager.count_tracked_resources(), 2);
    EXPECT_EQ(manager.count_active_resources(), 1);
    EXPECT_TRUE(manager.has_resource("foo"));
    EXPECT_TRUE(manager.resource_is_active("foo"));
    EXPECT_TRUE(manager.has_resource("bar"));
    EXPECT_FALSE(manager.resource_is_active("bar"));

    delete owner3;

    EXPECT_EQ(manager.count_tracked_resources(), 2);
    EXPECT_EQ(manager.count_active_resources(), 1);
    EXPECT_TRUE(manager.has_resource("foo"));
    EXPECT_TRUE(manager.resource_is_active("foo"));
    EXPECT_TRUE(manager.has_resource("bar"));
    EXPECT_FALSE(manager.resource_is_active("bar"));

    delete owner1;

    EXPECT_EQ(manager.count_tracked_resources(), 2);
    EXPECT_EQ(manager.count_active_resources(), 0);
    EXPECT_TRUE(manager.has_resource("foo"));
    EXPECT_FALSE(manager.resource_is_active("foo"));
    EXPECT_TRUE(manager.has_resource("bar"));
    EXPECT_FALSE(manager.resource_is_active("bar"));

    manager.clean();

    EXPECT_EQ(manager.count_tracked_resources(), 0);
    EXPECT_EQ(manager.count_active_resources(), 0);
    EXPECT_FALSE(manager.has_resource("foo"));
    EXPECT_FALSE(manager.resource_is_active("foo"));
    EXPECT_FALSE(manager.has_resource("bar"));
    EXPECT_FALSE(manager.resource_is_active("bar"));
}
