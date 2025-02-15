#include <SDL2/SDL.h>

#include <iostream>
#include <map>
#include <vector>

const int delta_x = 5;
const int delta_y = 5;

struct Point {
  int x, y;
};

int main(int argc, char* argv[]) {
  // Инициализация SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "Не удалось инициализировать SDL: " << SDL_GetError()
              << std::endl;
    return 1;
  }

  // Создание окна
  SDL_Window* window =
      SDL_CreateWindow("Рисование точек", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
  if (!window) {
    std::cerr << "Не удалось создать окно: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }

  // Создание рендерера
  SDL_Renderer* renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer) {
    std::cerr << "Не удалось создать рендерер: " << SDL_GetError() << std::endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  std::vector<Point> points;  // Вектор для хранения точек
  std::map<std::pair<int, int>, size_t> points_index;

  bool running = true;
  size_t index_point = 0;
  bool move_point = false;
  SDL_Event event;

  // Главный цикл приложения
  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = false;
      } else if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (points_index.find(std::make_pair(event.button.x / delta_x,
                                             event.button.y / delta_y)) !=
            points_index.end()) {
            index_point = points_index[std::make_pair(event.button.x / delta_x,
                                      event.button.y / delta_y)];
            move_point = true;

        } else {
          // Добавляем точку в вектор
          points_index[std::make_pair(event.button.x / delta_x,
                                      event.button.y / delta_y)] =
              points.size();
          points.push_back({event.button.x, event.button.y});
        }
      }
    }

    // Очистка экрана
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Черный цвет
    SDL_RenderClear(renderer);

    // Рисуем точки
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Белый цвет
    for (const auto& point : points) {
      SDL_RenderDrawPoint(renderer, point.x, point.y);
    }

    // Обновление экрана
    SDL_RenderPresent(renderer);
  }

  // Освобождение ресурсов
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
