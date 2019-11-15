# Graphics
## *Texturas
- *Agregar el TextureManager
- *Leer texturas de archivo y cargarlas
- *Modificar plyLoader para leer uv coor
- ?Importar modelos con texturas
- *Agregar texturasa a cGameObject
- *Modificar shader para tener textureSamplers y usar uv coords
- *Modificar Draw de GameObject para cargar la textura al sampler e inicializar uv coords
## *Transparencia
- *Agregar alpha transparency al gameObject
- *Modificar Draw de escena
  - *Separar los gameObjects de otros items
  - *Separar los transparentes de opacos
  - *Pintar los opacos
  - *Ordenar los transparentes por distancia a la camara
  - *Pintar los transparentes (pasar transparencia al shader)
## *SkyBox
- Cargar un skybox
  - *Agarrar una imagen
  - *Modificar el fragmentShader
    - Agregar un cubeSampler al fragmentShader
    - Agregar una bandera de skybox al fragmen
    - Colorear el objeto obteniendo los colores del skybox con las normales
  - *Cargarla con el TextureManager
  - Asignarla a una texture unit especial y asociarlo con el cubeSampler
  - Pintarlo
    - Desactivar el depth buffer y la escritura al mismo
    - Turn off face culling
    - Pintar la esfera en la cámara
    - Reactivar el depth buffer
    - Turn on face culling
  - Agregarlo al archivo de especificacion de escena
    - Si trae "Skybox" activar el skybox
    - Leer la locación de las texturas
    - Volver a escribirlo porque ctm
## Project
- *Explosiones pegadas (transparencias multiples)
- *Mezclar fuego y humo (solo en la nave)
- *Agujerear la nave con una textura
- *Agregar debbugging
  - SceneEditor. Agregar rotacion de luces?? 
- 4 shaders
  1. Textura
  2. Color plano
  3. Sin dibujar
  4. Textura con hoyos
- ? Mas luces
# Physics
## AABBs
- Buscar modelo de escena
- Implementar objeto AABB
- Inicializar cuadrados del mundo segun medidas del modelo de escena
- Busqueda de AABB segun una posicion
- Debug y pruebas
- Incluir colision (por gameObject??) unicamente en el AABB
# Engine
## iCommand
- Interfaz
- MoveTo
- RotateTo
- ScaleTo
- Serial
- Parallel
## Leer guion
## Bonus
- FollowCurve (porque no tengo ni idea)

# Pendientes
- Incluir navigation camera de Feeney
- Hacer la rotacion de luces mas como la rotacion de camara
- Poder cambiar entre camara de edicion y camara de navegacion