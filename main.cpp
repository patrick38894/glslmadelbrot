
//
// Disclamer:
// ----------
//
// This code will work only if you selected window, graphics and audio.
//
// Note that the "Run Script" build phase will copy the required frameworks
// or dylibs to your application bundle so you can execute it on any OS X
// computer.
//
// Your resource files (images, sounds, fonts, ...) are also copied to your
// application bundle. To get the path to these resource, use the helper
// method resourcePath() from ResourcePath.hpp
//

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

// Here is a small helper for you ! Have a look.
#include "ResourcePath.hpp"

int main(int, char const**)
{
    // Create the main window
    sf::Vector2f screensize(2560,1440);
    sf::RenderWindow window(sf::VideoMode(screensize.x, screensize.y), "SFML window");

    sf::RectangleShape rectangle;
    rectangle.setSize(screensize);
    rectangle.setOutlineColor(sf::Color::Red);
    rectangle.setOutlineThickness(1);
    rectangle.setPosition(0, 0);
    
    std::cout << sf::Shader::isAvailable() << std::endl;
    sf::Shader myshader;

    if (!myshader.loadFromFile(resourcePath() + "vertexshader" , resourcePath() + "fragshader")) {
        std::cout << "dude the shader is broken or something" << std::endl;
    }

    
    myshader.setParameter("screen",screensize);
    sf::Vector2f zoom(1,1);
    myshader.setParameter("zoom",zoom);
    sf::Vector2f center(0,0);
    
    sf::Vector2i startpos(0,0);
    sf::Vector2f startcenter(0,0);
    
    
    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Espace pressed : exit
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
            if (event.type == sf::Event::MouseWheelMoved) {
                if (event.mouseWheel.delta < 0)
                    zoom = sf::Vector2f(zoom.x * 2.0, zoom.y * 2.0);
                else if (event.mouseWheel.delta > 0)
                    zoom = sf::Vector2f(zoom.x * 0.5, zoom.y * 0.5);
                myshader.setParameter("zoom",zoom);
            }
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left) {
                center -= sf::Vector2f(zoom.x, 0);
                myshader.setParameter("center",center);
            }
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right) {
                center += sf::Vector2f(zoom.x, 0);
                myshader.setParameter("center",center);
            }
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down) {
                center -= sf::Vector2f(0, zoom.y);
                myshader.setParameter("center",center);
            }
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up) {
                center += sf::Vector2f(0, zoom.y);
                myshader.setParameter("center",center);
            }
            
            
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                if (startpos == sf::Vector2i(0,0)) {
                    startpos = sf::Mouse::getPosition();
                    startcenter = center;
                }
                else {
                    sf::Vector2i temp = sf::Mouse::getPosition() - startpos;
                    center = startcenter - sf::Vector2f(temp.x * zoom.x / (screensize.x /4.), -temp.y * zoom.y /(screensize.x/4.));
                    myshader.setParameter("center", center);
                }
            }
            else {
                startpos = sf::Vector2i(0,0);
                startcenter = center;
            }
                
            
        }
        // Clear screen
        window.clear();
        
        
        // Draw the sprite
        window.draw(rectangle, &myshader);

        // Update the window
        window.display();
    }
    
    return EXIT_SUCCESS;
}
