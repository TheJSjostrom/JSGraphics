#include "Core/Application.h"
#include <print>
#include <array>
#include <iostream>

static int s_HeapAllocationCount = 0;
static int s_CopiedCount = 0;
static int s_MoveCount = 0;

/*
void* operator new(size_t size)
{		
	s_HeapAllocationCount++;
	void* ptr = malloc(size);
	std::cout << ptr << std::endl;
	return ptr;
}
*/

class Player
{
public:

	Player() = default;

	Player(float x, float y, float z) :
		m_X(x),
		m_Y(y),
		m_Z(z)
	{
		std::cout << "Construction " << m_X << std::endl;
	}

	~Player()
	{
		std::cout << "Destruction " << m_X << std::endl;
	}

	Player(const Player& other) :
		m_X(other.m_X),
		m_Y(other.m_Y),
		m_Z(other.m_Z)
	{
		s_CopiedCount++;
		std::cout << "Copied " << other.m_X << std::endl;
	}

	Player(Player&& other) noexcept :
		m_X(other.m_X),
		m_Y(other.m_Y),
		m_Z(other.m_Z)
	{
		s_MoveCount++;
		std::cout << "Moved " << other.m_X << std::endl;
	}

	float GetX() const { return m_X; }
private:
	float m_X, m_Y, m_Z;
};

class String
{
public:
	String(const char* name, const Player& pl) :
		m_Player(pl),
		m_Size(strlen(name) + 1),
		m_Data(new char[m_Size])
	{
		std::cout << "CONSTRUCTED" << std::endl;

		memcpy(m_Data, name, m_Size);
	}

	~String()
	{
		std::cout << "DELETED" << std::endl;

		delete m_Data;
	}

	String(const String& other) :
		m_Size(other.m_Size),
		m_Data(new char[other.m_Size])
	{
		std::cout << "COPIED" << std::endl;
		s_CopiedCount++;

		memcpy(m_Data, other.m_Data, m_Size);
	}

	String(String&& other) noexcept :
		m_Size(other.m_Size),
		m_Data(other.m_Data)
	{
		std::cout << "MOVIED" << std::endl;
		s_MoveCount++;

		other.m_Size = 0;
		other.m_Data = nullptr;
	}

	uint32_t GetSize() const { return m_Size; }
private:
	Player m_Player;
	char* m_Data;
	uint32_t m_Size;
};




class Human
{
public:
	Human() = default;

private:
	int x = 2;
	char cha = 2.0f;
	int x2 = 3;
	int x3 = 3;
	int x4 = 3;
	int x5 = 3;
};

void CreateString(const String& title)
{
	std::cout << title.GetSize() << std::endl;
}

int main()
{
	/*
	Player pl = { 1.0f, 2.0f, 3.0f };

	String Title = "JSGraphics";
	String& NewNameTitle = Title;

	String CopyTitle = Title;
	String Move = std::move(Title);

	 /*
	std::vector<Player> players = std::vector<Player>();
	players.push_back(Player(1.0f, 1.0f, 1.0f));
	players.push_back(Player(2.0f, 2.0f, 2.0f));
	players.push_back(Player(3.0f, 3.0f, 3.0f));


	players.emplace_back(1.0f, 1.0f, 1.0f);
	players.emplace_back(2.0f, 2.0f, 2.0f);
	players.emplace_back(3.0f, 3.0f, 3.0f);
	players.emplace_back(4.0f, 4.0f, 4.0f);
	players.emplace_back(5.0f, 5.0f, 5.0f);


	std::cout << "Heap Allocation count: " << s_HeapAllocationCount << std::endl;
	std::cout << "Copy count: " << s_CopiedCount << std::endl;
	std::cout << "Move count: " << s_MoveCount << std::endl;
	*/
	JSG::Application app;
	app.Run();
}