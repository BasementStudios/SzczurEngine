#include "Szczur/Debug.hpp"
#include "Szczur/Application.hpp"

int main()
{
	INIT_DEBUG();

	rat::Application app;
	return app.run();
}
