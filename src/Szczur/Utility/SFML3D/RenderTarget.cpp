#include "RenderTarget.hpp"
#include "VertexArray.hpp"
#include "Drawable.hpp"
#include "Vertex.hpp"
#include <SFML/Graphics/Color.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include <iostream>


namespace sf3d {

	void RenderTarget::_setBasicValues() {
		_FOVx = glm::degrees(
			2 * glm::atan(glm::tan(glm::radians(_FOVy / 2.f)) * ((float)_windowSize.x / (float)_windowSize.y))
		);
		_halfFOVxTan = glm::tan(glm::radians(_FOVx / 2.f));
		_halfFOVyTan = glm::tan(glm::radians(_FOVy / 2.f));
		_view.create(2.f / (float)_windowSize.y, {0.f, 0.f, 3 * (float)_windowSize.x / 2.f});
		_defaultView.create(2.f / (float)_windowSize.y, {0.f, 0.f, 3 * (float)_windowSize.x / 2.f});
		setRenderDistance(100.f);
	}

	RenderTarget::RenderTarget() {

	}

	RenderTarget::RenderTarget(const glm::uvec2& size, float FOV, ShaderProgram* program) :
	_windowSize(size),
	_FOVy(FOV)
		{
		_states.shader = program;
		_setBasicValues();
	}

	RenderTarget::~RenderTarget() {

	}

	void RenderTarget::create(const glm::uvec2& size, float FOV, ShaderProgram* program) {
		_windowSize = size;
		_FOVy = FOV;
		_states.shader = program;
		_setBasicValues();

	}

	void RenderTarget::setProgram(ShaderProgram * program) {
		_states.shader = program;
	}

	
	void RenderTarget::setRenderDistance(float renderDistance) {
		_renderDistance = renderDistance;
		_projection = glm::perspective(glm::radians(_FOVy), (float)_windowSize.x / (float)_windowSize.y, 0.1f, _renderDistance);
	}
	float RenderTarget::getRenderDistance() const {
		return _renderDistance;
	}

	void RenderTarget::clear(float r, float g, float b, float a, GLbitfield flags) {
		if(_setActive()) {
			glClearColor(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
			glClear(flags);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}

	void RenderTarget::clear(const sf::Color& color, GLbitfield flags) {
		if(_setActive()) {
			glClearColor(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f);
			glClear(flags);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}

	void RenderTarget::draw(const Drawable & drawable, RenderStates states) {
		drawable.draw(*this, states);
	}

	void RenderTarget::draw(const Drawable & drawable) {
		draw(drawable, _states);
	}



	void RenderTarget::draw(const VertexArray& vertices, RenderStates states) {
		if(vertices.getSize() > 0 && _setActive()) {
			vertices.update();
			ShaderProgram* shader;
			if(states.shader)
				shader = states.shader;
			else if(_states.shader)
				shader = _states.shader;
			else {
				std::cout << "NO SHADER AVAILABLE!!!!\n";
				return;
			}

			for (int i = 0; i < 3; ++i)
				states.transform.getMatrix()[3][i] *= 2.0f / static_cast<float>(_windowSize.y);

			shader->use();
			shader->setUniform("positionFactor", 2.0f / static_cast<float>(_windowSize.y));
			shader->setUniform("model", states.transform.getMatrix());
			shader->setUniform("view", _view.getTransform().getMatrix());
			shader->setUniform("projection", _projection);
			shader->setUniform("isTextured", states.texture != nullptr);

			if(states.texture)
				states.texture->bind();

			vertices.bind();

			glDrawArrays(vertices.getPrimitiveType(), 0, vertices.getSize());
			states.texture->unbind();
			glBindVertexArray(0);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}

	void RenderTarget::simpleDraw(const VertexArray & vertices, RenderStates states) {
		if(vertices.getSize() > 0 && _setActive()) {
			if(states.shader)
				states.shader->use();
			else
				_states.shader->use();

			if(states.texture)
				states.texture->bind();

			vertices.bind();

			glDrawArrays(vertices.getPrimitiveType(), 0, vertices.getSize());

			states.texture->unbind();
			glBindVertexArray(0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}

	void RenderTarget::draw(const VertexArray & vertices) {
		draw(vertices, _states);
	}

	void RenderTarget::simpleDraw(const VertexArray & vertices) {
		simpleDraw(vertices, _states);
	}

	void RenderTarget::simpleDraw(Drawable & drawable) {
		draw(drawable, _states);
	}

	const View& RenderTarget::getDefaultView() const {
		return _defaultView;
	}

	const View& RenderTarget::getView() const {
		return _view;
	}

	void RenderTarget::setView(const View& view) {
		_view = view;
	}

	Linear RenderTarget::getLinerByScreenPos(const glm::vec2 & pos) const {
		float x = glm::atan(
			( 2.f * (pos.x / (float)_windowSize.x) - 1.f) * _halfFOVxTan
		);

		float y = glm::atan(
			(-2.f * (pos.y / (float)_windowSize.y) + 1.f) * _halfFOVyTan
		);

		float siny = glm::sin(y);
		float cosy = glm::cos(y);
		float sinx = glm::sin(x);
		float cosx = glm::cos(x);

		glm::vec3 rotation{
			cosy * sinx,
			siny * cosx,
			-cosy * cosx
		};
		rotation = glm::rotateX(rotation, glm::radians(-_view.getRotation().x));
		rotation = glm::rotateY(rotation, glm::radians(-_view.getRotation().y));
		return Linear(_view.getCenter(), {
			rotation.x,
			rotation.y,
			rotation.z
		});
	}

	bool RenderTarget::_setActive(bool /*state*/) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return true;
	}


}
