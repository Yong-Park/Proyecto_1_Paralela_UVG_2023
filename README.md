# Proyecto 1 
# Computacion Paralela
# Universidad del Valle de Guatemala

### Integrantes:
- Yong Bum Park
- Oscar Fernando López Barrios
- Pedro Pablo Arriola Jiménez

### Instalacion de la libreria en Linux
```sudo apt install libsdl2-dev```
### Compilación del código
```g++ -o screensaver_secuencial screensaver_secuencial.cpp $(sdl2-config--cflags--libs)```

```g++ -o screensaver_paralelo screensaver_paralelo.cpp $(sdl2-config--cflags--libs) -fopenmp```

  ### Ejecutar el programa compilado
  ```./screensaver_secuencial <numero_de_circulos> <numero_de_iteraciones>```
