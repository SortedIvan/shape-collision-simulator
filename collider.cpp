#include "collider.hpp"
#include <vector>
#include <iostream>
#include "math_utility.hpp"


Collider::Collider() 
{
    debugEnabled = true;
    colliderDebugColor = sf::Color::Yellow;
}

Collider::Collider(sf::VertexArray shape) 
{
    debugEnabled = true;
    colliderBounds = shape; 

    // clear the color of the shape
    for (int i = 0; i < colliderBounds.getVertexCount(); i++)
    {
        colliderBounds[i].color = sf::Color::Transparent;
    }

    colliderDebugColor = sf::Color::Yellow;
}

void drawVectorDebug(sf::RenderWindow& window, sf::Vector2f start, sf::Vector2f end, sf::Color color)
{
    sf::VertexArray line(sf::Lines, 2);
    line[0].position = start;
    line[1].position = end;
    line[0].color = color;
    line[1].color = color;

    window.draw(line);
}

void Collider::resize(float amount, bool inwards, sf::RenderWindow& window)
{
    // TODO: Make this for even and odd number of vertices
    if (colliderBounds.getVertexCount() == 3) 
    {
        // dealing with a triangle
        // colliderBounds[0] == A, colliderBounds[1] == B, colliderBounds[2] == C

        sf::Vector2f edgeA = colliderBounds[2].position - colliderBounds[1].position;
        sf::Vector2f edgeB = colliderBounds[2].position - colliderBounds[0].position;
        sf::Vector2f edgeC = colliderBounds[1].position - colliderBounds[0].position;

        float area = math->heronTriangleArea(math->vectorLength(edgeA), math->vectorLength(edgeB), math->vectorLength(edgeC));

        std::cout << area << std::endl;

        if (area + 100.f >= 15000.f || area - 100.f <= 500.f) 
        {
            return;
        }


        // Get the perpendicular vectors
        edgeA = sf::Vector2f(-edgeA.y, edgeA.x);
        edgeB = sf::Vector2f(edgeB.y, -edgeB.x);
        edgeC = sf::Vector2f(-edgeC.y, edgeC.x);

        // normalize
        math->normalize(edgeA);
        math->normalize(edgeB);
        math->normalize(edgeC);

        // scale with amount scalar
        edgeA = sf::Vector2f(edgeA.x * amount, edgeA.y * amount);
        edgeB = sf::Vector2f(edgeB.x * amount, edgeB.y * amount);
        edgeC = sf::Vector2f(edgeC.x * amount, edgeC.y * amount);

        // apply to correct vertices

        if (inwards) 
        {
            // collapse the triangle
            colliderBounds[0].position -= edgeA;
            colliderBounds[1].position -= edgeB;
            colliderBounds[2].position -= edgeC;
        }
        else 
        {
            // expand
            colliderBounds[0].position += edgeA;
            colliderBounds[1].position += edgeB;
            colliderBounds[2].position += edgeC;
        }
    }
}

void Collider::drawDebug(sf::RenderWindow& window) 
{
    if (!debugEnabled) 
    {
        return;
    }

    std::vector<sf::VertexArray> debugBounds;

    // for a shape with 3 vertices
    // bounds[0] -> bounds[1]
    // bounds[1] -> bounds[2]
    // bounds[2] -> bounds[0]
    // so we always end up looping back to the first

    for (int i = 0; i < colliderBounds.getVertexCount(); i++) 
    {
        sf::VertexArray line(sf::Lines, 2);

        line[0].color = colliderDebugColor;
        line[1].color = colliderDebugColor;

        if (i + 1 == colliderBounds.getVertexCount()) 
        {
            line[0].position = colliderBounds[i].position;
            line[1].position = colliderBounds[0].position;

            debugBounds.push_back(line);
            
            continue;
        }

        line[0].position = colliderBounds[i].position;
        line[1].position = colliderBounds[i+1].position;

        debugBounds.push_back(line);
    }

    for (int i = 0; i < debugBounds.size(); i++) 
    {
        window.draw(debugBounds[i]);
    }
}

void Collider::moveCollider(sf::Vector2f displacement) 
{
    for (int i = 0; i < colliderBounds.getVertexCount(); i++) 
    {
        colliderBounds[i].position += displacement;
    }
}

sf::VertexArray& Collider::getColliderBounds() 
{
    return colliderBounds;
}