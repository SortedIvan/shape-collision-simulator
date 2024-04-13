#include "math_utility.hpp"
#include <cmath>
#include <iostream>

# define M_PI  3.14159

MathUtility::MathUtility()
{

}

bool MathUtility::pointInTriangleTest(std::array<sf::Vector2f, 3>& points, sf::Vector2f clicked, float marginOfError)
{
	// assume points[0] == A, points[1] == B, points[2] == C

	float AC_len = vectorLength(points[2] - points[0]);
	float AB_len = vectorLength(points[1] - points[0]);
	float BC_len = vectorLength(points[2] - points[1]);

	float AP_len = vectorLength(clicked - points[0]);
	float BP_len = vectorLength(clicked - points[1]);
	float CP_len = vectorLength(clicked - points[2]);

	float actualArea = heronTriangleArea(AC_len, AB_len, BC_len);

	float testArea1 = heronTriangleArea(AC_len, CP_len, AP_len);
	float testArea2 = heronTriangleArea(AB_len, AP_len, BP_len);
	float testArea3 = heronTriangleArea(BC_len, CP_len, BP_len);

	if (std::abs(actualArea - (testArea1 + testArea2 + testArea3)) <= marginOfError) 
	{
		return true;
	}

	return false;
}

float MathUtility::heronTriangleArea(float sideA, float sideB, float sideC)
{
	float semi = (sideA + sideB + sideC) / 2;
	
	return std::sqrt(
		std::abs(semi * (semi - sideA) * (semi - sideB) * (semi - sideC))
	);
}

float MathUtility::vectorLength(sf::Vector2f vector)
{
	return std::sqrt((vector.x * vector.x) + (vector.y * vector.y));
}

sf::Vector2f MathUtility::caclulateTriangleCentroid(std::array<sf::Vector2f, 3>& points) 
{
	if (points.size() != 3) 
	{
		return sf::Vector2f(0, 0);
	}

	return sf::Vector2f(
		(points[0].x + points[1].x + points[2].x) / 3,
		(points[0].y + points[1].y + points[2].y) / 3		
	);
}

void MathUtility::rotateVector(sf::Vector2f& vectorToRotate, float radians, sf::Vector2f pointToRotateAround)
{
	//float degrees = radians * (180 / M_PI);

	sf::Vector2f offsetPoint(vectorToRotate - pointToRotateAround);
	
	sf::Vector2f rotatedVector(
		offsetPoint.x * std::cos(radians) - offsetPoint.y * std::sin(radians),
		offsetPoint.x * std::sin(radians) + offsetPoint.y * std::cos(radians)
	);

	vectorToRotate = rotatedVector + pointToRotateAround;
}

sf::Vector2f MathUtility::getRotatedVector(sf::Vector2f& vectorToRotate, float radians, sf::Vector2f pointToRotateAround) 
{
	float degrees = radians * (180 / M_PI);

	sf::Vector2f offsetPoint(vectorToRotate - pointToRotateAround);

	return pointToRotateAround + sf::Vector2f(
		offsetPoint.x * std::cos(degrees) - offsetPoint.y * std::sin(degrees),
		offsetPoint.x * std::sin(degrees) + offsetPoint.y * std::cos(degrees)
	);
}

void MathUtility::normalize(sf::Vector2f& input) 
{
	float length = std::sqrt(input.x * input.x + input.y * input.y);

	if (!length) 
	{
		return;
	}
	
	input.x = input.x / length;
	input.y = input.y / length;
}

std::array<sf::Vector2f, 3> MathUtility::getTriangleEdgeNormals(std::array<sf::Vector2f, 3> edges)
{
	std::array<sf::Vector2f, 3> edgeNormals;

	for (int i = 0; i < 3; i++)
	{
		edgeNormals[i] = sf::Vector2f(-edges[i].y, edges[i].x);
	}

	return edgeNormals;
}


sf::Vector2f MathUtility::getVectorNormal(sf::Vector2f input) 
{
	return sf::Vector2f(-input.y, input.x);
}

float MathUtility::dot(sf::Vector2f a, sf::Vector2f b) 
{
	return (a.x * b.x + a.y * b.y);
}