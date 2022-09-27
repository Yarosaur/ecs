#ifndef __PROJECTS_ECS_SYSTEM_HPP__
#define __PROJECTS_ECS_SESTEM_HPP__

#include "type_identifier.hpp"
#include "constants.hpp"
#include "signature.hpp"

#include <set>
#include <bitset>
#include <memory>

class ECSWorld;


class System
{
protected:
    std::set<EntityID>                      entities_;
    // std::bitset<konst::kMaxComponentsType>  signature_;
    std::shared_ptr<ECSWorld>               world_;
    
public:
    System() = default;
    virtual ~System() = default;
    System(const System &) = default;
    System &operator=(const System &) = default;
    System(System &&) = default;
    System &operator=(System &&) = default;

    // Called before the game starts but after the world initialize
    virtual void Init() = 0;

    
    // Called every game update
    virtual void Update(float dt) = 0;

    
    // When a system is added to the world, the world will register itself
    void RegisterWorld(std::shared_ptr<ECSWorld> world)
    {
	world_ = world;
    }


    // When a component is added such that this system should begin acting on it,
    // register will be called.
    void RegisterEntity(const EntityID& entity)
    {
	entities_.insert(entity);
    }
    

    // If a component is removed from an entity such that the system should stop
    // acting on it, UnRegister will be called.
    void UnRegisterEntity(const EntityID& entity)
    {
	entities_.erase(entity);
    }

};

#endif
