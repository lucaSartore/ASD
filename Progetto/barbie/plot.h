#ifndef __PLOT_H__
#define __PLOT_H__

#include <SFML/Graphics.hpp>
#include <vector>

sf::Color getRandomColor() {
    // Generate random color values
    sf::Uint8 r = rand() % 256;
    sf::Uint8 g = rand() % 256;
    sf::Uint8 b = rand() % 256;

    return sf::Color(r, g, b);
}

void plot(const std::vector<std::vector<std::pair<int, int>>>& dataPoints) {
    // Create a window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Graph");

    // Main loop
    while (window.isOpen()) {
        // Process events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Clear the window
        window.clear();

        // Draw x-axis
        sf::VertexArray xAxis(sf::Lines);
        xAxis.append(sf::Vertex(sf::Vector2f(0.f, 300.f), sf::Color::White));
        xAxis.append(sf::Vertex(sf::Vector2f(800.f, 300.f), sf::Color::White));
        window.draw(xAxis);

        // Draw y-axis
        sf::VertexArray yAxis(sf::Lines);
        yAxis.append(sf::Vertex(sf::Vector2f(400.f, 0.f), sf::Color::White));
        yAxis.append(sf::Vertex(sf::Vector2f(400.f, 600.f), sf::Color::White));
        window.draw(yAxis);

        // Draw the lines
        for (const auto& line : dataPoints) {
            sf::VertexArray array(sf::LinesStrip);
            for (const auto& point : line) {
                array.append(sf::Vertex(sf::Vector2f(static_cast<float>(point.first), static_cast<float>(point.second)), getRandomColor()));
            }
            window.draw(array);
        }

        // Display the window
        window.display();
    }
}

#endif
