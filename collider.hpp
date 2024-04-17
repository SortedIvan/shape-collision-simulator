#include "SFML/Graphics.hpp"

class MathUtility; // forward declare MathUtility

class Collider 
{
	private:
		sf::Vector2f colliderCenter;
		sf::VertexArray colliderBounds;
		sf::Color colliderDebugColor;
		bool debugEnabled;
		MathUtility* math;

	public:
		Collider(sf::VertexArray shape);
		Collider();
		void resize(float amount, bool inwards, sf::RenderWindow& window);
		void drawDebug(sf::RenderWindow& window);
		void moveCollider(sf::Vector2f displacement);
		sf::VertexArray& getColliderBounds();
};

