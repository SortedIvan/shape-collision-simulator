
#include "SFML/Graphics.hpp"

class MathUtility 
{
	public:
		MathUtility();
		bool pointInTriangleTest(sf::VertexArray& trianglePoints, sf::Vector2f point);
};