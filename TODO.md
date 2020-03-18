1. * Integrar código de BMPLoader
   - * Función que regrese el color según una coordenada
   - * Tamaño de la imágen
   - * Coordenada del origen
   - * Coordenada del recurso
   - * Coordenada de la meta
2. Pathfinding
   1. * Función que determina si una coordenada es "valida"
      - Dentro de límites (tamaño de la imagen)
      - No es una pared
   2. * Función que regresa los "vecinos" de una coordenada
      Sólo los válidos
   3. * Función que calcula la distancia manhattan de una coordenada a otra
   4. * Función que calcula el "peso" de una "arista"
      Dadas dos pares de coordenadas "vecinas" considera:
      - Si son verticales, horizontales o en diagonal
      - El color de la coordenada "objetivo"
   5. Dijkstra
      - Devolver la ruta en un vector de coordenadas
   6. A*
      - Devolver la ruta en un vector de coordenadas
3. Game & Rendering
   - Escala "global" en la que todo se pinta
   1. Funcion que pasa de coordenadas bmp a vec3 según la escala
   2. Pintar el mapa como una textura en un quad con la escala
   3. Pintar el personaje en el origen
   4. Pintar un conejo en el recurso
   5. Pintar una nave en la meta
   6. Mover un objeto a través de una lista de coordenadas
   7. "Máquina de estados" del personaje