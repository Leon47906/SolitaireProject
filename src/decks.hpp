#pragma once
#ifndef DECKS_HPP
#define DECKS_HPP
#include <iostream>
#include "cards.hpp"
#include <vector>
#include <random>

class GenericDeck {
    public:
        GenericDeck() = default;
        std::vector<CardWithTexture*> cards;
        void shuffle() {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dist(0, cards.size() - 1);
            for (auto & card : cards) {
                const unsigned int j = dist(gen);
                std::swap(card, cards[j]);
            }
        }
        CardWithTexture* draw_from_top() {
            CardWithTexture* c_ptr = cards.back();
            cards.pop_back();
            return c_ptr;
        }
        CardWithTexture* draw_from_bottom() {
            CardWithTexture* c = cards.front();
            cards.erase(cards.begin());
            return c;
        }
        void add_to_top(CardWithTexture* c) {
            cards.push_back(c);
        }
        void add_to_bottom(CardWithTexture* c) {
            cards.insert(cards.begin(), c);
        }
        void print() const {
            for (const auto& card : cards) {
                std::cout << card->value << card->suit << " ";
            }
            std::cout << std::endl;
        }
        CardWithTexture* operator[](size_t index) {
            return cards[index];
        }
        [[nodiscard]] unsigned int size() const { return cards.size(); }
};

class Deck : public GenericDeck {
    public:
        Deck() {
            for (int i = 1; i <= 13; i++) {
                const std::vector<std::string> filenames(4);
                for (const auto& suit : {'H', 'D', 'S', 'C'}) {
                    //const std::string filename = "src/sprites/" + std::to_string(i) + suit + ".png";
                    cards.emplace_back(i, suit, "src/Sprites/1C.png");
                }
            }
        }
};

#endif //DECKS_HPP
