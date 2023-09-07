# Proyecto 1 
# Computacion Paralela
# Universidad del Valle de Guatemala

### Integrantes:
- Yong Bum Park
- Oscar Fernando Lopez Barrios
- Pedro Pablo Arriola Jimenez

### libreria a descargar
- sudo apt install libsdl2-dev
### compilacion
- g++ -o screensaver_secuencial screensaver_secuencial.cpp $(sdl2-config--cflags--libs)
- g++ -o screensaver_paralelo screensaver_paralelo.cpp $(sdl2-config--cflags--libs)

  ### correr el programa
  - ./screensaver_secuencial <numero_de_circulos> <numero_de_iteraciones>
