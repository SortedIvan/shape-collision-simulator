#include "math_utility.hpp"
#include <cmath>
#include <iostream>


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

