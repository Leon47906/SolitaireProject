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
    std::vector<CardWithTexture*> rendered_cards;
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
                rendered_cards.push_back(c_ptr);
                tableau[i].add_to_top(c_ptr);
            }
        }
        for (int i = 0; i < 4; i++) {
            foundation.emplace_back();
        }
        rendered_cards.push_back(deck.cards.back());
        deck.cards.back()->makeClickable();
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
    static bool isFoundationMoveValid(const CardWithTexture* card_ptr, const GenericDeck* destination_ptr) {
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
    void sortDeck(GenericDeck* deck_ptr) {

    }
    void moveCard(CardWithTexture* card_ptr, GenericDeck* destination_ptr) {
        GenericDeck& destination = *destination_ptr;
        bool to_tableau = false;
        for (auto & i : tableau) {
            if (destination_ptr == &i) {
                to_tableau = true;
                break;
            }
        }
        if (!card_ptr->is_clickable) {
            return;
        }
        for (auto& t : tableau) {
            switch (t.size()) {
                case 0: {
                    continue;
                }
                case 1: {
                    if (t[0] == card_ptr) destination.add_to_top(t.draw_from_top());
                    break;
                }
                default: {
                    for (size_t i = 0; i < t.size(); i++) {
                        if (t[i] == card_ptr) {
                            destination.add_to_top(t.draw_from_top());
                            CardWithTexture* new_top = t.cards.back();
                            new_top->makeClickable();
                            new_top->flip();
                        }
                    }
                }
            }
        }
        if (waste.size() > 0 && waste.cards.back() == card_ptr) {
            if (waste.size() < 3) destination.add_to_top(waste.draw_from_top());
            else {
                destination.add_to_top(waste.draw_from_top());
                CardWithTexture* third_last = waste.cards[waste.size()-3];
                third_last->setPosition(waste_positions[0]);
                CardWithTexture* second_last = waste.cards[waste.size()-2];
                second_last->setPosition(waste_positions[1]);
                CardWithTexture* last = waste.cards[waste.size()-1];
                last->setPosition(waste_positions[2]);
                last->makeClickable();
                for (int i = 0; i < rendered_cards.size(); i++) {
                    if (rendered_cards[i] == second_last) {
                        rendered_cards.insert(rendered_cards.begin()+i, third_last);
                        break;
                    }
                }
            }
        }
        if (to_tableau) {
            size_t flipped_count = 0;
            for (const auto& c : destination_ptr->cards) {
                if (c->face_up) {
                    c->makeUnclickable();
                    if (flipped_count == 0) c->makeClickable();
                    flipped_count++;
                }
            }
            if (flipped_count >= 2) {
                CardWithTexture* last = destination_ptr->cards.back();
                last->makeClickable();
                last->face_up ? void() : last->flip();
            }
        }
        sortDeck(destination_ptr);
    }
    void drawFromDeck() {
        if (deck.size() == 0) {
            while (waste.size() > 0) {
                CardWithTexture* c_ptr = waste.draw_from_top();
                c_ptr->flip();
                c_ptr->makeUnclickable();
                c_ptr->setPosition(deck_position);
                for (int i = 0; i < rendered_cards.size(); i++) {
                    if (rendered_cards[i] == c_ptr) {
                        rendered_cards.erase(rendered_cards.begin() + i);
                        break;
                    }
                }
                deck.add_to_top(c_ptr);
            }
            return;
        }
        CardWithTexture* c_ptr = deck.draw_from_top();
        if (deck.size() > 0) {
            deck.cards.back()->makeClickable();
            rendered_cards.push_back(deck.cards.back());
        }
        switch (size_t waste_size = waste.size()) {
            case 0: {
                c_ptr->setPosition(waste_positions[0]);
                c_ptr->flip();
                rendered_cards.push_back(c_ptr);
                waste.add_to_top(c_ptr);
                break;
            }
            case 1: {
                waste[0]->makeUnclickable();
                c_ptr->setPosition(waste_positions[1]);
                c_ptr->flip();
                rendered_cards.push_back(c_ptr);
                waste.add_to_top(c_ptr);
                break;
            }
            case 2: {
                waste[1]->makeUnclickable();
                c_ptr->setPosition(waste_positions[2]);
                c_ptr->flip();
                rendered_cards.push_back(c_ptr);
                waste.add_to_top(c_ptr);
                break;
            }
            default: {
                //remove the third last card from the waste from the rendered cards
                CardWithTexture* third_last = waste.cards[waste_size-3];
                for (int i = 0; i < rendered_cards.size(); i++) {
                    if (rendered_cards[i] == third_last) {
                        rendered_cards.erase(rendered_cards.begin() + i);
                        break;
                    }
                }
                CardWithTexture* second_last = waste.cards[waste_size-2];
                second_last->setPosition(waste_positions[0]);
                CardWithTexture* last = waste.cards[waste_size-1];
                last->makeUnclickable();
                last->setPosition(waste_positions[1]);
                c_ptr->setPosition(waste_positions[2]);
                c_ptr->flip();
                rendered_cards.push_back(c_ptr);
                waste.add_to_top(c_ptr);
            }
        }
    }
};




#endif //SOLITAIRE_GAME_HPP
