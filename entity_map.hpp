#ifndef __PROJECTS_ECS_ENTITY_MAP_HPP__
#define __PROJECTS_ECS_ENTITY_MAP_HPP__

#include "type_identifier.hpp"

#include <unordered_map>
#include <cassert>

class EntityMap
{
private:
    std::unordered_map<EntityID, ComponentIndex>   entity_to_index_;
    std::unordered_map<ComponentIndex, EntityID>   index_to_entity_;
    
public:
    bool ContainEntity(EntityID entity)
    {
	if (entity_to_index_.find(entity) == entity_to_index_.end())
	{
	    return false;
	}
	
	return true;
    }
    
    
    EntityID GetEntity(ComponentIndex index)
    {
	assert(index_to_entity_.find(index) != index_to_entity_.end() &&
	       "Removing non-existent entity.");
	
	return index_to_entity_[index];
    }

    
    ComponentIndex GetIndex(EntityID entity)
    {
	
	assert(entity_to_index_.find(entity) != entity_to_index_.end() &&
	       "Removing non-existent component.");
	return entity_to_index_[entity];
    }

    
    void Add(EntityID entity, ComponentIndex index)
    {
	assert(entity_to_index_.find(entity) == entity_to_index_.end() &&
	       "Component added to same entity more than once.");
	
	entity_to_index_.insert( {entity, index} );
	index_to_entity_.insert( {index, entity} );
    }

    
    void Update(EntityID entity, ComponentIndex index)
    {
	entity_to_index_.insert_or_assign(entity, index);
	index_to_entity_.insert_or_assign(index, entity);
    }

    
    void Remove(EntityID entity)
    {
	assert(entity_to_index_.find(entity) != entity_to_index_.end() &&
	       "Removind non-existent component.");
	
	ComponentIndex index {entity_to_index_[entity]};
	index_to_entity_.erase(index);
	entity_to_index_.erase(entity);
    }
};

#endif 
