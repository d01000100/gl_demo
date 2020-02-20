# Stencil

3 partes de render

1. Escena externa
  - enable depth test
  - enable depth writing
  - disable stencil test
  - enable color buffer
2. Portal(es)
  - clear stencil
  - enable stencil test
  - define test as fill in numbers
  - disable color buffer
  - disable writing to depth buffer
3. Escena interna
  - enable writing to depth buffer
  - clearing depth buffer (inside scene draws over inside of the box)
  - enable color buffer
  - define test as "draw only on pixels with values"
4. Reset things (maybe necessary??)
  - Disable stencil
  - Enable writhing to depth buffer
  - Enable color buffer

Ideas de implementacion:

- Poner la propiedad en la escena:
  Poner que parte del pipeline del stencil eres (if any), por escena.

  * Se parametriza el uso del stencil
  - Hay que agregar mas a leer del json

- Una funcion que reciba tres escenas y dibuje:
  1. La primera como la escena externa
  2. La segunda como el portal del stencil
  3. La tercera como la escena interna

  * Es sencillo y usa el SceneManager como ya esta
  - El armado del stencil esta en codigo y no parametrizado
  * Aunque no este parametrizado per se, de todas maneras la dibujada de escenas y las camaras tampoco lo esta??



# Efectos en textura deferida
- Al momento de dibujar el objeto que usa la textura

# Reflection y refraction