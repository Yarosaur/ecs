#ifndef __PROJECTS_ECS_CONSTANTS_HPP__
#define __PROJECTS_ECS_CONSTANTS_HPP__

#include <cstdint>

namespace konst
{
    inline constexpr std::size_t kMaxEntityID          {1024};
    inline constexpr std::size_t kMaxComponentsNumber  {1024};
    inline constexpr std::size_t kMaxComponentsType    {32};  // 32, 64, 96, 128
}

#endif
