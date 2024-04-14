#include "SFML/Graphics.hpp"

class Collider 
{
	private:
		sf::Vector2f colliderCenter;
		sf::VertexArray colliderBounds;
		sf::Color colliderDebugColor;
		bool debugEnabled;

	public:
		Collider(sf::VertexArray shape);
		Collider();
		void resize();
		void drawDebug(sf::RenderWindow& window);
};

