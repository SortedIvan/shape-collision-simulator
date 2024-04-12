
#include <iostream>
#include "SFML/Graphics.hpp"
#include <vector>
#include "math_utility.hpp"
#include <utility>

void initializeTriangles(std::vector<sf::VertexArray>& vertexVector, int numberOfTriangles);
int checkCollisionWithTriangles(std::vector<sf::VertexArray>& triangles, sf::Vector2f mouseClickPoint, MathUtility& mathUtil);
void moveSelectedTriangleWithCenter(MathUtility& mathUtil, std::vector<sf::VertexArray>& vertexVector, int selectedTriangle, sf::Vector2f mouseMovedTo);
void moveSelectedTriangleWithPoint(MathUtility& mathUtil, std::vector<sf::VertexArray>& vertexVector, int selectedTriangle, sf::Vector2f mouseMovedTo, sf::Vector2f& pointHeld);
void rotateSelectedTriangle(MathUtility& math, std::vector<sf::VertexArray>& triangles, int selectedTriangle, float degrees, sf::Vector2f pointToRotateAround);
void applyRotationToTriangle(MathUtility& math, std::vector<sf::VertexArray>& triangles, int selectedTriangle, float degrees, sf::Vector2f pointToRotateAround);
void initializeRandomTriangles(std::vector<sf::VertexArray>& vertexVector, int numberOfTriangles);
sf::Vector2f projectVertices(sf::Vector2f axis, sf::VertexArray verticesA, MathUtility& math);
bool applyCollisionToSelectedTriangle(std::vector<sf::VertexArray>& triangles, int selectedTriangle, MathUtility& math);


int main()
{
    sf::RenderWindow window(
        sf::VideoMode(1000, 800),
        "Shapes");

    sf::Event e;

    MathUtility mathUtil;

    std::vector<sf::VertexArray> triangles;
    initializeTriangles(triangles, 3);

    //initializeRandomTriangles(triangles, 100000);

    int previouslySelectedTriangle = -1;
    bool triangleSelected = false;
    sf::Vector2f pointTriangleHeld;
    bool collisionTest = false;


    // Main loop
    while (window.isOpen())
    {
        while (window.pollEvent(e))
        {
            if (triangleSelected)
            {
                collisionTest = applyCollisionToSelectedTriangle(triangles, previouslySelectedTriangle, mathUtil);
            }
            else 
            {
                collisionTest = false;
            }



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
                    if (!collisionTest) 
                    {
                        moveSelectedTriangleWithPoint(mathUtil, triangles, previouslySelectedTriangle, mouseMove, pointTriangleHeld);
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
sf::Vector2f projectVertices(sf::Vector2f axis, sf::VertexArray verticesA, MathUtility& math) 
{

    float max = std::numeric_limits<float>::lowest();
    float min = std::numeric_limits<float>::max();

    for (int i = 0; i < verticesA.getVertexCount(); i++) 
    {
        // project and find the min and max
        float scalarProjection = math.dot(verticesA[i].position, axis);

        if (scalarProjection > max) 
        {
            max = scalarProjection;
            continue;
        }

        if (scalarProjection < min) 
        {
            min = scalarProjection;
            continue;
        }
    }

    return sf::Vector2f(min, max);
}

bool applyCollisionToSelectedTriangle(std::vector<sf::VertexArray>& triangles, int selectedTriangle, MathUtility& math) 
{
    bool colliding = true;

    std::array<sf::Vector2f, 3> selectedTriangleEdges =
    {
        triangles[selectedTriangle][1].position - triangles[selectedTriangle][0].position,
        triangles[selectedTriangle][2].position - triangles[selectedTriangle][1].position,
        triangles[selectedTriangle][2].position - triangles[selectedTriangle][0].position
    };

    std::array<sf::Vector2f, 3> selectedTriangleNormals = math.getTriangleEdgeNormals(selectedTriangleEdges);

    for (int i = 0; i < triangles.size(); i++) 
    {
        if (i == selectedTriangle) 
        {
            continue;
        }

        // triangles[0][0] == A, triangles[0][1] == B, triangles[0][2] == C 
        std::array<sf::Vector2f, 3> triangleEdgesToCheck =
        {
            triangles[i][1].position - triangles[i][0].position,
            triangles[i][2].position - triangles[i][1].position,
            triangles[i][2].position - triangles[i][0].position
        };


        std::array<sf::Vector2f, 3> edgeNormalsToCheck = math.getTriangleEdgeNormals(triangleEdgesToCheck);

        // start looping all normals and checking all potential overlaps
        for (int k = 0; k < edgeNormalsToCheck.size(); k++) 
        {
            sf::Vector2f triangleMinMax_A = projectVertices(edgeNormalsToCheck[k], triangles[selectedTriangle], math);
            sf::Vector2f triangleMinMax_B = projectVertices(edgeNormalsToCheck[k], triangles[i], math);

            // since we are looking for a seperating axis, we look whether there is a minimum that is greater than a maximum

            if (triangleMinMax_A.y <= triangleMinMax_B.x
                || triangleMinMax_B.y <= triangleMinMax_A.x) 
            {
                // seperating axis found

                colliding = false; 
                return colliding;
            }
        }

        for (int k = 0; k < selectedTriangleNormals.size(); k++)
        {
            sf::Vector2f triangleMinMax_A = projectVertices(selectedTriangleNormals[k], triangles[selectedTriangle], math);
            sf::Vector2f triangleMinMax_B = projectVertices(selectedTriangleNormals[k], triangles[i], math);

            // since we are looking for a seperating axis, we look whether there is a minimum that is greater than a maximum

            if (triangleMinMax_A.y <= triangleMinMax_B.x
                || triangleMinMax_B.y <= triangleMinMax_A.x)
            {
                // seperating axis found

                colliding = false;
                return colliding;
            }
        }

    }

    return colliding;
}