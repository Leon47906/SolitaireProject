#include <SFML/Graphics.hpp>    // Include the SFML header files
#include "solitaire_game.hpp"   // Include the header file

int main()
{
    SolitaireGame game;
    game.print();
    const sf::Texture texture1("src/sprites/1C.png");
    sf::Sprite sprite(texture1);
    sprite.setPosition({-10, -10});
    auto window = sf::RenderWindow(sf::VideoMode({800u, 600u}), "Solitaire",
        sf::Style::Close);
    window.setFramerateLimit(144);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            const float mouse_x = static_cast<float>(sf::Mouse::getPosition(window).x);
            const float mouse_y = static_cast<float>(sf::Mouse::getPosition(window).y);
            const sf::Vector2f mouse_position = {mouse_x, mouse_y};
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                sprite.setPosition(mouse_position);
            }
        }
        window.clear();
        window.draw(sprite);
        window.display();
    }
}
