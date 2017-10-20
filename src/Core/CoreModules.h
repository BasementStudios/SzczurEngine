#pragma once

#include "Canvas.h"
#include "Loader.h"

namespace rat {
	class CoreModules {
    public:

        using Holder_t = std::tuple<Canvas, Loader>;

	private:

        Holder_t _modules;

    public:

        CoreModules() = default;

        CoreModules(const CoreModules&) = delete;

        CoreModules& operator = (const CoreModules&) = delete;

        template <typename T>
        T& get() {
            return std::get<T>(_modules);
        }

        template <typename T>
        const T& get() const {
            return std::get<T>(_modules);
        }
    };

}
