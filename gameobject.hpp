#include "SFML/Graphics.hpp"
#include "collider.hpp"

class GameObject 
{
	private:
		sf::Vector2f shapeCenter;
		sf::VertexArray shape;
		Collider collider;

	public:
		GameObject(sf::VertexArray _shape);
		sf::VertexArray getShape();
		void draw(sf::RenderWindow& window);	
};