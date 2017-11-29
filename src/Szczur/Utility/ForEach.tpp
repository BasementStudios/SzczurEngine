namespace rat {
	namespace detail {
		template<typename T, typename F, size_t... Ns>
		inline constexpr void forEach(T&& tuple, F&& function, std::index_sequence<Ns...>) {
			(std::invoke(function, std::get<Ns>(tuple)), ...);
		}
	}

	template<typename T, typename F>
	inline constexpr void forEach(T&& tuple, F&& function) {
		detail::forEach(tuple, function, std::make_index_sequence<std::tuple_size_v<std::decay_t<T>>>{});
	}
}
