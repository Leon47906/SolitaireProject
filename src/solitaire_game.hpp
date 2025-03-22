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
    std::vector<CardWithTexture*> movable_cards, rendered_cards;
    SolitaireGame() {
        deck.shuffle();
        for (int i = 0; i < 7; i++) {
            tableau.emplace_back();
            for (int j = 0; j <= i; j++) {
                CardWithTexture* c_ptr = deck.draw_from_top();
                if (j == i) {
                    c_ptr->flip();
                    c_ptr->makeClickable();
                    movable_cards.push_back(c_ptr);
                }
                rendered_cards.push_back(c_ptr);
                tableau[i].add_to_top(c_ptr);
            }
        }
        for (int i = 0; i < 4; i++) {
            foundation.emplace_back();
        }
        rendered_cards.push_back(deck.cards.back());
    }
    void drawFromDeck(const sf::Vector2f waste_position) {
        if (deck.size() == 0) {
            while (waste.size() > 0) {
                CardWithTexture* c_ptr = waste.draw_from_top();
                c_ptr->flip();
                c_ptr->makeUnclickable();
                deck.add_to_top(c_ptr);
            }
            return;
        }
        CardWithTexture* c_ptr = deck.draw_from_top();
        c_ptr->flip();
        c_ptr->makeClickable();
        switch (size_t waste_size = waste.size()) {
        case 0: {
            c_ptr->position = waste_position;
            break;
        }
        case 1: {
            c_ptr->position = waste_position+sf::Vector2f({0, TABLEAU_VERTICAL_SPACING});
            break;
        }
        case 2: {
            //waste[1]->makeUnclickable();
            c_ptr->position = waste_position+sf::Vector2f({0, 2*TABLEAU_VERTICAL_SPACING});
            break;
        }
        default: {
          // remove waste[0] from rendered_cards
            for (int i = 0; i < rendered_cards.size(); i++) {
                if (rendered_cards[i] == waste[waste_size-3]) {
                    std::cout << "Erasing " << rendered_cards[i]->toString() << std::endl;
                    rendered_cards.erase(rendered_cards.begin() + i);
                }
            }
            for (int i = 0; i < 3 ; i++) {
                waste[waste_size - i - 1]->position = waste_position+sf::Vector2f({0, (2-i)*TABLEAU_VERTICAL_SPACING});
            }
            break;
        }
        }
        waste.add_to_top(c_ptr);
        rendered_cards.push_back(deck.cards.back());
        for (int i = 0; i < waste.size(); i++) {
            std::cout << waste[i]->toString() + " ";
        }
        std::cout << std::endl;
        //print();
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
    void move_card(CardWithTexture* card, GenericDeck& destination) {
        if (!card->is_clickable) {
            return;
        }
        if (deck.size() > 0 && deck.cards.back() == card) {
            if (deck.size() == 1) {
                destination.add_to_top(card);
                deck.cards.pop_back();
                card->flip();
                card->makeClickable();
                return;
            }
            destination.add_to_top(card);
            deck.cards.pop_back();
            CardWithTexture* new_top = deck.cards.back();
            new_top->flip();
            new_top->makeClickable();
            rendered_cards.push_back(new_top);
            return;
        }
        for (auto& t : tableau) {
            if (t.size() == 0) {
                continue;
            }
            for (auto& c : t.cards) {
                if (c == card) {
                    if (t.size() == 1) {
                        destination.add_to_top(card);
                        t.cards.pop_back();
                        return;
                    }
                    destination.add_to_top(card);
                    t.cards.pop_back();
                    CardWithTexture* new_top = t.cards.back();
                    new_top->makeClickable();
                    new_top->flip();
                    return;
                }
            }
        }
        if (waste.size() > 0 && waste.cards.back() == card) {
            waste.cards.pop_back();
            destination.add_to_top(card);
            return;
        }
    }
};



inline void move_card(CardWithTexture* card, Deck& origin, GenericDeck& destination) {
    origin.cards.pop_back();
    destination.add_to_top(card);
}

#endif //SOLITAIRE_GAME_HPP
