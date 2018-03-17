namespace rat {

	template <typename T>
	void MiniObject::addComponent() {
		components.emplace_back(new T(this));
		scriptComponents.set(components.back()->getComponentName(), dynamic_cast<T*>(components.back().get()));
	}

}
