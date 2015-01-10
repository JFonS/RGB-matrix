#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <cmath>
using namespace std;
using namespace sf;

void imageRegion(string path) {
  const int cellSize = 40; //Mida de la cuadricula a la pantalla RGB
  Texture tex;
  tex.loadFromFile(path);

  Image img;
  img.loadFromFile(path);

  Vector2i imgSize = Vector2i(tex.getSize().x, tex.getSize().y);

  Sprite sprite;
  sprite.setTexture(tex);
  
  RenderWindow windowImg(VideoMode(imgSize.x, imgSize.y), path, Style::Titlebar | Style::Close);
  RenderWindow windowResult(VideoMode(cellSize * 8, cellSize * 8), "RGB Matrix", Style::Titlebar | Style::Close);
  
  RectangleShape select;
  select.setFillColor(Color(0,255,0,50));
  select.setOutlineThickness(1);
  select.setOutlineColor(Color::Green);
  
  RectangleShape cell;
  cell.setOutlineThickness(1);
  cell.setOutlineColor(Color::Black);
  cell.setSize(Vector2f(cellSize,cellSize));

  Color result[8][8];
  
  Vector2f startPos(-1,-1);

  while (windowImg.isOpen()) {
    Event event;
    while (windowImg.pollEvent(event)) {
      if (event.type == Event::Closed or (event.type == Event::KeyPressed and event.key.code == Keyboard::Escape))  {
        windowImg.close();

      } else if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        startPos.x = event.mouseButton.x;
        startPos.y = event.mouseButton.y;
        select.setPosition(startPos);
        select.setSize(Vector2f());

      } else if (event.type == Event::MouseMoved && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

        int x = min(event.mouseMove.x, imgSize.x);
        int y = min(event.mouseMove.y, imgSize.y);
        Vector2f offset;
        int absX = abs(startPos.x - x);
        int absY = abs(startPos.y - y); 
        if (absX > absY) offset = Vector2f(absY, absY);
        else offset = Vector2f(absX, absX);
        if (startPos.x > x) offset.x *= -1;
        if (startPos.y > y) offset.y *= -1;
        select.setSize(offset);
      }

      int regionSize = select.getSize().x;
      int cellSize = regionSize/8;
      if (cellSize != 0) {
        //if (cellSize < 0) cellSize *= -1;
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
          }
        }
      }
    }

    windowImg.clear();
    windowResult.clear();

    windowImg.draw(sprite);
    windowImg.draw(select);

    for (int i = 0; i < 8; ++i) {
      for (int j = 0; j < 8; ++j) {
        cell.setFillColor(result[i][j]);
        cell.setPosition(i*cellSize, j*cellSize);
        windowResult.draw(cell);
      }
    }

    windowImg.display();
    windowResult.display();
  }
  windowResult.close();
}


int main(int argc, char* argv[]) {
  imageRegion("img.jpg");
}
