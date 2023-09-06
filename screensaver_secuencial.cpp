#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

struct Circle {
    int x, y;       // Posición del centro del círculo
    int vx, vy;     // Velocidad del círculo en cada eje
    int radius;     // Radio del círculo
    SDL_Color color; // Color del círculo 
};

//metodo para dibujar los circulos
void FillCircle(SDL_Renderer* renderer, int32_t centreX, int32_t centreY, int32_t radius) {
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w; // offset horizontal 
            int dy = radius - h; // offset vertical 
            if ((dx*dx + dy*dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, centreX + dx, centreY + dy);
            }
        }
    }
}


int main(int argc, char** argv) {
    if (argc < 3) {
        printf("Uso: %s <numero de circulos> <numero de movimientos>\n", argv[0]);
        return 1;
    }

    int numCircles = atoi(argv[1]);
    int numMovement  = atoi(argv[2]);

    if (numCircles <= 0) {
        printf("El numero de circulos debe ser mayor a cero.\n");
        return 1;
    }

    if (numMovement <= 0) {
        printf("El numero de movimientos debe ser mayor a cero.\n");
        return 1;
    }
    
    SDL_Init(SDL_INIT_VIDEO);
    // Creacion del screen
    SDL_Window* window = SDL_CreateWindow(
        "Screensaver Secuencial",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
        640, 480, SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    
    srand(time(NULL)); // Inicializar la semilla del generador de números aleatorios

    // Inicializar los círculos
    std::vector<Circle> circles(numCircles);
    for (int i = 0; i < numCircles; ++i) {
        circles[i].x = rand() % 640; // Generar un número aleatorio entre 0 y 640
        circles[i].y = rand() % 480; // Generar un número aleatorio entre 0 y 480
        circles[i].vx = rand() % 21 - 10; // Generar un número aleatorio entre -10 y 10
        circles[i].vy = rand() % 21 - 10; // Generar un número aleatorio entre -10 y 10
        circles[i].radius = 10; // El radio del círculo 
        circles[i].color = { rand() % 256, rand() % 256, rand() % 255, 255 }; // Generar color del circulo que no sea blanco
    }

    Uint64 frameStart; 
    int frameTime; 
    int frameCount = 0; //contador para que pueda desplegar despues de cada tiempo
    Uint64 totalFrameTime = 0;
    int totalFrames = 0;
    int moveCounter = 0;
    bool shouldStop = false;

    // Bucle principal
    while (true) {
        frameStart = SDL_GetPerformanceCounter(); //comenzar a el contador para los fps
   
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // fondo de color blanco
        SDL_RenderClear(renderer); // realizar limpieza de la pantalla.

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

        }
        // Deteccion de cirlculos
        for (int i = 0; i < numCircles; ++i) {
            for (int j = i + 1; j < numCircles; ++j) {
                int dx = circles[i].x - circles[j].x;
                int dy = circles[i].y - circles[j].y;
                int distanceSquared = dx * dx + dy * dy;
                int radiusSum = circles[i].radius + circles[j].radius;
                // Los círculos están colisionando
                if (distanceSquared < radiusSum * radiusSum) {
                    /// Mover los círculos fuera de la colisión
                    int overlap = radiusSum - sqrt(distanceSquared);
                    int angle = atan2(dy, dx);
                    circles[i].x += cos(angle) * overlap / 2;
                    circles[i].y += sin(angle) * overlap / 2;
                    circles[j].x -= cos(angle) * overlap / 2;
                    circles[j].y -= sin(angle) * overlap / 2;

                    // Generar un ángulo aleatorio
                    float angle1 = (float)(rand() % 360) * M_PI / 180.0f;
                    float angle2 = (float)(rand() % 360) * M_PI / 180.0f;

                    // Calcular las nuevas velocidades segun su nuevo angulo a dirigirse
                    circles[i].vx = cos(angle1) * 10;
                    circles[i].vy = sin(angle1) * 10;
                    circles[j].vx = cos(angle2) * 10;
                    circles[j].vy = sin(angle2) * 10;
                }
            }
        }

        for (Circle& circle : circles) {
            // Dibujar el círculo
            SDL_SetRenderDrawColor(renderer, circle.color.r, circle.color.g, circle.color.b, circle.color.a);
            FillCircle(renderer, circle.x, circle.y, circle.radius);
        }

        SDL_RenderPresent(renderer); // Presentar los cambios en el renderer

        Uint64 end = SDL_GetPerformanceCounter();
        // Calcular los frames
        float elapsedSeconds = (end - frameStart) / (float)SDL_GetPerformanceFrequency();
        float fps = 1.0f / elapsedSeconds;
        
        // Mostrar los FPS cada 60 cuadros
        if (frameCount++ % 60 == 0) {
            printf("FPS: %.2f\n", fps); // Imprimir los FPS en la terminal
        }

        totalFrameTime += end - frameStart;
        totalFrames++;

        if (moveCounter >= numMovement) {
            shouldStop = true;
            break;
        }
    
        if (shouldStop) {
            break;
        }
        moveCounter++;

    }
    float averageFrameTime = totalFrameTime / (float)totalFrames / SDL_GetPerformanceFrequency();
    float averageFPS = 1.0f / averageFrameTime;
    printf("FPS promedio secuencial: %.2f\n", averageFPS);


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
