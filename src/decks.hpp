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
        std::vector<CardWithSprite> cards;
        void shuffle() {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dist(0, cards.size() - 1);
            for (auto & card : cards) {
                const uint j = dist(gen);
                std::swap(card, cards[j]);
            }
        }
        CardWithSprite draw_from_top() {
            const CardWithSprite c = cards.back();
            cards.pop_back();
            return c;
        }
        CardWithSprite draw_from_bottom() {
            const CardWithSprite c = cards.front();
            cards.erase(cards.begin());
            return c;
        }
        void add_to_top(const CardWithSprite& c) {
            cards.push_back(c);
        }
        void add_to_bottom(const CardWithSprite& c) {
            cards.insert(cards.begin(), c);
        }
        void print() const {
            for (const auto& card : cards) {
                std::cout << card.value << card.suit << " ";
            }
            std::cout << std::endl;
        }
        int size() const { return cards.size(); }
};

class Deck : public GenericDeck {
    public:
        Deck() {
            for (int i = 1; i <= 13; i++) {
                const std::vector<std::string> filenames(4);
                for (const auto& suit : {'H', 'D', 'S', 'C'}) {
                    const std::string filename = "src/sprites/" + std::to_string(i) + suit + ".png";
                    const sf::Texture texture(filename);
                    cards.emplace_back(i, suit, texture);
                }
                /*
                cards.emplace_back(i, 'H');
                cards.emplace_back(i, 'D');
                cards.emplace_back(i, 'S');
                cards.emplace_back(i, 'C');
                */
            }
        }
};

#endif //DECKS_HPP
