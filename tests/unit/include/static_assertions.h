#pragma once

#include <type_traits>

namespace burda
{
namespace timers
{
namespace testing
{
template <typename class_type>
void assert_properties()
{
    static_assert(std::is_default_constructible<class_type>::value,
                  "Given class_type is not default constructible (should be)");

    static_assert(!std::is_move_constructible<class_type>::value,
                  "Given class_type is move constructible (shouldn't be)");
    static_assert(!std::is_move_assignable<class_type>::value,
                 "Given class_type is move assignable (shouldn't be)");

    static_assert(!std::is_copy_constructible<class_type>::value,
                 "Given class_type is copy constructible (shouldn't be)");
    static_assert(!std::is_copy_assignable<class_type>::value,
                 "Given class_type is copy assignable (shouldn't be)");
}
}
}
}
