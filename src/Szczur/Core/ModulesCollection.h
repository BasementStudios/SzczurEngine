#pragma once

#include <Szczur/Core/ModuleBase.h>

template<typename... Ts>
class ModulesCollection {
private:

    std::tuple<Ts...> _modules;

public:

    ModulesCollection() :
        _modules(((void)std::conditional_t<true, int, Ts>{}, _modules)...) {}

    template <typename T>
    T& get() {
        return std::get<T>(_modules);
    }

    template <typename T>
    const T& get() const {
        return std::get<T>(_modules);
    }
};
