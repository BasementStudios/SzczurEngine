#pragma once
#include <SFML/Graphics/Color.hpp>
#include "RenderStates.hpp"
#include "View.hpp"
#include "Linear.hpp"
#include "ShaderProgram.hpp"

namespace sf3d {
	struct Vertex;
	class VertexArray;
	class Drawable;

	class RenderTarget {
	public:
		RenderTarget();
		RenderTarget(const glm::uvec2& size, float FOV = 45.f, ShaderProgram* program = nullptr);
		~RenderTarget();

		void create(const glm::uvec2& size, float FOV = 45.f, ShaderProgram* program = nullptr);

		void setProgram(ShaderProgram* program);

		void clear(float r, float g, float b, float a, GLbitfield flags);
		void clear(const sf::Color& color, GLbitfield flags);

		void draw(const Drawable& drawable, RenderStates states);
		void draw(const Drawable& drawable);

		void draw(const VertexArray& vertices, RenderStates states);
		void draw(const VertexArray& vertices);

		void simpleDraw(const VertexArray& vertices, RenderStates states);
		void simpleDraw(const VertexArray& vertices);
		void simpleDraw(Drawable& drawable);

		const View& getDefaultView() const;
		const View& getView() const;
		void setView(const View& view);

		Linear getLinerByScreenPos(const glm::vec2& pos) const;



	private:
		void _setBasicValues();
		virtual bool _setActive(bool state = true);

		RenderStates _states;

		glm::uvec2 _windowSize;

		float _FOVy;
		float _FOVx;
		float _halfFOVxTan;
		float _halfFOVyTan;
		View _view;
		View _defaultView;
		glm::mat4 _projection;
	};
}
