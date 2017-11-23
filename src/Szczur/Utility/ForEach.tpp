namespace rat {
	namespace detail {
		template<size_t I, typename T, typename F>
		inline constexpr void forEach(T&& tuple, F&& function) {
			std::invoke(function, std::get<std::tuple_size_v<std::decay_t<T>> - I>(tuple));

			if constexpr (I > 1)
				forEach<I - 1>(tuple, function);
		}
	}

	template<typename T, typename F>
	inline constexpr void forEach(T&& tuple, F&& function) {
		detail::forEach<std::tuple_size_v<std::decay_t<T>>>(tuple, function);
	}
}
