#ifndef __PROJECTS_ECS_SYSTEM_MANAGER_HPP__
#define __PROJECTS_ECS_SYSTEM_MANAGER_HPP__

#include "system.hpp"
#include "type_identifier.hpp"
#include "constants.hpp"
#include "signature.hpp"

#include <cassert>
#include <memory>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>


class SystemManager
{
private:
    std::unordered_map<std::type_index, std::shared_ptr<System>> systems_;
    std::unordered_map<std::type_index, Signature>               signatures_;  

public:
    template <typename System>
    std::shared_ptr<System> RegisterSystem()
    {
	const std::type_index index { std::type_index(typeid(System)) };

	assert(systems_.find(index) == systems_.end() && "Registering system more than once.");

	auto system { std::make_shared<System>() };
	systems_.insert( {index, system} );
	
	return system;
    }


    template<typename System>
    void SetSignature(Signature signature)
    {
	const std::type_index index { std::type_index(typeid(System)) };

	assert(systems_.find(index) != systems_.end() && "System used before registered.");

	signatures_.insert( {index, signature} );
    }

    
    void EntityDestroyed(EntityID entity)
    {
	for (auto const& pair : systems_)
	{
	    auto const& system = pair.second;


	    system->UnRegisterEntity(entity);
	}
    }

    
    void EntitySignatureChanged(EntityID entity, Signature entity_signature)
    {
	for (auto const& pair : systems_)
	{
	    auto const& index            { pair.first };
	    auto const& system           { pair.second };
	    auto const& system_signature { signatures_[index] };

	    if (entity_signature == system_signature)
	    {
		system->RegisterEntity(entity);
	    }
	    else
	    {
		system->UnRegisterEntity(entity);
	    }
	}
    }    
    
};

#endif
