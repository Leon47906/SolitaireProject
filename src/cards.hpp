#pragma once

#ifndef CARDS_HPP
#define CARDS_HPP
#include <SFML/Graphics.hpp>

class Card {
public:
    int value;
    char suit;
    bool face_up, is_clickable;
    Card(const int _value, const char _suit) : value(_value), suit(_suit), face_up(false),
    is_clickable(false) {}
    void flip() { face_up = !face_up; }
    void make_clickable() { is_clickable = true; }
    void make_unclickable() { is_clickable = false; }
};

class CardWithSprite : public Card {
public:
    sf::Sprite sprite;
    sf::Vector2f position;
    CardWithSprite(const int _value, const char _suit, const sf::Texture& texture) : Card(_value, _suit),
    sprite(texture), position({0,0}) {}
    void set_position(const sf::Vector2f& position) { sprite.setPosition(position); }
    void draw(sf::RenderWindow& window) const { window.draw(sprite); }
};
#endif //CARDS_HPP
