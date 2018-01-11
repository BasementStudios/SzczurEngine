#pragma once

namespace rat
{

template <typename T>
T& Application::getModule()
{
	return _modules.getModule<T>();
}

template <typename T>
const T& Application::getModule() const
{
	return _modules.getModule<T>();
}

}
