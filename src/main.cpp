#include <SFML/Graphics.hpp>    // Include the SFML header files
#include <SFML/Window.hpp>
#include "solitaire_game.hpp"   // Include the header file

constexpr sf::Vector2f offset = {32.0f, 48.0f};

int main()
{
    sf::Vector2f deck_position = {7*(CARD_WIDTH+TABLEAU_HORIZONTAL_SPACING), 0},
        waste_position = {8*(CARD_WIDTH+TABLEAU_HORIZONTAL_SPACING), 0};
    sf::Rect deck_rect(deck_position, {CARD_WIDTH, CARD_HEIGHT});
    std::vector<sf::Rect<float>> empty_tableau_rects;
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
    Deck& deck = game.deck;
    GenericDeck& waste = game.waste;
    std::vector<GenericDeck>& foundation = game.foundation;
    for (auto card_ptr : deck.cards) {
        CardWithTexture& card = *card_ptr;
        card.setPosition(deck_position);
    }
    std::vector<GenericDeck>& tableau = game.tableau;
    const auto backTexture(sf::Texture("src/Sprites/CardBackRed.png"));
    const auto backgroundTexture(sf::Texture("src/Sprites/Background.png"));
    const auto congratulationsSheet(sf::Texture("src/Sprites/congratulationFrames.png"));
    for (size_t i = 0; i < tableau.size(); i++) {
        for (size_t j = 0; j < tableau[i].size(); j++) {
            CardWithTexture& card = *tableau[i][j];
            sf::Vector2f position = {i*(CARD_WIDTH+TABLEAU_HORIZONTAL_SPACING), j*TABLEAU_VERTICAL_SPACING};
            card.setPosition(position);
            if (j == 0) empty_tableau_rects.push_back({position, {CARD_WIDTH, CARD_HEIGHT}});
        }
    }
    auto window = sf::RenderWindow(sf::VideoMode({800u, 600u}), "Solitaire",
        sf::Style::Close);
    sf::IntRect rectSourceSprite({0, 0}, {400, 209});
    sf::Sprite congratulationsSprite(congratulationsSheet, rectSourceSprite);
    congratulationsSprite.setPosition({200, 150});
    sf::Clock clock;
    sf::Sprite backgroundSprite(backgroundTexture);
    window.setFramerateLimit(144);
    CardWithTexture* dragging_card_ptr = nullptr;
    sf::Vector2f initial_position;
    bool isGameWon = false;
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
            if (isGameWon) break;
            if (game.isGameWon()) isGameWon = true;
            if (const auto* mouseButtonPresed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButtonPresed->button == sf::Mouse::Button::Right && dragging_card_ptr != nullptr) {
                    bool from_tableau = false;
                    std::vector<CardWithTexture*> moving_cards;
                    for (auto& t : tableau) {
                        if (auto card_it = std::find(t.cards.begin(), t.cards.end(), dragging_card_ptr); card_it != t.cards.end()) {
                            for (auto it = card_it; it < t.cards.end(); ++it) {
                                moving_cards.push_back(*it);
                            }
                            from_tableau = true;
                            break;
                        }
                    }
                    if (!from_tableau) dragging_card_ptr->position = initial_position;
                    else {
                        for (size_t i = 0; i < moving_cards.size(); i++) {
                            moving_cards[i]->position = initial_position + sf::Vector2f(0, i*TABLEAU_VERTICAL_SPACING);
                        }
                    }
                    dragging_card_ptr = nullptr;
                }
                if (mouseButtonPresed->button == sf::Mouse::Button::Left) {
                    if (dragging_card_ptr != nullptr) {
                        for (auto& f : foundation) {
                            if (f.size() > 0) {
                                if (f.cards.back()->createSprite().getGlobalBounds().contains(mouse_position)) {
                                    if (game.isFoundationMoveValid(dragging_card_ptr, &f)) {
                                        game.moveCard(dragging_card_ptr, &f);
                                        dragging_card_ptr->setPosition(foundation_positions[&f - &foundation[0]]);
                                        dragging_card_ptr->makeUnclickable();
                                        dragging_card_ptr = nullptr;
                                        break;
                                    }
                                }
                            }
                        }
                        if (dragging_card_ptr == nullptr) {
                            continue;
                        }
                        for (size_t i = 0; i < tableau.size(); i++) {
                            GenericDeck& t = tableau[i];
                            if (t.size() > 0) {
                                if (t.cards.back()->createSprite().getGlobalBounds().contains(mouse_position)) {
                                    if (SolitaireGame::isTableauMoveValid(dragging_card_ptr, &t)) {
                                        game.moveCard(dragging_card_ptr, &t);
                                        for (size_t j = 0; j < t.size(); j++) {
                                            t[j]->setPosition({i*(CARD_WIDTH+TABLEAU_HORIZONTAL_SPACING), j*TABLEAU_VERTICAL_SPACING});
                                        }
                                        dragging_card_ptr = nullptr;
                                        break;
                                    }
                                }
                            }
                            else {
                                if (empty_tableau_rects[i].contains(mouse_position)) {
                                    if (SolitaireGame::isTableauMoveValid(dragging_card_ptr, &t)) {
                                        game.moveCard(dragging_card_ptr, &t);
                                        for (size_t j = 0; j < t.size(); j++) {
                                            t[j]->setPosition({i*(CARD_WIDTH+TABLEAU_HORIZONTAL_SPACING), j*TABLEAU_VERTICAL_SPACING});
                                        }
                                        dragging_card_ptr = nullptr;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    else {
                        if (deck_rect.contains(mouse_position)) {
                            game.drawFromDeck();
                            continue;
                        }
                        if (waste.size() > 0) {
                            CardWithTexture* card_ptr = waste.cards.back();
                            if (card_ptr->is_clickable && card_ptr->createSprite().getGlobalBounds().contains(mouse_position)) {
                                initial_position = card_ptr->position;
                                if (card_ptr->value == 1) {
                                    switch (card_ptr->suit) {
                                        case 'H': {
                                            game.moveCard(card_ptr, &foundation[0]);
                                            card_ptr->setPosition(foundation_positions[0]);
                                            card_ptr->makeUnclickable();
                                            break;
                                        }
                                        case 'D': {
                                            game.moveCard(card_ptr, &foundation[1]);
                                            card_ptr->setPosition(foundation_positions[1]);
                                            card_ptr->makeUnclickable();
                                            break;
                                        }
                                        case 'S': {
                                            game.moveCard(card_ptr, &foundation[2]);
                                            card_ptr->setPosition(foundation_positions[2]);
                                            card_ptr->makeUnclickable();
                                            break;
                                        }
                                        case 'C': {
                                            game.moveCard(card_ptr, &foundation[3]);
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
                        for (auto& t : tableau) {
                            if (t.size() > 0) {
                                for (auto card_ptr : t.cards) {
                                    if (card_ptr->is_clickable && card_ptr->createSprite().getGlobalBounds().contains(mouse_position)) {
                                        initial_position = card_ptr->position;
                                        if (card_ptr->value == 1) {
                                            switch (card_ptr->suit) {
                                                case 'H': {
                                                    game.moveCard(card_ptr, &foundation[0]);
                                                    card_ptr->setPosition(foundation_positions[0]);
                                                    card_ptr->makeUnclickable();
                                                    break;
                                                }
                                                case 'D': {
                                                    game.moveCard(card_ptr, &foundation[1]);
                                                    card_ptr->setPosition(foundation_positions[1]);
                                                    card_ptr->makeUnclickable();
                                                    break;
                                                }
                                                case 'S': {
                                                    game.moveCard(card_ptr, &foundation[2]);
                                                    card_ptr->setPosition(foundation_positions[2]);
                                                    card_ptr->makeUnclickable();
                                                    break;
                                                }
                                                case 'C': {
                                                    game.moveCard(card_ptr, &foundation[3]);
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
                }
            }
            if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>())
            {
                if (dragging_card_ptr != nullptr) {
                    bool from_tableau = false;
                    std::vector<CardWithTexture*> moving_cards;
                    for (auto& t : tableau) {
                        if (auto card_it = std::find(t.cards.begin(), t.cards.end(), dragging_card_ptr); card_it != t.cards.end()) {
                            for (auto it = card_it; it < t.cards.end(); ++it) {
                                moving_cards.push_back(*it);
                            }
                            from_tableau = true;
                            break;
                        }
                    }
                    if (!from_tableau) dragging_card_ptr->position = mouse_position - offset;
                    else {
                        for (size_t i = 0; i < moving_cards.size(); i++) {
                            moving_cards[i]->position = mouse_position - offset + sf::Vector2f(0, i*TABLEAU_VERTICAL_SPACING);
                        }
                    }
                }
            }

        }
        if (isGameWon) {
            if (clock.getElapsedTime().asSeconds() > 0.15f) {
                if (rectSourceSprite.position.x == 1200) rectSourceSprite.position.x = 0;
                else rectSourceSprite.position.x += 400;
                congratulationsSprite.setTextureRect(rectSourceSprite);
                clock.restart();
            }
            window.clear();
            window.draw(backgroundSprite);
            window.draw(congratulationsSprite);
            window.display();
            continue;
        }
        window.clear();
        window.draw(backgroundSprite);
        if (deck.size() > 0) {
            sf::Sprite backSprite(backTexture);
            backSprite.setPosition(deck_position);
            window.draw(backSprite);
        }
        if (deck.size() == 0) {
            if (!waste.cards.empty()) {
                const auto texture(sf::Texture("src/Sprites/GreenReload.png"));
                sf::Sprite sprite(texture);
                sprite.setPosition(deck_position);
                window.draw(sprite);
            }
        }
        for (const auto & i : tableau) {
            for (size_t j = 0; j < i.size(); j++) {
                if (const CardWithTexture& card = *i[j]; card.face_up) {
                    window.draw(card.createSprite());
                } else {
                    sf::Sprite backSprite(backTexture);
                    backSprite.setPosition(card.position);
                    window.draw(backSprite);
                }
            }
        }
        for (const auto & pile : foundation) {
            if (pile.size() > 0) {
                window.draw(pile.cards.back()->createSprite());
            }
        }
        switch (waste.size()) {
            case 0: break;
            case 1: {
                window.draw(waste.cards.back()->createSprite());
                break;
            }
            case 2: {
                window.draw(waste.cards[0]->createSprite());
                window.draw(waste.cards[1]->createSprite());
                break;
            }
            default: {
                window.draw(waste.cards[waste.size()-3]->createSprite());
                window.draw(waste.cards[waste.size()-2]->createSprite());
                window.draw(waste.cards[waste.size()-1]->createSprite());
            }
        }
        window.display();
    }
}
