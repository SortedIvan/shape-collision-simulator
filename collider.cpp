#include "collider.hpp"
#include <vector>
#include <iostream>

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

void Collider::resize() 
{

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
