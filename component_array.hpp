#ifndef __PROJECTS_TGG_COMPONENT_ARRAY_HPP__
#define __PROJECTS_TGG_COMPONENT_ARRAY_HPP__

#include "constants.hpp"
#include "entity_map.hpp"
#include "type_identifier.hpp"

#include <memory>
#include <array>

class BaseComponentArray
{
public:
    virtual ~BaseComponentArray()                             = default;
    BaseComponentArray(const BaseComponentArray &)            = default;
    BaseComponentArray& operator=(const BaseComponentArray &) = default;
    BaseComponentArray(BaseComponentArray &&)                 = default;
    BaseComponentArray &operator=(BaseComponentArray &&)      = default;
    
    virtual void EntityDestroyed(EntityID entity) = 0;
};


template <typename Component>
class ComponentArray : public BaseComponentArray
{
private:
    std::size_t                                        size_;
    std::array<Component, konst::kMaxComponentsNumber> data_;
    EntityMap                                          entity_map_;
    
public:
    ComponentArray()
	: size_       {0}
	, data_       ()
	, entity_map_ ()
    {}
    
 
    void InsertData(EntityID entity, Component& component)
    {
	entity_map_.Add(entity, size_); // Register entity <-> component_index pair
	data_[size_++] = component;     // Put new component entry at end of data array
    }


    Component& GetData(EntityID entity)
    {
	assert(entity_map_.ContainEntity(entity) &&
	       "Retrieving non-existent component.");
	    
	return data_[entity_map_.GetIndex(entity)];
    }

    
    void RemoveData(EntityID entity)
    {
	assert(entity_map_.ContainEntity(entity) &&
	       "Removing non-existent component.");

	// Copy element at end into deleted element's place to maintain density
	size_t index_remove { entity_map_.GetIndex(entity) };
	size_t index_last   { size_ - 1 };
	data_[index_remove] = data_[index_last];

	// Update map to point to moved spot
	EntityID entity_update { entity_map_.GetEntity(index_last) };
	entity_map_.Update(entity_update, index_remove);
	entity_map_.Update(entity, index_last);
	entity_map_.Remove(entity);

	--size_;
    }



    void EntityDestroyed(EntityID entity) override
    {
	if ( entity_map_.ContainEntity(entity) )
	{
	    RemoveData(entity);
	}
    }
    
};


#endif
