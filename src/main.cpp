#include <SFML/Graphics.hpp>    // Include the SFML header files
#include <SFML/Window.hpp>
#include "solitaire_game.hpp"   // Include the header file

int main()
{
    static SolitaireGame game;
    game.print();
    std::vector<sf::Sprite> sprites;
    unsigned int i = 0;
    for (auto& card : game.get_clickable_cards()) {
        sf::Sprite sprite(card.texture);
        sprite.setPosition({32.0f * i, 48.0f});
        i++;
    }

    auto back_texture = sf::Texture("src/Sprites/CardBackBlack.png");
    sf::Sprite card_back(back_texture);
    auto window = sf::RenderWindow(sf::VideoMode({800u, 600u}), "Solitaire",
        sf::Style::Close);
    window.setFramerateLimit(144);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            const float mouse_x = static_cast<float>(sf::Mouse::getPosition(window).x);
            const float mouse_y = static_cast<float>(sf::Mouse::getPosition(window).y);
            const sf::Vector2f mouse_position = {mouse_x, mouse_y}, offset = {32, 48};
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                if (card_back.getGlobalBounds().contains(mouse_position)) {
                    card_back.setPosition(mouse_position-offset);
                }
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                for (auto& sprite : sprites) {
                    if (sprite.getGlobalBounds().contains(mouse_position)) {
                        sprite.setPosition(mouse_position-offset);
                    }
                }
            }
        }
        window.clear();
        window.draw(card_back);
        window.display();
    }
}
