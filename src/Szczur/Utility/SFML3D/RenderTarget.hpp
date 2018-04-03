#pragma once
#include "RenderStates.hpp"
#include "View.hpp"

namespace sf3d {
	class Vertex;
	class VertexArray;
	class Drawable;

	class RenderTarget {
	public:
		RenderTarget(const char* vertexPath, const char* fragmentPath, const glm::uvec2& size, float FOV = 45.f);
		~RenderTarget();

		void clear(float r, float g, float b);

		void draw(Drawable& drawable, RenderStates states);
		void draw(Drawable& drawable);

		void draw(const VertexArray& vertices, RenderStates states);
		void draw(const VertexArray& vertices);

		void simpleDraw(const VertexArray& vertices, RenderStates states);
		void simpleDraw(const VertexArray& vertices);
		void simpleDraw(Drawable& drawable);

		const View& getDefaultView() const;
		const View& getView() const;
		void setView(const View& view);

		

	private:
		virtual bool _setActive(bool state = true);

		Shader* _shader;
		RenderStates _states;

		glm::uvec2 _windowSize;

		//glm::mat4 _view;
		View _view;
		View _defaultView;
		glm::mat4 _projection;
	};
}