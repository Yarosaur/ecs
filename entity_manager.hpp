#ifndef __PROJECTS_ECS_ENTITY_MANAGER_HPP__
#define __PROJECTS_ECS_ENTITY_MANAGER_HPP__

#include "constants.hpp"
#include "type_identifier.hpp"

#include <cstdint>
#include <stack>
#include <cassert>
#include <array>


class EntityManager
{
private:
    std::stack<EntityID>                        entity_pool_;  // free (also reusable) entity IDs
    std::size_t                                 available_;    // count free ID
    std::array<Signature, konst::kMaxEntityID>  signatures_;   // Signatures of the live entities
    

public:
    EntityManager()
	: available_  {konst::kMaxEntityID}
	, signatures_ ()
    {
	// Initialize the stack with all possible entity IDs
	for (std::size_t i {0}; i < konst::kMaxEntityID; ++i)
	{
	    entity_pool_.push(konst::kMaxEntityID - 1 - i);
	}
     }


    EntityID CreateEntity()
    {
	assert(available_ > 0 && "Too many entities in existence.");
	
	// Take an ID from the top of the stack
	EntityID id  {entity_pool_.top()};
	entity_pool_.pop();
	--available_;;
	
	return id;
    }


    void DestroyEntity(EntityID entity)
    {
	assert(entity < konst::kMaxEntityID && "Entity out of range.");

	// Clear signature
	signatures_[entity].reset();
	
	// Put the destroyed ID at the top of the stack
	entity_pool_.push(entity);
	++available_;
    }


    void SetSignature(EntityID entity, Signature signature)
    {
	assert(entity < konst::kMaxEntityID && "Entity out of range.");

	signatures_[entity] = signature;
    }

    
    Signature GetSignature(EntityID entity)
    {
	assert(entity < konst::kMaxEntityID && "Entity out of range.");

	return signatures_[entity];
    }

};

#endif
