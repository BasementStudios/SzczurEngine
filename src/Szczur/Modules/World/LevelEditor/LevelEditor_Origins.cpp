#include "LevelEditor.hpp"


namespace rat {

	void LevelEditor::_prepareOrigins() {

	// Rectangle
		// Out
		_originRectOut.setSize({100.f, 100.f});
		_originRectOut.setOrigin({50.f, 50.f, -5.f});
		_originRectOut.setColor({0.6f, 0.f, 0.7f, 0.4f});
		// In
		_originRectIn.setSize({70.f, 70.f});
		_originRectIn.setOrigin({35.f, 35.f, -10.f});
		_originRectIn.setColor({1.f, 1.f, 0.f, 0.6f});
		// Out (selected)
		_originRectOutSel.setSize({100.f, 100.f});
		_originRectOutSel.setOrigin({50.f, 50.f, -5.f});
		_originRectOutSel.setColor({1.f, 0.3f, 1.f, 0.4f});
		// In (selected)
		_originRectInSel.setSize({80.f, 80.f});
		_originRectInSel.setOrigin({40.f, 40.f, -10.f});
		_originRectInSel.setColor({0.f, 1.f, 1.f, 0.6f});

	// Circle
		// Out
		_originCirOut.setRadius(50.f);
		_originCirOut.setOrigin({50.f, 50.f, -5.f});
		_originCirOut.setColor({0.6f, 0.f, 0.7f, 0.4f});
		// In
		_originCirIn.setRadius(35.f);
		_originCirIn.setOrigin({35.f, 35.f, -10.f});
		_originCirIn.setColor({0.05f, 0.7f, 0.4f, 0.6f});
		// Out (selected)
		_originCirOutSel.setRadius(50.f);
		_originCirOutSel.setOrigin({50.f, 50.f, -5.f});
		_originCirOutSel.setColor({1.f, 0.3f, 1.f, 0.4f});
		// In (selected)
		_originCirInSel.setRadius(40.f);
		_originCirInSel.setOrigin({40.f, 40.f, -10.f});
		_originCirInSel.setColor({0.1f, 1.f, 0.4f, 0.6f});
	}

	void LevelEditor::_renderOriginRectangle(const glm::vec3& position, bool selected, sf3d::RenderTarget& target) {
		if(selected) {
			_originRectOutSel.setPosition(position);
			target.draw(_originRectOutSel);
			_originRectInSel.setPosition(position);
			target.draw(_originRectInSel);
		}
		else {
			_originRectOut.setPosition(position);
			target.draw(_originRectOut);
			_originRectIn.setPosition(position);
			target.draw(_originRectIn);
		}
	}

	void LevelEditor::_renderOriginCircle(const glm::vec3& position, bool selected, sf3d::RenderTarget& target) {
		if(selected) {
			_originCirOutSel.setPosition(position);
			target.draw(_originCirOutSel);
			_originCirInSel.setPosition(position);
			target.draw(_originCirInSel);
		}
		else {
			_originCirOut.setPosition(position);
			target.draw(_originCirOut);
			_originCirIn.setPosition(position);
			target.draw(_originRectIn);
		}
	}

	void LevelEditor::_renderOrigins(sf3d::RenderTarget& target) {


		// Render origins for background group
		for(auto& entity : _scenes.getCurrentScene()->getEntities("background")) {
			if(_objectsList.isEntitySelected(entity.get())) {
				_renderOriginRectangle(entity->getPosition(), true, target);
			}
			else {
				_renderOriginRectangle(entity->getPosition(), false, target);
			}
		}

		// Render origins for path group
		for(auto& entity : _scenes.getCurrentScene()->getEntities("path")) {			
			if (_objectsList.isEntitySelected(entity.get())) {
				_renderOriginRectangle(entity->getPosition(), true, target);
			}
			else {
				_renderOriginRectangle(entity->getPosition(), false, target);
			}
		}

		// Render origins for foreground group
		for(auto& entity : _scenes.getCurrentScene()->getEntities("foreground")) {			
			if (_objectsList.isEntitySelected(entity.get())) {
				_renderOriginRectangle(entity->getPosition(), true, target);
			}
			else {
				_renderOriginRectangle(entity->getPosition(), false, target);
			}
		}

		// Render origins for single group
		for(auto& entity : _scenes.getCurrentScene()->getEntities("single")) {			
			if (_objectsList.isEntitySelected(entity.get())) {
				_renderOriginRectangle(entity->getPosition(), true, target);
			}
			else {
				_renderOriginRectangle(entity->getPosition(), false, target);
			}
		}

		// Render origins for entries group
		for(auto& entity : _scenes.getCurrentScene()->getEntities("entries")) {			
			if (_objectsList.isEntitySelected(entity.get())) {
				_renderOriginCircle(entity->getPosition(), true, target);
			}
			else {
				_renderOriginCircle(entity->getPosition(), false, target);
			}
		}

		sf3d::CircleShape circ;
		sf3d::CircleShape circ2;
		circ.rotate({-90.f, 0.f, 0.f});
		circ.setColor({1.f, 0.f, 1.f, 0.2f});
		circ2.setColor({0.f, 1.f, 0.f, 0.2f});
		circ2.setRadius(50.f);
		
		circ2.setOrigin({50.f, 50.f, 0.f});

		sf3d::RectangleShape rect;
		rect.rotate({ -90.f, 0.f, 0.f });

		_scenes.getCurrentScene()->forEach([&](const std::string& group, Entity& entity){
			if(auto* comp = entity.getComponentAs<InteractableComponent>(); comp != nullptr) {
				circ.setColor({1.f, 0.f, 1.f, 0.2f});
				circ.setPosition(entity.getPosition() + glm::vec3{0.f, comp->getHeight(), 0.f});
				float r = comp->getDistance();
				circ.setRadius(r);
				circ.setOrigin({r, r, 0.f});
				target.draw(circ);
			}
			if(auto* comp = entity.getComponentAs<TriggerComponent>(); comp != nullptr) {
				if (comp->getShapeType() == TriggerComponent::Shape::Circle) {
					circ.setColor({ 1.f, 1.f, 0.f, 0.2f });
					circ.setPosition(entity.getPosition());
					float r = comp->getRadius();
					circ.setRadius(r);
					circ.setOrigin({ r, r, 0.f });
					target.draw(circ);
				}
				else if (comp->getShapeType() == TriggerComponent::Shape::Rectangle) {
					rect.setColor({ 1.f, 1.f, 0.f, 0.2f });
					rect.setPosition(entity.getPosition() + glm::vec3(0.f, 10.f, 0.f));
					auto& size = comp->getRectSize();
					rect.setSize(size);
					rect.setOrigin({ size.x / 2.f, size.y / 2.f, 0.f });
					target.draw(rect);
				}
			}
			if(auto* comp = entity.getComponentAs<ColliderComponent>(); comp != nullptr) { 
                if (comp->isCircleCollider()) { 
                    circ.setColor({ 0.13f, 0.59f, 0.95f, 0.2f }); 
                    circ.setPosition(entity.getPosition()); 
                    float r = comp->getCircleRadius(); 
                    circ.setRadius(r); 
                    circ.setOrigin({ r, r, 0.f }); 
                    target.draw(circ); 
                } 
                if (comp->isBoxCollider()) { 
                    rect.setColor({ 0.3f, 0.69f, 0.31f, 0.2f }); 
                    rect.setPosition(entity.getPosition() + glm::vec3(0.f, 10.f, 0.f)); 
                    auto& size = comp->getBoxSize(); 
                    rect.setSize(size); 
                    rect.setOrigin({ size.x / 2.f, size.y / 2.f, 0.f }); 
                    target.draw(rect); 
                } 
            } 
		});
	}

}
