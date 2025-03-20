#include <SFML/Graphics.hpp>    // Include the SFML header files
#include <SFML/Window.hpp>
#include "solitaire_game.hpp"   // Include the header file

constexpr sf::Vector2f offset = {32.0f, 48.0f};

int main()
{
    SolitaireGame game;
    game.print();
    Deck& deck = game.deck;
    GenericDeck& waste = game.waste;
    std::vector<GenericDeck>& foundation = game.foundation;
    sf::Vector2f deck_position = {7*(CARD_WIDTH+TABLEAU_HORIZONTAL_SPACING), 0},
        waste_position = {8*(CARD_WIDTH+TABLEAU_HORIZONTAL_SPACING), 0};
    std::vector<sf::Vector2f> foundation_positions = {
        {9*(CARD_WIDTH+TABLEAU_HORIZONTAL_SPACING), 0},
        {9*(CARD_WIDTH+TABLEAU_HORIZONTAL_SPACING), CARD_HEIGHT+TABLEAU_VERTICAL_SPACING},
        {9*(CARD_WIDTH+TABLEAU_HORIZONTAL_SPACING), 2*(CARD_HEIGHT+TABLEAU_VERTICAL_SPACING)},
        {9*(CARD_WIDTH+TABLEAU_HORIZONTAL_SPACING), 3*(CARD_HEIGHT+TABLEAU_VERTICAL_SPACING)}};
    for (auto card_ptr : deck.cards) {
        CardWithTexture& card = *card_ptr;
        card.setPosition(deck_position);
    }
    std::vector<GenericDeck>& tableau = game.tableau;
    std::vector<CardWithTexture*> movable_cards, rendered_cards = game.rendered_cards;
    const auto backTexture(sf::Texture("src/Sprites/CardBackRed.png"));
    for (size_t i = 0; i < tableau.size(); i++) {
        for (size_t j = 0; j < tableau[i].size(); j++) {
            CardWithTexture& card = *tableau[i][j];
            card.setPosition({i*(CARD_WIDTH+TABLEAU_HORIZONTAL_SPACING), j*(TABLEAU_VERTICAL_SPACING)});
            if (j == tableau[i].size()-1) {movable_cards.push_back(&card);}
        }
    }
    auto window = sf::RenderWindow(sf::VideoMode({800u, 600u}), "Solitaire",
        sf::Style::Close);
    window.setFramerateLimit(144);
    CardWithTexture* dragging_card_ptr = nullptr;
    sf::Vector2f initial_position;
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
                if (mouseButtonPresed->button == sf::Mouse::Button::Right && dragging_card_ptr != nullptr) {
                    dragging_card_ptr->position = initial_position;
                    dragging_card_ptr = nullptr;
                }
                if (mouseButtonPresed->button == sf::Mouse::Button::Left) {
                    //release card
                    if (dragging_card_ptr != nullptr) {
                        const int x_index = mouse_x / (CARD_WIDTH + TABLEAU_HORIZONTAL_SPACING);
                        if (x_index == 8) {
                            if (dragging_card_ptr == deck.cards.back()) {
                                deck.draw_from_top();
                                CardWithTexture* card_ptr = deck[deck.size()-1];
                                card_ptr->flip();
                                rendered_cards.push_back(card_ptr);
                                movable_cards.push_back(card_ptr);
                                waste.add_to_top(dragging_card_ptr);
                                dragging_card_ptr->position = waste_position;
                                game.print();
                                for (int i = 0; i < movable_cards.size(); i++) {
                                    if (movable_cards[i] == dragging_card_ptr) {
                                        movable_cards.erase(movable_cards.begin() + i);
                                        break;
                                    }
                                }
                                dragging_card_ptr = nullptr;
                            }

                        }
                        else {
                            for (size_t i = 0; i < tableau.size(); i++) {
                                if (i == x_index) {
                                    tableau[i].add_to_top(dragging_card_ptr);
                                    break;
                                }
                            }
                        }
                    }
                    else {
                        if (deck.cards.back()->createSprite().getGlobalBounds().contains(mouse_position)) {

                        }
                        for (auto card_ptr : movable_cards) {
                            if (dragging_card_ptr == nullptr) {
                                if (card_ptr->createSprite().getGlobalBounds().contains(mouse_position)) {
                                    initial_position = card_ptr->position;
                                    if (card_ptr->value == 1) {
                                        const char& suit = card_ptr->suit;
                                        switch (suit) {
                                            case 'H':
                                                game.move_card(card_ptr, foundation[0]);
                                                card_ptr->position = foundation_positions[0];
                                            case 'D':
                                                game.move_card(card_ptr, foundation[1]);
                                                card_ptr->position = foundation_positions[1];
                                            case 'S':
                                                game.move_card(card_ptr, foundation[2]);
                                                card_ptr->position = foundation_positions[2];
                                            case 'C':
                                                game.move_card(card_ptr, foundation[3]);
                                                card_ptr->position = foundation_positions[3];
                                            default:
                                                break;
                                        }
                                        for (int i = 0; i < movable_cards.size(); i++) {
                                            if (movable_cards[i] == card_ptr) {
                                                movable_cards.erase(movable_cards.begin() + i);
                                                break;
                                            }
                                        }
                                    }
                                    else {
                                        dragging_card_ptr = card_ptr;
                                        break;
                                    }
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
        if (deck.size() > 1) {
            sf::Sprite backSprite(backTexture);
            backSprite.setPosition(deck_position);
            window.draw(backSprite);
        }
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
