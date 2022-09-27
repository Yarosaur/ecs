#ifndef __PROJECTS_ECS_COMPONENT_MANAGER_HPP__
#define __PROJECTS_ECS_COMPONENT_MANAGER_HPP__

#include "component_array.hpp"
#include "constants.hpp"
#include "type_identifier.hpp"
#include "signature.hpp"

#include <unordered_map>

class ComponentManager
{
private:
    std::unordered_map<Signature, std::shared_ptr<BaseComponentArray>> component_arrays_;


public:
    template<typename Component>
    std::shared_ptr<ComponentArray<Component>> GetComponentArray()
    {
	Signature mask { GetComponentSignature<Component>() };

	assert(component_arrays_.find(mask) != component_arrays_.end() &&
	       "Component not registered before use.");

	return std::dynamic_pointer_cast<ComponentArray<Component>>(component_arrays_[mask]);
    }

    
    template<typename Component>
    void RegisterComponent()
    {
	// Every component have unique signature
	Signature mask { GetComponentSignature<Component>() };
	
	assert(component_arrays_.find(mask) == component_arrays_.end() &&
	       "Registering component type more than once.");

	component_arrays_.insert( {mask, std::make_shared<ComponentArray<Component>>()} );
    }

    template<typename Component>
    void AddComponent(EntityID entity, Component component)
    {
	GetComponentArray<Component>()->InsertData(entity, component);
    }


    template<typename Component>
    Component& GetComponent(EntityID entity)
    {
	return GetComponentArray<Component>()->GetData(entity);
    }


    template<typename Component>
    void RemoveComponent(EntityID entity)
    {
	GetComponentArray<Component>()->RemoveData(entity);
    }


    void EntityDestroyed(EntityID entity)
    {
	for (auto const& pair : component_arrays_)
	{
	    auto const& component_array  { pair.second };

	    component_array->EntityDestroyed(entity);
	}
    }
};

#endif
