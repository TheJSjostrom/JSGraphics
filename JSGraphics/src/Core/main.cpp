#include "Core/Application.h"
class Entity
{
public:

};

class Player : public Entity
{
public:
	void func()
	{

	}
};

int main()
{
	Player player;
	Entity& e = player;
	Player& p = static_cast<Player&>(e);
	e.func();
	JSG::Application app;
	app.Run();
}