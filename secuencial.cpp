#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

struct Circle {
    int x, y;       // Posición del centro del círculo
    int vx, vy;     // Velocidad del círculo en cada eje
    int radius;     // Radio del círculo
};

// Función para dibujar un círculo
void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius){
   const int32_t diameter = (radius * 2);

   int32_t x = (radius - 1);
   int32_t y = 0;
   int32_t tx = 1;
   int32_t ty = 1;
   int32_t error = (tx - diameter);

   while (x >= y)
   {
      SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

      if (error <= 0)
      {
         ++y;
         error += ty;
         ty += 2;
      }

      if (error > 0)
      {
         --x;
         tx += 2;
         error += (tx - diameter);
      }
   }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Uso: %s <numero de circulos>\n", argv[0]);
        return 1;
    }

    int numCircles = atoi(argv[1]);

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "Screensaver",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
        640, 480, SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Establecer el color de dibujo en rojo

    srand(time(NULL)); // Inicializar la semilla del generador de números aleatorios

    // Inicializar los círculos
    std::vector<Circle> circles(numCircles);
    for (int i = 0; i < numCircles; ++i) {
        circles[i].x = rand() % 640; // Generar un número aleatorio entre 0 y 640
        circles[i].y = rand() % 480; // Generar un número aleatorio entre 0 y 480
        circles[i].vx = rand() % 21 - 10; // Generar un número aleatorio entre -10 y 10
        circles[i].vy = rand() % 21 - 10; // Generar un número aleatorio entre -10 y 10
        circles[i].radius = 50; // El radio del círculo
    }

    // Bucle principal
    while (true) {
        // Limpiar la pantalla
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        for (Circle& circle : circles) {
            // Actualizar la posición del círculo
            circle.x += circle.vx;
            circle.y += circle.vy;

            // Verificar si el círculo ha alcanzado el límite de la ventana
            if (circle.x - circle.radius < 0 || circle.x + circle.radius > 640) {
                circle.vx = -circle.vx; // Invertir la velocidad en el eje x
            }
            if (circle.y - circle.radius < 0 || circle.y + circle.radius > 480) {
                circle.vy = -circle.vy; // Invertir la velocidad en el eje y
            }

            // Dibujar el círculo
            DrawCircle(renderer, circle.x, circle.y, circle.radius);
        }

        SDL_RenderPresent(renderer); // Presentar los cambios en el renderer
        SDL_Delay(1000 / 60); // Esperar 1/60 de segundo
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
