#include "gameobject.hpp"

sf::VertexArray& GameObject::getShape() 
{
	return shape;
}

void GameObject::setShape(sf::VertexArray _shape) 
{
	shape = _shape;
}

void GameObject::draw(sf::RenderWindow& window) 
{
	window.draw(shape);
	collider.drawDebug(window);
}

GameObject::GameObject(sf::VertexArray _shape) 
{	
	shape = _shape;
	collider = Collider(_shape);
}