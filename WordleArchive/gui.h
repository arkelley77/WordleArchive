#ifndef GUI_H
#define GUI_H

#include <array>
#include <iostream>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include <SFML/Graphics.hpp>

template <class SFMLObject>
void centerOrigin(SFMLObject& obj) {
  sf::FloatRect bounds = obj.getLocalBounds();
  obj.setOrigin(
    bounds.left + bounds.width / 2.f,
    bounds.top + bounds.height / 2.f
  );
}

using action = std::function<void()>;

class UIElement {
public:
  UIElement(sf::RenderWindow& win, bool visible = true)
    : window(win), visible(visible) {
    /*if (mouse_buttons.empty()) {
      for (int button_type = 0; button_type < sf::Mouse::ButtonCount; ++button_type) {
        mouse_buttons.emplace(static_cast<sf::Mouse::Button>(button_type), false);
      }
    }
    if (keys.empty()) {
      for (int key = 0; key < sf::Keyboard::KeyCount; ++key) {
        keys.emplace(static_cast<sf::Keyboard::Key>(key), false);
      }
    }*/
  }

  UIElement(const UIElement& other)
    : window(other.window), visible(other.visible) {}
  
  
  static bool something_changed;  // flag to redraw the screen when something changes

  constexpr static char* type = "UIElement";
  
  void draw() { if (visible) onDraw(); }
  void handleEvent(const sf::Event& event) { if (visible) onEvent(event); }

  virtual void onDraw() {}
  virtual void onKeyPress(const sf::Event::KeyEvent& event) {}
  virtual void onKeyRelease(const sf::Event::KeyEvent& event) {}
  virtual void onMousePress(const sf::Event::MouseButtonEvent& event) {}
  virtual void onMouseRelease(const sf::Event::MouseButtonEvent& event) {}
  virtual void onMouseMove(const sf::Event::MouseMoveEvent& event) {}
  virtual void onEvent(const sf::Event& event) {
    switch (event.type) {
    case sf::Event::KeyPressed:
      keys[event.key.code] = true;
      onKeyPress(event.key);
      break;
    case sf::Event::KeyReleased:
      if (keys[event.key.code]) {
        keys[event.key.code] = false;
        onKeyRelease(event.key);
      }
      break;
    case sf::Event::MouseButtonPressed:
      mouse_buttons[event.mouseButton.button] = true;
      onMousePress(event.mouseButton);
      break;
    case sf::Event::MouseButtonReleased:
      if (mouse_buttons[event.mouseButton.button]) {
        mouse_buttons[event.mouseButton.button] = false;
        onMouseRelease(event.mouseButton);
      }
      break;
    case sf::Event::MouseMoved:
      onMouseMove(event.mouseMove);
      break;
    default: break;
    }
  }
  virtual void onRefresh() {}

  void show() {
    if (!visible) {
      visible = true;
      something_changed = true;
    }
  }
  void hide() {
    if (visible) {
      visible = false;
      something_changed = true;
    }
  }
protected:
  sf::RenderWindow& window;
  bool visible;

  static std::array<bool, sf::Mouse::ButtonCount> mouse_buttons;
  static std::array<bool, sf::Keyboard::KeyCount> keys;
};

bool UIElement::something_changed = true;
std::array<bool, sf::Mouse::ButtonCount> UIElement::mouse_buttons = {};
std::array<bool, sf::Keyboard::KeyCount> UIElement::keys = {};


class Text : public UIElement {
public:
  Text(sf::RenderWindow& win) : text(), UIElement(win) {}
  Text(const Text& other) : text(other.text), UIElement(other) {}

  constexpr static char* type = "Text";

  sf::Text text;
  virtual void onDraw() override {
    window.draw(text);
  }
};

class Button : public Text {
public:
  Button(sf::RenderWindow& win)
    : rect(), clicked(false), moused(false), Text(win) {}
  Button(const Button& other)
    : rect(other.rect), on_click(other.on_click),
    clicked(other.clicked), moused(other.moused), Text(other) {}
  
  sf::RectangleShape rect;
  action on_click;
  virtual void onDraw() override {
    centerOrigin(text);
    text.setPosition({
      rect.getGlobalBounds().left + rect.getGlobalBounds().width / 2.f,
      rect.getGlobalBounds().top + rect.getGlobalBounds().height / 2.f
    });
    if (!moused) {
      //std::cout << "drawing normal button" << std::endl;
      auto c = rect.getOutlineColor();
      rect.setOutlineColor(rect.getFillColor());
      window.draw(rect);
      rect.setOutlineColor(c);
    }
    else if (clicked) {
      //std::cout << "drawing clicked button" << std::endl;
      auto c = rect.getFillColor();
      rect.setFillColor(rect.getOutlineColor());
      window.draw(rect);
      rect.setFillColor(c);
    }
    else {
      //std::cout << "drawing moused button" << std::endl;
      window.draw(rect);
    }
    Text::onDraw();
  }
  virtual void onMousePress(const sf::Event::MouseButtonEvent& event) override {
    if (event.button != sf::Mouse::Left) return;
    if (rect.getGlobalBounds().contains(window.mapPixelToCoords(sf::Vector2i(event.x, event.y)))) {
      //std::cout << "Button " << std::hex << this << ": mouse pressed" << std::endl;
      clicked = true;
      something_changed = true;
    }
  }
  virtual void onMouseRelease(const sf::Event::MouseButtonEvent& event) override {
    if (event.button != sf::Mouse::Left) return;
    sf::Vector2f local_mouse = window.mapPixelToCoords(sf::Vector2i(event.x, event.y));
    auto b = rect.getGlobalBounds();
    if (b.contains(local_mouse)) {
      std::cout << "Button " << std::hex << this << ": clicked" << std::endl;
      on_click();
      clicked = false;
      something_changed = true;
    }
  }
  virtual void onMouseMove(const sf::Event::MouseMoveEvent& event) override {
    //std::cout << "Button " << std::hex << this << ": mouse moved" << std::endl;
    //std::cout << std::dec << event.x << ' ' << event.y << std::endl;
    sf::Vector2f local_mouse = window.mapPixelToCoords(sf::Vector2i(event.x, event.y));
    //std::cout << local_mouse.x << ' ' << local_mouse.y << std::endl;
    //std::cout << rect.getGlobalBounds().left << ' ' << rect.getGlobalBounds().width + rect.getGlobalBounds().left << std::endl;
    //std::cout << rect.getGlobalBounds().top << ' ' << rect.getGlobalBounds().top + rect.getGlobalBounds().height << std::endl;
    if (rect.getGlobalBounds().contains(local_mouse)) {
      if (!moused) something_changed = true;
      moused = true;
      if (mouse_buttons[sf::Mouse::Left]) {
        if (!clicked) something_changed = true;
        clicked = true;
      }
    }
    else {
      if (moused || clicked) something_changed = true;
      moused = false;
      clicked = false;
    }
  }
protected:
  bool clicked, moused;
};

#endif