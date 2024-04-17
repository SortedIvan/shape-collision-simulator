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

void GameObject::moveGameObject(sf::Vector2f displacement) 
{
	// Create a displacement vector that maps the move logic from where the last 
	// position of a vertex was and the new offset position with the mouse clicked
	for (int i = 0; i < shape.getVertexCount(); i++) 
	{
		shape[i].position += displacement;
	}

	collider.moveCollider(displacement);
}

void GameObject::changeSizeCollider(float amount, bool inwards, sf::RenderWindow& window)
{
	collider.resize(amount, inwards, window);
}

Collider& GameObject::getCollider() 
{
	return collider;
}