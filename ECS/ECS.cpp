#include "ECS.hpp"

void Entity::addGroup(Group mGroup)
{
    groupBitSet[mGroup] = true;
    manager.AddToGroup(this, mGroup);
}

std::ostream &operator<<(std::ostream &stream, Entity &entity)
{
    std::cout << "Groups: ";
    for (Group group = 0; group < maxGroups; group++)
    {
        if (entity.hasGroup(group))
        {
            std::cout << group;
        }
    }
    std::cout << std::endl;
    return stream;
}
