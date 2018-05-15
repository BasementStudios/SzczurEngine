namespace rat
{

template <typename T, typename... Ts>
inline void GlobalVarsHolder::create(const Key_t& key, Ts&&... args)
{
	_holder[key].DEPENDENT_TEMPLATE_SCOPE emplace<T>(std::forward<Ts>(args)...);
}

inline void GlobalVarsHolder::remove(const Key_t& key)
{
	_holder.erase(key);
}

template <typename T>
inline void GlobalVarsHolder::set(const Key_t& key, T&& arg)
{
	_holder[key] = std::forward<T>(arg);
}

template <typename T>
inline T& GlobalVarsHolder::get(const Key_t& key)
{
	return std::any_cast<T&>(_holder[key]);
}

}
