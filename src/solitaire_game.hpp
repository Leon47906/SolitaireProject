#pragma once
#ifndef SOLITAIRE_GAME_HPP
#define SOLITAIRE_GAME_HPP
#include "decks.hpp"

class SolitaireGame {
    Deck deck;
    std::vector<GenericDeck> tableau;
    std::vector<GenericDeck> foundation;
    GenericDeck waste;
    public:
        SolitaireGame() {
            deck.shuffle();
            for (int i = 0; i < 7; i++) {
                tableau.emplace_back();
                for (int j = 0; j <= i; j++) {
                    CardWithSprite c = deck.draw_from_top();
                    if (j == i) {
                        c.flip();
                        c.make_clickable();
                    }
                    tableau[i].add_to_top(c);
                }
            }
            for (int i = 0; i < 4; i++) {
                foundation.emplace_back();
            }
        }
        [[nodiscard]] std::vector<CardWithSprite> get_clickable_cards() const {
            std::vector<CardWithSprite> clickable_cards;
            for (auto& t : tableau) {
                if (t.size() > 0) {
                if (CardWithSprite c = t.cards.back(); c.is_clickable) {
                        clickable_cards.push_back(c);
                    }
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
