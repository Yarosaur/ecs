#ifndef __PROJECTS_ECS_ECSWORLD_HPP__
#define __PROJECTS_ECS_ECSWORLD_HPP__

#include "entity_manager.hpp"
#include "component_manager.hpp"
#include "system_manager.hpp"

class ECSWorld
{
private:
    std::unique_ptr<ComponentManager> component_manager_;
    std::unique_ptr<EntityManager>    entity_manager_;
    //std::unique_ptr<EventManager>     event_manager_;
    std::unique_ptr<SystemManager>    system_manager_;

public:
    void Init()
    {
	component_manager_ = std::make_unique<ComponentManager>();
	entity_manager_    = std::make_unique<EntityManager>();
	//event_manager_     = std::make_unique<EventManager>();
	system_manager_    = std::make_unique<SystemManager>();
    }


    EntityID CreateEntity()
    {
	return entity_manager_->CreateEntity();
    }
    

    void DestroyEntity(EntityID entity)
    {
	entity_manager_    -> DestroyEntity(entity);
	component_manager_ -> EntityDestroyed(entity);
	system_manager_    -> EntityDestroyed(entity);
    }


    template<typename Component>
    void RegisterComponent()
    {
	component_manager_ -> RegisterComponent<Component>();
    }

    
    template<typename Component>
    void AddComponent(EntityID entity, Component component)
    {
	component_manager_->AddComponent<Component>(entity, component);

	Signature signature { entity_manager_ -> GetSignature(entity) };
	signature |= GetComponentSignature<Component>();
	entity_manager_ -> SetSignature(entity, signature);

	system_manager_ -> EntitySignatureChanged(entity, signature);
    }

    
    template<typename Component>
    void RemoveComponent(EntityID entity)
    {
	component_manager_->RemoveComponent<Component>(entity);

	Signature signature { entity_manager_->GetSignature(entity) };
	signature &= GetComponentSignature<Component>().flip();
	entity_manager_ -> SetSignature(entity, signature);

	system_manager_ -> EntitySignatureChanged(entity, signature);
    }

    
    template<typename Component>
    Component& GetComponent(EntityID entity)
    {
	return component_manager_ -> GetComponent<Component>(entity);
    }

    
    template<typename Component>
    std::shared_ptr<ComponentArray<Component>> GetComponentArray()
    {
	return component_manager_ -> GetComponentArray<Component>();
    }


    template<typename System>
    std::shared_ptr<System> RegisterSystem()
    {
	auto registered { system_manager_ -> RegisterSystem<System>() };
	registered -> RegisterWorld(std::make_shared<ECSWorld>(this));

	return registered;
    }

    
    template<typename System>
    void SetSystemSignature(Signature signature)
    {
	system_manager_ -> SetSignature<System>(signature);
    }
    
};

#endif
