#ifndef __PROJECTS_ECS_TYPE_IDENTIFIER_HPP__
#define __PROJECTS_ECS_TYPE_IDENTIFIER_HPP__

#include "constants.hpp"

#include <cstdint>
#include <bitset>

using EntityID        = std::size_t;
using Signature       = std::bitset<konst::kMaxComponentsType>;  
using ComponentIndex  = std::size_t;




#endif
