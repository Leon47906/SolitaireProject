#include <SFML/Graphics.hpp>    // Include the SFML header files
#include <SFML/Window.hpp>
#include "solitaire_game.hpp"   // Include the header file

constexpr sf::Vector2f offset = {32.0f, 48.0f};

int main()
{
    SolitaireGame game;
    game.print();
    std::vector<GenericDeck>& tableau = game.tableau;
    std::vector<CardWithTexture*> rendered_cards, movable_cards;
    const auto backTexture(sf::Texture("src/Sprites/CardBackRed.png"));
    for (size_t i = 0; i < tableau.size(); i++) {
        for (size_t j = 0; j < tableau[i].size(); j++) {
            CardWithTexture& card = *tableau[i][j];
            card.setPosition({i*(CARD_WIDTH+TABLEAU_HORIZONTAL_SPACING), j*(CARD_HEIGHT+TABLEAU_VERTICAL_SPACING)/2.5f});
            rendered_cards.push_back(&card);
            if (j == tableau[i].size()-1) {movable_cards.push_back(&card);}
        }
    }

    auto window = sf::RenderWindow(sf::VideoMode({800u, 600u}), "Solitaire",
        sf::Style::Close);
    window.setFramerateLimit(144);
    CardWithTexture* dragging_card_ptr = nullptr;
    while (window.isOpen())
    {
        const float mouse_x = static_cast<float>(sf::Mouse::getPosition(window).x);
        const float mouse_y = static_cast<float>(sf::Mouse::getPosition(window).y);
        const sf::Vector2f mouse_position = {mouse_x, mouse_y};
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (const auto* mouseButtonPresed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButtonPresed->button == sf::Mouse::Button::Left) {
                    if (dragging_card_ptr != nullptr) {
                        CardWithTexture& card = *dragging_card_ptr;
                        card.position = mouse_position - offset;
                        movable_cards.push_back(dragging_card_ptr);
                        dragging_card_ptr = nullptr;
                    }
                    else {
                        for (auto& card_ptr : movable_cards) {
                            if (dragging_card_ptr == nullptr) {
                                if (card_ptr->createSprite().getGlobalBounds().contains(mouse_position)) {
                                    dragging_card_ptr = card_ptr;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>())
            {
                if (dragging_card_ptr != nullptr) {
                    dragging_card_ptr->position = mouse_position - offset;
                }
            }

        }
        window.clear();
        for (const auto card_ptr : rendered_cards) {
            if (const CardWithTexture& card = *card_ptr; card.face_up) {
                window.draw(card.createSprite());
            } else {
                sf::Sprite backSprite(backTexture);
                backSprite.setPosition(card.position);
                window.draw(backSprite);
            }
        }
        window.display();
    }
}
