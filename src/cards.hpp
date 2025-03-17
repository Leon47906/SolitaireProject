#pragma once

#ifndef CARDS_HPP
#define CARDS_HPP
#include <SFML/Graphics.hpp>
#include <utility>

constexpr float CARD_WIDTH = 64.0f;
constexpr float CARD_HEIGHT = 96.0f;
constexpr float TABLEAU_VERTICAL_SPACING = 16.0f;
constexpr float TABLEAU_HORIZONTAL_SPACING = 16.0f;

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
    void makeClickable() { is_clickable = true; }
    void makeUnclickable() { is_clickable = false; }
    std::string toString() const {
        return std::to_string(value) + suit;
    }
};

class CardWithTexture : public Card {
public:
    sf::Texture texture;
    sf::Vector2f position;
    CardWithTexture(const int _value, const char _suit, const std::filesystem::path& path_to_texture) : Card(_value, _suit),
    position({0,0}), texture(sf::Texture(path_to_texture)) {}
    //Copy constructor
    CardWithTexture(const CardWithTexture& other) : Card(other.value, other.suit),
    texture(other.texture), position(other.position) {}
    void setPosition(const sf::Vector2f& _position) { position = _position; }
    sf::Sprite createSprite() const {
        sf::Sprite sprite(texture);
        sprite.setPosition(position);
        return sprite;
    }
    sf::Sprite createBackSprite() const {
        const auto texture(sf::Texture("src/Sprites/CardBackRed.png"));
        sf::Sprite sprite(texture);
        sprite.setPosition(position);
        return sprite;
    }
};
#endif //CARDS_HPP
