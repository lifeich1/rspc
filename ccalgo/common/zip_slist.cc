#include "zip_slist.hh"
#include "assert.hh"
#include "config.h"
#include <vector>

#if defined(HAVE_GCC_ABI_DEMANGLE)
# include <cxxabi.h>
# define SHOWTYPE(V) \
    do { \
        const std::type_info  &ti = typeid(V); \
        int sta; \
        auto realname = abi::__cxa_demangle(ti.name(), 0, 0, &sta); \
        std::cout << "TYPEID(" << ti.name() << ") is " << realname << " \te" << sta << std::endl; \
    } while (0)
#else
# define SHOWTYPE(V)
#endif

int main() {
    typedef std::vector<int> Vec;
    Vec v0{1, 2, 3, 4, 5, 6};
    Vec v1{100, 99, 98, 97, 96};
    Vec v2{-1, -2, -3, -4, -5};

    {
        auto l = (A::zip_slist(v0.begin(), v0.begin() + 5) + v1).c(v2.begin(), v2.begin() + 4);
        auto it = l.begin();
        auto & t = std::get<0>(*it);
        ASSERT_EQ(t, 1);
        static_assert(std::is_assignable_v<decltype(t), int>);
        for (int i = 0; i < 4; ++i) {
            std::cout << "expect: " << v0[i] << ' ' << v1[i] << ' ' << v2[i] << std::endl;
            auto && r = *it++;
            int x0, x1, x2;
            std::tie(x0, x1, x2) = r;
            std::cout << "got: " << x0 << ' ' << x1 << ' ' << x2 << std::endl;
            ASSERT_EQ(r, std::make_tuple(v0[i], v1[i], v2[i]));
        }
        ASSERT_EQ(it, l.end());
    }

    {
        const auto & cv0 = v0;
        const auto & cv2 = v2;
        auto l = A::zip_slist(cv0) + v1 + cv2;
        auto it = l.begin();
        {
            auto && t = std::get<0>(*it);
            ASSERT_EQ(t, 1);
            static_assert(!std::is_assignable_v<decltype(t), int>);
        }
        {
            auto && t = std::get<2>(*it);
            ASSERT_EQ(t, -1);
            static_assert(!std::is_assignable_v<decltype(t), int>);
        }
        {
            auto && t = std::get<1>(*it);
            ASSERT_EQ(t, 100);
            static_assert(std::is_assignable_v<decltype(t), int>);
            t = -44;
            ASSERT_EQ(t, -44);
            ASSERT_EQ(v1[0], -44);
        }
        for (int i = 0; i < 5; ++i) {
            ASSERT_EQ(*it++, std::make_tuple(v0[i], v1[i], v2[i]));
        }
        ASSERT_EQ(it, l.end());
    }

    return 0;
}
