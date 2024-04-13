
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
		void rotateVector(sf::Vector2f& vectorToRotate, float radians, sf::Vector2f pointToRotateAround);
		sf::Vector2f getRotatedVector(sf::Vector2f& vectorToRotate, float angle, sf::Vector2f pointToRotateAround);
		void normalize(sf::Vector2f& input);
		std::array<sf::Vector2f, 3> getTriangleEdgeNormals(std::array<sf::Vector2f, 3> triangle);
		sf::Vector2f getVectorNormal(sf::Vector2f input);
		float dot(sf::Vector2f a, sf::Vector2f b);
};