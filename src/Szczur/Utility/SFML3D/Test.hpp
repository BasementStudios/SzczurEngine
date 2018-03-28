#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include "Transform.hpp"
#include "glm/gtx/string_cast.hpp"


namespace rat {
    void test() {
        sf3d::Transform trans;
        trans.scale({2.f, 3.f, 4.f});

        std::cout << glm::to_string(trans._mat) << '\n';
    }
}