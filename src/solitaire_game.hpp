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
                tableau[i].add_to_top(&c);
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
                if (CardWithTexture* c_ptr = t[size-1]; c_ptr->is_clickable) {
                    CardWithTexture& c = *c_ptr;
                    std::cout << c.toString() << std::endl;
                    clickable_cards.push_back(c);
                }
            }
        }
        if (waste.size() > 0) {
            if (CardWithTexture* c_ptr = waste.cards.back(); c_ptr->is_clickable) {
                CardWithTexture& c = *c_ptr;
                clickable_cards.push_back(c);
            }
        }
        return clickable_cards;
    }
    bool isMoveValid(const CardWithTexture& card, const GenericDeck& destination) {
        if (destination.size() == 0) {
            if (card.value == 13) {
                return true;
            }
        } else {
            const CardWithTexture* top_card = destination[destination.size()-1];
            if (top_card->value == card.value + 1 && top_card->is_red != card.is_red) {
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
                if (c_ptr->face_up) {
                    std::cout << c_ptr->value << c_ptr->suit << " ";
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
Deck deck;
std::vector<GenericDeck> tableau;
std::vector<GenericDeck> foundation;
GenericDeck waste;
inline void move_card(CardWithTexture* card, GenericDeck& destination) {
    if (!card->is_clickable) {
        return;
    }
    for (auto& c : deck) {
        if (c == card) {
            if (deck.size() == 1) {
                destination.add_to_top(card);
                deck.cards.pop_back();
                return;
            }

            deck.cards.pop_back();
            destination.add_to_top(card);
            return;
        }
    }
    for (auto& t : tableau) {
        for (auto& c : t.cards) {
            if (c == card) {
                if (t.size() == 1) {
                    destination.add_to_top(card);
                    t.cards.pop_back();
                    return;
                }
                destination.add_to_top(card);
                t.cards.pop_back();
                CardWithTexture* new_top = t[t.size()-1];
                new_top->makeClickable();
                new_top->flip();
                return;
            }
        }
    }
    for (auto& c : waste) {
        if (c == card) {
            waste.cards.pop_back();
            destination.add_to_top(card);
            return;
        }
    }
    if (origin.size() == 0) {
        return;
    }
    if (origin.size() == 1) {
        destination.add_to_top(card);
        origin.cards.pop_back();
        return;
    }
    if (origin.size() > 1) {
        origin.cards.pop_back();
        CardWithTexture* new_top = origin[origin.size()-1];
        new_top->makeClickable();
        new_top->flip();
        destination.add_to_top(card);
    }
}

inline void move_card(CardWithTexture* card, Deck& origin, GenericDeck& destination) {
    origin.cards.pop_back();
    destination.add_to_top(card);
}

#endif //SOLITAIRE_GAME_HPP
