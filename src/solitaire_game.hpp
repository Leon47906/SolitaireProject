#pragma once
#ifndef SOLITAIRE_GAME_HPP
#define SOLITAIRE_GAME_HPP
#include <algorithm>
#include "decks.hpp"

class SolitaireGame {
public:
    Deck deck;
    std::vector<GenericDeck> tableau;
    std::vector<GenericDeck> foundation;
    GenericDeck waste;
    sf::Vector2f deck_position;
    std::vector<sf::Vector2f> waste_positions, foundation_positions;
    SolitaireGame(const sf::Vector2f _deck_position, const std::vector<sf::Vector2f>& _waste_positions,
        const std::vector<sf::Vector2f>& _foundation_positions) : deck_position(_deck_position),
    waste_positions(_waste_positions), foundation_positions(_foundation_positions) {
        deck.shuffle();
        for (int i = 0; i < 7; i++) {
            tableau.emplace_back();
            for (int j = 0; j <= i; j++) {
                CardWithTexture* c_ptr = deck.draw_from_top();
                if (j == i) {
                    c_ptr->flip();
                    c_ptr->makeClickable();
                }
                tableau[i].add_to_top(c_ptr);
            }
        }
        for (int i = 0; i < 4; i++) {
            foundation.emplace_back();
        }
        deck.cards.back()->makeClickable();
    }
    [[nodiscard]] bool isGameWon() const {
        return std::all_of(foundation.begin(), foundation.end(), [](const auto& f) {
            return f.size() == 13;
        });
    }
    bool isOnTop(const CardWithTexture* card_ptr) const {
        for (const auto& t : tableau) {
            if (t.size() > 0) {
                if (t.cards.back() == card_ptr) {
                    return true;
                }
            }
        }
        if (waste.size() > 0) {
            if (waste.cards.back() == card_ptr) {
                return true;
            }
        }
        return false;
    }
    bool isFoundationMoveValid(const CardWithTexture* card_ptr, const GenericDeck* destination_ptr) const {
        if (!isOnTop(card_ptr)) {
            return false;
        }
        if (const CardWithTexture* top_card = destination_ptr->cards.back(); destination_ptr->size() == 0) {
            if (card_ptr->value == 1) {
                return true;
            }
        }
        else {
            if (top_card->value == card_ptr->value - 1 && top_card->suit == card_ptr->suit) {
                return true;
            }
        }
        return false;
    }
    static bool isTableauMoveValid(const CardWithTexture* card_ptr, const GenericDeck* destination_ptr) {
        if (const CardWithTexture* top_card = destination_ptr->cards.back(); destination_ptr->size() == 0) {
            if (card_ptr->value == 13) {
                return true;
            }
        } else {
            if (top_card->value == card_ptr->value + 1 && top_card->is_red != card_ptr->is_red) {
                return true;
            }
        }
        return false;
    }
    void print() const {
        std::cout << "Deck: ";
        deck.print();
        std::cout << "Tableau: " << std::endl;
        for (const auto& t : tableau) {
            for (const auto& c_ptr : t.cards) {
                std::cout << c_ptr->value << c_ptr->suit << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "Foundation: " << std::endl;
        for (const auto& f : foundation) {
            f.print();
        }
        std::cout << "Waste: ";
        waste.print();
    }
    void moveCard(CardWithTexture* card_ptr, GenericDeck* destination_ptr) {
        GenericDeck& destination = *destination_ptr;
        bool to_tableau = false, card_found = false;
        if (!card_ptr->is_clickable) {
            return;
        }
        for (auto & t : tableau) {
            if (destination_ptr == &t) {
                to_tableau = true;
                break;
            }
        }
        for (auto& t : tableau) {
            if (card_found) break;
            switch (t.size()) {
                case 0: {
                    continue;
                }
                case 1: {
                    if (t[0] == card_ptr) {
                        destination.add_to_top(t.draw_from_top());
                        card_found = true;
                    }
                    break;
                }
                default: {
                    if (auto card_iter = std::find(t.cards.begin(), t.cards.end(), card_ptr); card_iter != t.cards.end()){
                        card_found = true;
                        std::vector<CardWithTexture*> cards_to_move;
                        while (card_iter != t.cards.end()) {
                            cards_to_move.push_back(t.draw_from_top());
                            card_iter = std::find(t.cards.begin(), t.cards.end(), card_ptr);
                        }
                        while (!cards_to_move.empty()) {
                            destination.add_to_top(cards_to_move.back());
                            cards_to_move.pop_back();
                        }
                        if (t.size() > 0) {
                            CardWithTexture* new_top = t.cards.back();
                            new_top->makeClickable();
                            new_top->face_up ? void() : new_top->flip();
                        }
                        break;
                    }
                }
            }
        }
        if (waste.size() > 0 && waste.cards.back() == card_ptr) {
            switch (waste.size()) {
                case 1: {
                    destination.add_to_top(waste.draw_from_top());
                    break;
                }
                case 2: {
                    destination.add_to_top(waste.draw_from_top());
                    waste.cards.back()->makeClickable();
                    break;
                }
                case 3: {
                    destination.add_to_top(waste.draw_from_top());
                    CardWithTexture* second_last = waste.cards[waste.size()-2];
                    second_last->setPosition(waste_positions[0]);
                    CardWithTexture* last = waste.cards[waste.size()-1];
                    last->setPosition(waste_positions[1]);
                    last->makeClickable();
                    break;
                }
                default: {
                    destination.add_to_top(waste.draw_from_top());
                    CardWithTexture* third_last = waste.cards[waste.size()-3];
                    third_last->setPosition(waste_positions[0]);
                    CardWithTexture* second_last = waste.cards[waste.size()-2];
                    second_last->setPosition(waste_positions[1]);
                    CardWithTexture* last = waste.cards[waste.size()-1];
                    last->setPosition(waste_positions[2]);
                    last->makeClickable();
                }
            }
        }
        if (to_tableau && destination_ptr->size() > 0) {
            size_t flipped_count = 0;
            for (const auto& c : destination_ptr->cards) {
                if (c->face_up) {
                    c->makeUnclickable();
                    if (flipped_count == 0) c->makeClickable();
                    flipped_count++;
                }
            }
            if (flipped_count > 1) {
                CardWithTexture* last = destination_ptr->cards.back();
                last->makeClickable();
                last->face_up ? void() : last->flip();
            }
        }
    }
    void drawFromDeck() {
        if (deck.size() == 0) {
            while (waste.size() > 0) {
                CardWithTexture* c_ptr = waste.draw_from_top();
                c_ptr->flip();
                c_ptr->makeUnclickable();
                c_ptr->setPosition(deck_position);
                deck.add_to_top(c_ptr);
            }
            return;
        }
        CardWithTexture* c_ptr = deck.draw_from_top();
        if (deck.size() > 0) {
            deck.cards.back()->makeClickable();
        }
        switch (size_t waste_size = waste.size()) {
            case 0: {
                c_ptr->setPosition(waste_positions[0]);
                c_ptr->flip();
                c_ptr->makeClickable();
                waste.add_to_top(c_ptr);
                break;
            }
            case 1: {
                waste[0]->makeUnclickable();
                c_ptr->setPosition(waste_positions[1]);
                c_ptr->flip();
                c_ptr->makeClickable();
                waste.add_to_top(c_ptr);
                break;
            }
            case 2: {
                waste[1]->makeUnclickable();
                c_ptr->setPosition(waste_positions[2]);
                c_ptr->flip();
                c_ptr->makeClickable();
                waste.add_to_top(c_ptr);
                break;
            }
            default: {
                CardWithTexture* second_last = waste.cards[waste_size-2];
                second_last->setPosition(waste_positions[0]);
                CardWithTexture* last = waste.cards[waste_size-1];
                last->makeUnclickable();
                last->setPosition(waste_positions[1]);
                c_ptr->setPosition(waste_positions[2]);
                c_ptr->flip();
                c_ptr->makeClickable();
                waste.add_to_top(c_ptr);
            }
        }
    }
};




#endif //SOLITAIRE_GAME_HPP
