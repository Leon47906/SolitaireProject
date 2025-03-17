#pragma once
#ifndef SOLITAIRE_GAME_HPP
#define SOLITAIRE_GAME_HPP
#include "decks.hpp"

class SolitaireGame {
public:
    Deck deck;
    std::vector<GenericDeck> tableau;
    std::vector<GenericDeck> foundation;
    GenericDeck waste;
    SolitaireGame() {
        deck.shuffle();
        for (int i = 0; i < 7; i++) {
            tableau.emplace_back();
            for (int j = 0; j <= i; j++) {
                CardWithTexture* c_ptr = deck.draw_from_top();
                CardWithTexture& c = *c_ptr;
                if (j == i) {
                    c.flip();
                    c.makeClickable();
                }
                tableau[i].add_to_top(c);
            }
        }
        for (int i = 0; i < 4; i++) {
            foundation.emplace_back();
        }
    }
    [[nodiscard]] std::vector<CardWithTexture> getClickableCards() {
        std::vector<CardWithTexture> clickable_cards;
        for (auto& t : tableau) {
            if (const size_t size = t.size(); size > 0) {
                if (CardWithTexture c = t[size-1]; c.is_clickable) {
                    std::cout << c.toString() << std::endl;
                    clickable_cards.push_back(c);
                }
            }
        }
        if (waste.size() > 0) {
            if (CardWithTexture c = waste.cards.back(); c.is_clickable) {
                clickable_cards.push_back(c);
            }
        }
        return clickable_cards;
    }
    void print() const {
        std::cout << "Deck: ";
        deck.print();
        std::cout << "Tableau: " << std::endl;
        for (const auto& t : tableau) {
            for (const auto& c : t.cards) {
                if (c.face_up) {
                    std::cout << c.value << c.suit << " ";
                } else {
                    std::cout << "X ";
                }
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
};

#endif //SOLITAIRE_GAME_HPP
