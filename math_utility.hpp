
#include "SFML/Graphics.hpp"
#include <array>


class MathUtility 
{
	public:
		MathUtility();
		bool pointInTriangleTest(std::array<sf::Vector2f, 3>& points, sf::Vector2f clicked, float marginOfError);
		float vectorLength(sf::Vector2f vector);
		float heronTriangleArea(float sideA, float sideB, float sideC);
		sf::Vector2f caclulateTriangleCentroid(std::array<sf::Vector2f, 3>& points);
};