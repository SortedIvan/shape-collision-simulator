
#include <iostream>
#include "SFML/Graphics.hpp"
#include <vector>
#include "math_utility.hpp"


void initializeTriangles(std::vector<sf::VertexArray>& vertexVector, int numberOfTriangles);
int checkCollisionWithTriangles(std::vector<sf::VertexArray>& triangles, sf::Vector2f mouseClickPoint, MathUtility& mathUtil);
void moveSelectedTriangleWithCenter(MathUtility& mathUtil, std::vector<sf::VertexArray>& vertexVector, int selectedTriangle, sf::Vector2f mouseMovedTo);
void moveSelectedTriangleWithPoint(MathUtility& mathUtil, std::vector<sf::VertexArray>& vertexVector, int selectedTriangle, sf::Vector2f mouseMovedTo, sf::Vector2f& pointHeld);
void rotateSelectedTriangle(MathUtility& math, std::vector<sf::VertexArray>& triangles, int selectedTriangle, float degrees, sf::Vector2f pointToRotateAround);
void applyRotationToTriangle(MathUtility& math, std::vector<sf::VertexArray>& triangles, int selectedTriangle, float degrees, sf::Vector2f pointToRotateAround);

int main()
{
    sf::RenderWindow window(
        sf::VideoMode(1000, 800),
        "Shapes");

    sf::Event e;

    MathUtility mathUtil;

    std::vector<sf::VertexArray> triangles;
    initializeTriangles(triangles, 3);

    int previouslySelectedTriangle = -1;
    bool triangleSelected = false;
    sf::Vector2f pointTriangleHeld;

    // Main loop
    while (window.isOpen())
    {
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
            {
                window.close();
            }
            
            if (e.type == sf::Event::KeyPressed) 
            {
                switch (e.key.code) 
                {
                    case (sf::Keyboard::R):
                        rotateSelectedTriangle(mathUtil, triangles, previouslySelectedTriangle, 0.06f, pointTriangleHeld);
                        break;
                }
            }

            if (e.type == sf::Event::MouseMoved) 
            {
                sf::Vector2f mouseMove = (sf::Vector2f)sf::Mouse::getPosition(window);

                if (triangleSelected)
                {
                    moveSelectedTriangleWithPoint(mathUtil, triangles, previouslySelectedTriangle, mouseMove, pointTriangleHeld);
                }
            }

            if (e.type == sf::Event::MouseButtonPressed) 
            {
                sf::Vector2f mouseClick = (sf::Vector2f)sf::Mouse::getPosition(window);

                if (!triangleSelected) 
                {
                    int triangleCollision = checkCollisionWithTriangles(triangles, mouseClick, mathUtil);

                    if (triangleCollision != -1)
                    {
                        if (previouslySelectedTriangle != -1)
                        {
                            triangles[previouslySelectedTriangle][0].color = sf::Color::White;
                        }

                        pointTriangleHeld = mouseClick;

                        triangles[triangleCollision][0].color = sf::Color::Yellow;
                        previouslySelectedTriangle = triangleCollision;
                        triangleSelected = true;
                    }
                }
            }

            if (e.type == sf::Event::MouseButtonReleased) 
            {
                if (triangleSelected) 
                {
                    if (previouslySelectedTriangle != -1) 
                    {
                        triangles[previouslySelectedTriangle][0].color = sf::Color::White;
                        previouslySelectedTriangle = -1;
                        triangleSelected = false;
                    }
                }
            }

        }

        // Clear the window
        window.clear(sf::Color::Black);

        // draw
        for (int i = 0; i < triangles.size(); i++) 
        {
            window.draw(triangles[i]);
        }

        // display
        window.display();
    }

    return 0;
}

int checkCollisionWithTriangles(std::vector<sf::VertexArray>& triangles, sf::Vector2f mouseClickPoint, MathUtility& mathUtil) 
{
    for (int i = 0; i < triangles.size(); i++) 
    {
        std::array<sf::Vector2f, 3> arr{triangles[i][0].position, triangles[i][1].position, triangles[i][2].position};

        if (mathUtil.pointInTriangleTest(arr, mouseClickPoint, 0.01))
        {
            return i;
        }
    }

    return -1;
}

// Applies instant rotation to a triangle
void rotateSelectedTriangle(MathUtility& math, std::vector<sf::VertexArray>& triangles, int selectedTriangle, float degrees, sf::Vector2f pointToRotateAround) 
{
    if (selectedTriangle == -1) 
    {
        return;
    }

    math.rotateVector(triangles[selectedTriangle][0].position, degrees, pointToRotateAround);
    math.rotateVector(triangles[selectedTriangle][1].position, degrees, pointToRotateAround);
    math.rotateVector(triangles[selectedTriangle][2].position, degrees, pointToRotateAround);
}

// Applies a gradual rotation to a triangle
void applyRotationToTriangle(MathUtility& math, std::vector<sf::VertexArray>& triangles, int selectedTriangle, float degrees, sf::Vector2f pointToRotateAround)
{
    if (selectedTriangle == -1)
    {
        return;
    }

    triangles[selectedTriangle][0].position += math.getRotatedVector(triangles[selectedTriangle][0].position, degrees, pointToRotateAround);
    triangles[selectedTriangle][1].position += math.getRotatedVector(triangles[selectedTriangle][1].position, degrees, pointToRotateAround);
    triangles[selectedTriangle][2].position += math.getRotatedVector(triangles[selectedTriangle][2].position, degrees, pointToRotateAround);
}

void moveSelectedTriangleWithPoint(MathUtility& mathUtil, std::vector<sf::VertexArray>& vertexVector, int selectedTriangle, sf::Vector2f mouseMovedTo, sf::Vector2f& pointHeld)
{
    if (selectedTriangle == -1)
    {
        return;
    }

    std::array<sf::Vector2f, 3> triangleCoordinates =
    {
        vertexVector[selectedTriangle][0].position,
        vertexVector[selectedTriangle][1].position,
        vertexVector[selectedTriangle][2].position
    };

    sf::Vector2f displacementVector = mouseMovedTo - pointHeld;

    pointHeld = pointHeld + displacementVector;

    // Now, add the displacement vector to all vertices of the triangle
    vertexVector[selectedTriangle][0].position += displacementVector;
    vertexVector[selectedTriangle][1].position += displacementVector;
    vertexVector[selectedTriangle][2].position += displacementVector;
}

void moveSelectedTriangleWithCenter(MathUtility& mathUtil,std::vector<sf::VertexArray>& vertexVector, int selectedTriangle, sf::Vector2f mouseMovedTo)
{
    if (selectedTriangle == -1) 
    {
        return;
    }
    
    std::array<sf::Vector2f, 3> triangleCoordinates = 
    { 
        vertexVector[selectedTriangle][0].position,
        vertexVector[selectedTriangle][1].position,
        vertexVector[selectedTriangle][2].position 
    };

    sf::Vector2f triangleCentroid = mathUtil.caclulateTriangleCentroid(triangleCoordinates);
        
    sf::Vector2f displacementVector = mouseMovedTo - triangleCentroid;

    // Now, add the displacement vector to all vertices of the triangle
    vertexVector[selectedTriangle][0].position += displacementVector;
    vertexVector[selectedTriangle][1].position += displacementVector;
    vertexVector[selectedTriangle][2].position += displacementVector;
}

void initializeTriangles(std::vector<sf::VertexArray>& vertexVector, int numberOfTriangles) 
{
    sf::Vector2f shiftPoint(200, 400);

    float triangleMainSideLength = 100.f;

    for (int i = 0; i < numberOfTriangles; i++) 
    {
        sf::VertexArray triangle(sf::Triangles, 3);

        triangle[0].position = sf::Vector2f(shiftPoint.x + (i * 200.f), shiftPoint.y);
        triangle[1].position = sf::Vector2f(shiftPoint.x + (i * 200.f) + triangleMainSideLength, shiftPoint.y);
        triangle[2].position = sf::Vector2f(shiftPoint.x + (i * 200.f) + (triangleMainSideLength / 2), shiftPoint.y + 75.f);

        triangle[0].color = sf::Color::Blue;
        triangle[1].color = sf::Color::Blue;
        triangle[2].color = sf::Color::Blue;

        vertexVector.push_back(triangle);
    }
}