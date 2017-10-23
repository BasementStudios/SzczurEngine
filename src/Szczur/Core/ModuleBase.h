#pragma once

#include <tuple>

template<typename... Ts>
class ModuleBase {
private:

    const std::tuple<std::add_lvalue_reference_t<Ts>...> _modules;

public:

    template<typename... Us>
    ModuleBase(std::tuple<Us...>& tuple) :
        _modules(std::get<Ts>(tuple)...) {}

    ModuleBase(const ModuleBase&) = delete;

    ModuleBase& operator = (const ModuleBase&) = delete;

protected:

    template <typename T>
    T& getModule() {
        return std::get<T&>(_modules);
    }

    template <typename T>
    const T& getModule() const {
        return std::get<T&>(_modules);
    }
};
