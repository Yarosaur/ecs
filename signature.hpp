#ifndef __PROJECTS_ECS_SIGNATURE_HPP__
#define __PROJECTS_ECS_SIGNATURE_HPP__

#include "constants.hpp"
#include "type_identifier.hpp"

#include <bitset>
#include <vector>


struct ComponentCounter
{
    static std::size_t component_type_counter;
};
std::size_t ComponentCounter::component_type_counter { 0 };


// Util method for getting signature of given component type
template <typename Component>
Signature GetComponentSignature()
{
    static Signature component_signature;
    component_signature.set( ComponentCounter::component_type_counter++ );
    return component_signature;
}


// Util method for getting signature of given system type
template <class ...Components>
Signature ConstructSystemSignature()
{
    static Signature system_signature;
    std::size_t      components_number { sizeof...(Components) };
    if (components_number == 0)
    {
	system_signature.set(); // all bits true
    }
    else
    {
	// Unpack the template parameters into an initializer list
	std::vector<Signature> components_signature = {  GetComponentSignature<Components>() ... };
	for (std::size_t i {0}; i < components_number; ++i)
	{
	    system_signature |= components_signature[i];
	}
    }
    return system_signature;
}

#endif
