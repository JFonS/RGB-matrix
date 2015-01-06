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
  
  Image img;
  img.loadFromFile("img.jpg");

  Vector2i scSize = Vector2i(tex.getSize().x, tex.getSize().y);

  Sprite sprite;
  sprite.setTexture(tex);
  RenderWindow window(VideoMode(scSize.x, scSize.y), "RGB", Style::Titlebar | Style::Close);
  
  RectangleShape rect;
  rect.setFillColor(Color(0,255,0,50));
  rect.setOutlineThickness(1);
  rect.setOutlineColor(Color::Green);
  
  Color result[8][8];
  
  Vector2f startPos;
  bool done  = false; 
  while (window.isOpen()) {
    
    Event event;
    while (window.pollEvent(event)) {
      if (event.type == Event::Closed or (event.type == Event::KeyPressed and event.key.code == Keyboard::Escape))  window.close();
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
      } else if (event.type == Event::KeyPressed and event.key.code == Keyboard::Return and not done) {
        int regionSize = rect.getSize().x;
        int cellSize = regionSize/8;

        for (int i = 0; i < 8; ++i) {
          for (int j = 0; j < 8; ++j) {
            int r, g, b;
            r = g = b = 0;
            for (int k = 0; k < cellSize; ++k) {
              for (int l = 0; l < cellSize; ++l) {
                Color c = img.getPixel(startPos.x + i*cellSize + k, startPos.y + j*cellSize + l);
                r += c.r * c.a/255;
                g += c.g * c.a/255;
                b += c.b * c.a/255;
              }
            }
            int n = cellSize * cellSize;
            result[i][j] = Color(r/n, g/n, b/n, 255);
            cout << "(" << result[i][j].r << "," << result[i][j].g << "," << result[i][j].b << ")   ";
          }
          cout << endl;
        }
        done = true;
      } 

    window.clear();
    if (not done) {
      window.draw(sprite);
      window.draw(rect);
    } else {
      int size = 40;
      rect.setOutlineColor(Color::Black);
      rect.setSize(Vector2f(size,size));
      for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
          rect.setFillColor(result[i][j]);
          rect.setPosition(i*size, j*size);
          window.draw(rect);
        }
      }
    }
    window.display();
  }
  }
}
