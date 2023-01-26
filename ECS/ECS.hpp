#include <algorithm>
#include <array>
#include <bitset>
#include <iostream>
#include <memory>
#include <vector>

class Component;
class Entity;
class Manager;

using ComponentID = std::size_t;
using Group = std::size_t;

/// @brief Returns component ID
inline ComponentID getNewComponentTypeID()
{
    static ComponentID lastID = 0u;
    return lastID++;
}
/// @brief Returns component type ID
/// @tparam T Component ID
template <typename T>
inline ComponentID getComponentTypeID() noexcept
{
    static ComponentID typeID = getNewComponentTypeID();
    return typeID;
}

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using GroupBitSet = std::bitset<maxGroups>;
using ComponentArray = std::array<Component *, maxComponents>;

class Component
{
public:
    Entity *entity;

    virtual ~Component(){};

    virtual void init(){};
    virtual void update(int deltaTime){};
    virtual void draw(){};
};

class Entity
{
private:
    Manager &manager;
    bool active = true;
    std::vector<std::unique_ptr<Component>> components;
    ComponentArray componentArray;
    ComponentBitSet componentBitSet;
    GroupBitSet groupBitSet;

public:
    Entity(Manager &mManager) : manager(mManager){};
    ~Entity(){};
    /// @brief Updates each component of the entity
    void update(int deltaTime)
    {
        for (auto &c : components)
            c->update(deltaTime);
    };
    /// @brief Draws each component of the entity
    void draw()
    {
        for (auto &c : components)
            c->draw();
    };
    /// @return true if the entity is active in the game
    bool isActive() const { return active; };
    /// @brief Disable an entity
    void destroy() { active = false; };

    /// @brief Checks if an entity is part of a particular group
    /// @param mGroup Particular group
    /// @return true if it is in this group
    bool hasGroup(Group mGroup)
    {
        return groupBitSet[mGroup];
    }
    /// @brief Adds the entity to the group
    /// @param mGroup Group that the entity will be part of
    void addGroup(Group mGroup);
    /// @brief Deletes the entity of a group
    /// @param mGroup Group that the entity will be deleted
    void delGroup(Group mGroup)
    {
        groupBitSet[mGroup] = false;
    };
    /// @brief Verifies if a component is attached
    /// @tparam T Type of the component
    /// @return true if the component is attached
    template <typename T>
    bool hasComponent() const
    {
        return componentBitSet[getComponentTypeID<T>()];
    }

    /// @brief Adds a component to an entity
    /// @tparam T Component Type
    /// @return Reference to new component
    template <typename T, typename... Targs>
    T &addComponents(Targs &&...margs)
    {
        T *c(new T(std::forward<Targs>(margs)...));
        c->entity = this;
        std::unique_ptr<Component> uPtr{c};
        components.emplace_back(std::move(uPtr));

        componentArray[getComponentTypeID<T>()] = c;
        componentBitSet[getComponentTypeID<T>()] = true;

        c->init();
        return *c;
    }

    /// @brief Get a component by its TypeID
    /// @tparam T Component Type Id
    /// @return Reference to component
    template <typename T>
    T &getComponent() const
    {
        auto ptr(componentArray[getComponentTypeID<T>()]);
        return *static_cast<T *>(ptr);
    }

    friend std::ostream &operator<<(std::ostream &stream, Entity &entity);
};

/// @brief Manages all entities of the game
class Manager
{
private:
    std::vector<std::unique_ptr<Entity>> entities;
    std::array<std::vector<Entity *>, maxGroups> groupedEntities;

public:
    ~Manager(){}
    /// @brief Updates each entity of the game
    void update(int deltaTime)
    {
        for (auto &e : entities)
            e->update(deltaTime);
    };
    /// @brief Draw each entity of the game
    void draw()
    {
        for (auto &e : entities)
            e->draw();
    };
    /// @brief Filter the list of entities and removes inactive ones
    void refresh()
    {
        for (auto i(0u); i < maxGroups; i++)
        {
            auto &v(groupedEntities[i]);
            v.erase(
                std::remove_if(
                    std::begin(v),
                    std::end(v),
                    [i](Entity *mEntity)
                    {
                        return !mEntity->isActive() || !mEntity->hasGroup(i);
                    }),
                std::end(v));
        }
        entities.erase(
            std::remove_if(std::begin(entities), std::end(entities),
                           [](const std::unique_ptr<Entity> &mEntity)
                           {
                               return !mEntity->isActive();
                           }),
            std::end(entities));
    };

    void AddToGroup(Entity *mEntity, Group mGroup)
    {
        groupedEntities[mGroup].emplace_back(mEntity);
    }

    std::vector<Entity *> &getGroup(Group mGroup)
    {
        return groupedEntities[mGroup];
    }

    /// @brief Add an entity to the game
    /// @return New entity reference
    Entity &addEntity()
    {
        Entity *e = new Entity(*this);
        std::unique_ptr<Entity> uPtr{e};
        entities.emplace_back(std::move(uPtr));
        return *e;
    };
};