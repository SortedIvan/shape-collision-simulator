
#include <iostream>
#include "SFML/Graphics.hpp"
#include <vector>
#include "math_utility.hpp"
#include <utility>
#include "gameobject.hpp"

void initializeTriangles(std::vector<sf::VertexArray>& vertexVector, int numberOfTriangles);
int checkCollisionWithTriangles(std::vector<sf::VertexArray>& triangles, sf::Vector2f mouseClickPoint, MathUtility& mathUtil);
void moveSelectedTriangleWithCenter(MathUtility& mathUtil, std::vector<sf::VertexArray>& vertexVector, int selectedTriangle, sf::Vector2f mouseMovedTo);
void moveSelectedTriangleWithPoint(MathUtility& mathUtil, std::vector<sf::VertexArray>& vertexVector, int selectedTriangle, sf::Vector2f mouseMovedTo, sf::Vector2f& pointHeld, bool& isColliding);
void rotateSelectedTriangle(MathUtility& math, std::vector<sf::VertexArray>& triangles, int selectedTriangle, float degrees, sf::Vector2f pointToRotateAround);
void applyRotationToTriangle(MathUtility& math, std::vector<sf::VertexArray>& triangles, int selectedTriangle, float degrees, sf::Vector2f pointToRotateAround);
void initializeRandomTriangles(std::vector<sf::VertexArray>& vertexVector, int numberOfTriangles);
sf::Vector2f projectVertices(sf::Vector2f axis, sf::VertexArray verticesA, MathUtility& math);
void drawVector(sf::RenderWindow& window, sf::Vector2f start, sf::Vector2f vector, sf::Color color);
bool checkTwoTrianglesColliding(sf::VertexArray triangleA, sf::VertexArray triangleB, MathUtility& math);
bool checkSelectedTriangleCollision(std::vector<sf::VertexArray>& triangles, MathUtility& math, int& selectedTriangle);
void initializeTriangleGameObjects(std::vector<GameObject>& triangles, int amount);


int main()
{
    sf::RenderWindow window(
        sf::VideoMode(1000, 800),
        "Shapes");

    sf::Event e;

    MathUtility mathUtil;

    std::vector<sf::VertexArray> triangles;
    //initializeTriangles(triangles, 2);
    //initializeRandomTriangles(triangles, 1000);

    std::vector<GameObject> triangleObjects;
    initializeTriangleGameObjects(triangleObjects, 2);


    int previouslySelectedTriangle = -1;
    bool triangleSelected = false;
    sf::Vector2f pointTriangleHeld;
    bool isColliding = false;


    // Main loop
    while (window.isOpen())
    {
        if (triangleSelected) 
        {
            isColliding = checkSelectedTriangleCollision(triangles, mathUtil, previouslySelectedTriangle);
        }
        

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
                    if (!isColliding)
                    {
                        moveSelectedTriangleWithPoint(mathUtil, triangles, previouslySelectedTriangle, mouseMove, pointTriangleHeld, isColliding);
                    }
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

        for (int i = 0; i < triangleObjects.size(); i++) 
        {
            triangleObjects[i].draw(window);
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

void moveSelectedTriangleWithPoint(MathUtility& mathUtil, std::vector<sf::VertexArray>& vertexVector, int selectedTriangle, sf::Vector2f mouseMovedTo, sf::Vector2f& pointHeld, bool& isColliding)
{

    if (selectedTriangle == -1)
    {
        return;
    }

    sf::Vector2f displacementVector = mouseMovedTo - pointHeld;

    // Before any changes are made, check if this move will collide and get the object stuck


    sf::VertexArray newTriangleCoordinates(sf::PrimitiveType::Triangles, 3);
    
    std::vector<sf::VertexArray> trianglesAfterMove = vertexVector;

    newTriangleCoordinates[0].position = vertexVector[selectedTriangle][0].position + displacementVector;
    newTriangleCoordinates[1].position = vertexVector[selectedTriangle][1].position + displacementVector;
    newTriangleCoordinates[2].position = vertexVector[selectedTriangle][2].position + displacementVector;

    trianglesAfterMove[selectedTriangle] = newTriangleCoordinates;

    if (checkSelectedTriangleCollision(trianglesAfterMove, mathUtil, selectedTriangle))
    {
        isColliding = true;
        return;
    }

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

void initializeRandomTriangles(std::vector<sf::VertexArray>& vertexVector, int numberOfTriangles)
{
    // Fixed size of 1000
    srand(static_cast<unsigned int>(time(nullptr)));

    sf::Vector2u screenSize = sf::Vector2u(1000, 800); 

    for (int i = 0; i < numberOfTriangles; i++)
    {
        // Create a new triangle vertex array
        sf::VertexArray triangle(sf::Triangles, 3);

        float randX = static_cast<float>(rand() % (screenSize.x - 10)); // Generate a random x-coordinate
        float randY = static_cast<float>(rand() % (screenSize.y - 10)); // Generate a random y-coordinate
        triangle[0].position = sf::Vector2f(randX, randY);

        // Calculate positions for the second and third vertices to ensure a side length of 10
        float sideLength = 10.0f;
        float offsetX = sideLength / 2.0f;
        float offsetY = sideLength * sqrt(3.0f) / 2.0f;

        triangle[1].position = sf::Vector2f(randX + sideLength, randY);
        triangle[2].position = sf::Vector2f(randX + offsetX, randY + offsetY);

        triangle[0].color = sf::Color::Blue;
        triangle[1].color = sf::Color::Blue;
        triangle[2].color = sf::Color::Blue;
        
        vertexVector.push_back(triangle);
    }
}

// This function projects the vertices and gets the min and max from them
// Since we are only looking for min and max, we don't need the full projection
// we only get the dot product measure (the scalar) of the resulting vector
sf::Vector2f projectVertices(sf::Vector2f axis, sf::VertexArray vertices, MathUtility& math) 
{
    float max = std::numeric_limits<float>::lowest();
    float min = std::numeric_limits<float>::max();

    for (int i = 0; i < vertices.getVertexCount(); i++)
    {
        // project and find the min and max
        float vertexProjected = math.dot(vertices[i].position, axis);

        if (vertexProjected > max)
        {
            max = vertexProjected;
        }

        if (vertexProjected < min)
        {
            min = vertexProjected;
        }
    }

    return sf::Vector2f(min, max);
}


bool checkSelectedTriangleCollision(std::vector<sf::VertexArray>& triangles, MathUtility& math, int& selectedTriangle) 
{
    for (int i = 0; i < triangles.size(); i++) 
    {
        if (i == selectedTriangle) 
        {
            continue;
        }

        if (checkTwoTrianglesColliding(triangles[selectedTriangle], triangles[i], math)) 
        {
            return true;
        }
    }

    return false;
}

bool checkTwoTrianglesColliding(sf::VertexArray triangleA, sf::VertexArray triangleB, MathUtility& math) 
{
    std::array<sf::Vector2f, 3> triangleA_edges =
    {
        triangleA[1].position - triangleA[0].position,
        triangleA[2].position - triangleA[1].position,
        triangleA[2].position - triangleA[0].position
    };

    std::array<sf::Vector2f, 3> edgeNormals_A = math.getTriangleEdgeNormals(triangleA_edges);

    for (int i = 0; i < edgeNormals_A.size(); i++)
    {
        sf::Vector2f minMax_A = projectVertices(edgeNormals_A[i], triangleA, math);
        sf::Vector2f minMax_B = projectVertices(edgeNormals_A[i], triangleB, math);

        if (minMax_A.x >= minMax_B.y || minMax_B.x >= minMax_A.y) 
        {
            return false;
        }

    }

    std::array<sf::Vector2f, 3> triangleB_edges =
    {
        triangleB[1].position - triangleB[0].position,
        triangleB[2].position - triangleB[1].position,
        triangleB[2].position - triangleB[0].position
    };

    std::array<sf::Vector2f, 3> edgeNormals_B = math.getTriangleEdgeNormals(triangleB_edges);

    for (int i = 0; i < edgeNormals_B.size(); i++)
    {
        sf::Vector2f minMax_A = projectVertices(edgeNormals_B[i], triangleA, math);
        sf::Vector2f minMax_B = projectVertices(edgeNormals_B[i], triangleB, math);

        if (minMax_A.x >= minMax_B.y || minMax_B.x >= minMax_A.y)
        {
            return false;
        }

    }

    return true;

}

// Function to draw a vector as a line segment
void drawVector(sf::RenderWindow& window, sf::Vector2f start, sf::Vector2f end, sf::Color color) 
{
    sf::VertexArray line(sf::Lines, 2);
    line[0].position = start;
    line[1].position = end;
    line[0].color = color;
    line[1].color = color;

    window.draw(line);
}

void initializeTriangleGameObjects(std::vector<GameObject>& triangles, int amount) 
{
    sf::Vector2f shiftPoint(200, 400);

    float triangleMainSideLength = 100.f;

    std::vector<sf::VertexArray> trianglesObjs;

    for (int i = 0; i < amount; i++)
    {
        sf::VertexArray triangle(sf::Triangles, 3);

        triangle[0].position = sf::Vector2f(shiftPoint.x + (i * 200.f), shiftPoint.y);
        triangle[1].position = sf::Vector2f(shiftPoint.x + (i * 200.f) + triangleMainSideLength, shiftPoint.y);
        triangle[2].position = sf::Vector2f(shiftPoint.x + (i * 200.f) + (triangleMainSideLength / 2), shiftPoint.y + 75.f);

        triangle[0].color = sf::Color::Blue;
        triangle[1].color = sf::Color::Blue;
        triangle[2].color = sf::Color::Blue;

        trianglesObjs.push_back(triangle);
    }

    for (int i = 0; i < trianglesObjs.size(); i++)
    {
        GameObject triangle(trianglesObjs[i]);
        triangles.push_back(triangle);
    }
}