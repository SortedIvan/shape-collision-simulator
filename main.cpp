
#include <iostream>
#include "SFML/Graphics.hpp"
#include <vector>
#include "math_utility.hpp"


void initializeTriangles(std::vector<sf::VertexArray>& vertexVector, int numberOfTriangles);

int main()
{
    sf::RenderWindow window(
        sf::VideoMode(1000, 800),
        "Unimail");

    sf::Event e;

    MathUtility mathUtil;

    sf::VertexArray triangle1(sf::Triangles, 3);
    sf::VertexArray triangle2(sf::Triangles, 3);
    sf::VertexArray triangle3(sf::Triangles, 3);

    std::vector<sf::VertexArray> triangles;
    initializeTriangles(triangles, 3);

    // Main loop
    while (window.isOpen())
    {
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
            {
                window.close();
            }
            
            if (e.type == sf::Event::MouseButtonReleased) 
            {
                std::array<sf::Vector2f, 3> arr{triangles[0][0].position, triangles[0][1].position, triangles[0][2].position};

                if (mathUtil.pointInTriangleTest(arr, (sf::Vector2f)sf::Mouse::getPosition(window), 0.01)) 
                {
                    std::cout << "niggerlicious";
                }
                    
            }

        }

        // Clear the window
        window.clear(sf::Color::Black);

        for (int i = 0; i < triangles.size(); i++) 
        {
            window.draw(triangles[i]);
        }

        window.display();
    }

    return 0;
}

void checkCollisionWithTriangles(std::vector<sf::VertexArray>& triangles, sf::Vector2i mouseClickPoint) 
{
    for (int i = 0; i < triangles.size(); i++) 
    {
        sf::Vector2f leftmostPoint = triangles[i][0].position;
        sf::Vector2f rightmostPoint = triangles[i][1].position;
        sf::Vector2f midPoint = triangles[i][2].position;



    }
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
        triangle[1].color = sf::Color::Yellow;
        triangle[2].color = sf::Color::Blue;

        vertexVector.push_back(triangle);
    }
}