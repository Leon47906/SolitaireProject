#pragma once

#ifndef CARDS_HPP
#define CARDS_HPP
#include <SFML/Graphics.hpp>
#include <utility>

constexpr float CARD_WIDTH = 64.0f;
constexpr float CARD_HEIGHT = 96.0f;
constexpr float TABLEAU_SPACING = 16.0f;

class Card {
public:
    int value;
    char suit;
    bool face_up, is_clickable, is_red;
    Card(const int _value, const char _suit) : value(_value), suit(_suit), face_up(false),
    is_clickable(false) {
        if (suit == 'H' || suit == 'D') {
            is_red = true;
        } else {
            is_red = false;
        }
    }
    void flip() { face_up = !face_up; }
    void make_clickable() { is_clickable = true; }
    void make_unclickable() { is_clickable = false; }
    [[nodiscard]] std::string to_string() const {
        return std::to_string(value) + suit;
    }
};

class CardWithTexture : public Card {
public:
    sf::Texture texture;
    CardWithTexture(const int _value, const char _suit, sf::Texture  _texture) : Card(_value, _suit),
    texture(std::move(_texture)) {}
    //Copy constructor
    CardWithTexture(const CardWithTexture& other) : Card(other.value, other.suit),
    texture(other.texture) {}
    sf::Sprite create_sprite() const {
        sf::Sprite sprite(texture);
        return sprite;
    }
};
#endif //CARDS_HPP
