#include <SFML/Graphics.hpp>    // Include the SFML header files
#include <SFML/Window.hpp>
#include "solitaire_game.hpp"   // Include the header file

constexpr sf::Vector2f offset = {32.0f, 48.0f};

int main()
{
    sf::Vector2f deck_position = {7*(CARD_WIDTH+TABLEAU_HORIZONTAL_SPACING), 0},
        waste_position = {8*(CARD_WIDTH+TABLEAU_HORIZONTAL_SPACING), 0};
    sf::Rect deck_rect(deck_position, {CARD_WIDTH, CARD_HEIGHT});
    std::vector<sf::Vector2f> foundation_positions = {
        {9*(CARD_WIDTH+TABLEAU_HORIZONTAL_SPACING), 0},
        {9*(CARD_WIDTH+TABLEAU_HORIZONTAL_SPACING), CARD_HEIGHT+TABLEAU_VERTICAL_SPACING},
        {9*(CARD_WIDTH+TABLEAU_HORIZONTAL_SPACING), 2*(CARD_HEIGHT+TABLEAU_VERTICAL_SPACING)},
        {9*(CARD_WIDTH+TABLEAU_HORIZONTAL_SPACING), 3*(CARD_HEIGHT+TABLEAU_VERTICAL_SPACING)}},
    waste_positions = {
        {8*(CARD_WIDTH+TABLEAU_HORIZONTAL_SPACING), 0},
        {8*(CARD_WIDTH+TABLEAU_HORIZONTAL_SPACING), TABLEAU_VERTICAL_SPACING},
        {8*(CARD_WIDTH+TABLEAU_HORIZONTAL_SPACING), 2*TABLEAU_VERTICAL_SPACING}};
    SolitaireGame game(deck_position, waste_positions, foundation_positions);
    game.print();
    Deck& deck = game.deck;
    GenericDeck& waste = game.waste;
    std::vector<GenericDeck>& foundation = game.foundation;
    for (auto card_ptr : deck.cards) {
        CardWithTexture& card = *card_ptr;
        card.setPosition(deck_position);
    }
    std::vector<GenericDeck>& tableau = game.tableau;
    const auto backTexture(sf::Texture("src/Sprites/CardBackRed.png"));
    for (size_t i = 0; i < tableau.size(); i++) {
        for (size_t j = 0; j < tableau[i].size(); j++) {
            CardWithTexture& card = *tableau[i][j];
            card.setPosition({i*(CARD_WIDTH+TABLEAU_HORIZONTAL_SPACING), j*(TABLEAU_VERTICAL_SPACING)});
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
                        for (auto& f : foundation) {
                            if (f.size() > 0) {
                                if (game.isFoundationMoveValid(dragging_card_ptr, &f)) {

                                }
                            }
                        }
                    }

                    else {
                        if (deck_rect.contains(mouse_position)) {
                            game.drawFromDeck();
                            continue;
                        }
                        for (auto card_ptr : game.rendered_cards) {
                            if (card_ptr->is_clickable && card_ptr->createSprite().getGlobalBounds().contains(mouse_position)) {
                                initial_position = card_ptr->position;
                                if (card_ptr->value == 1) {
                                    switch (card_ptr->suit) {
                                        case 'H': {
                                            game.moveCard(card_ptr, foundation[0]);
                                            card_ptr->setPosition(foundation_positions[0]);
                                            card_ptr->makeUnclickable();
                                            break;
                                        }
                                        case 'D': {
                                            game.moveCard(card_ptr, foundation[1]);
                                            card_ptr->setPosition(foundation_positions[1]);
                                            card_ptr->makeUnclickable();
                                            break;
                                        }
                                        case 'S': {
                                            game.moveCard(card_ptr, foundation[2]);
                                            card_ptr->setPosition(foundation_positions[2]);
                                            card_ptr->makeUnclickable();
                                            break;
                                        }
                                        case 'C': {
                                            game.moveCard(card_ptr, foundation[3]);
                                            card_ptr->setPosition(foundation_positions[3]);
                                            card_ptr->makeUnclickable();
                                            break;
                                        }
                                        default:
                                            break;
                                    }
                                    break;
                                }
                                dragging_card_ptr = card_ptr;
                                break;
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
        if (deck.size() == 0) {
            const auto texture(sf::Texture("src/Sprites/GreenReload.png"));
            sf::Sprite sprite(texture);
            sprite.setPosition(deck_position);
            window.draw(sprite);
        }
        for (const auto card_ptr : game.rendered_cards) {
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
