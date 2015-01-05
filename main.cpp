#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <cmath>
using namespace std;
using namespace sf;

int main(int argc, char* argv[]) {
  Texture tex;
  tex.loadFromFile("img.jpg");
  
  Vector2i scSize = Vector2i(tex.getSize().x, tex.getSize().y);

  Sprite sprite;
  sprite.setTexture(tex);
  RenderWindow window(VideoMode(scSize.x, scSize.y), "RGB", Style::Titlebar | Style::Close);
  
  RectangleShape rect;
  rect.setFillColor(Color::Green);  

  Vector2f startPos;
  
  while (window.isOpen()) {
    
    Event event;
    while (window.pollEvent(event)) {
      if (event.type == Event::Closed)  window.close();
      else if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        startPos.x = event.mouseButton.x;
        startPos.y = event.mouseButton.y;
        rect.setPosition(startPos);
        rect.setSize(Vector2f());
      }
      else if (event.type == Event::MouseMoved && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        int x = min(event.mouseMove.x, scSize.x);
        int y = min(event.mouseMove.y, scSize.y);
        Vector2f offset;
        int absX = abs(startPos.x - x);
        int absY = abs(startPos.y - y); 
        if (absX > absY) offset = Vector2f(absY, absY);
        else offset = Vector2f(absX, absX);
        if (startPos.x > x) offset.x *= -1;
        if (startPos.y > y) offset.y *= -1;
        rect.setSize(offset);
      }
    }


    window.clear();
    window.draw(sprite);
    window.draw(rect);
    window.display();
  }
}
