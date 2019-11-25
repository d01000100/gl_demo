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
- *Cargar un skybox
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
## *Project
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
# *Physics
## AABBs
- *Buscar modelo de escena: Battlestar galactica
  (-195,-107,-516) -> (232, 53, 531): 427, 161, 1047 ~ 500, 200, 1100
- *Implementar objeto AABB: mins y size 
- *Inicializar cuadrados del mundo segun medidas del modelo de escena
  11 cajas de 100x100x100 cada lado = 1331 cajas
- *Busqueda de AABB segun una posicion
- Meter triangulos
  - Subdividir triángulos
## *Puntos de colision
- *Los GameObjects pueden tener subobjetos
- *Mover game object mueve los objetos
- ? *Escalar game object mueve los objetos
- ? *Rotar game object mueve los objetos
## *Mover nave
- *Poner cámara detrás de nave y que la siga (estáticamente)
- *Mover nave con fuerzas.
## Colisionar nave
- Obtener AABB que contiene a la nave
- Detectar colisiones en cada uno de los objetos de colisión de la nave
- Responder a la colisión:
  - Detener la nave
  - !? Empujarla hacia afuera de la pared
# Engine
## Pre requisitos
  - *Aplicar quaterniones en game objects
    - Cambiar propiedad
    - Setter(euler), set(quad), quat getOrientationQuad, vec3 getOrientationEuler, update(quat), update(euler)
    - Update transformMatrix (using the quat)
    - Draw correctly
  - *Aplicar cuaterniones en luces
  - Hacer la camara un gameItem
    - Cambiar posiscion
    - Cambiar orientacion
## iCommand
  - *Interfaz
    - bool isDone
    - void Update
    - int getID
    - string getTag
    - string getName
    - ? void Init

  - *MoveTo
    - targetPos, time, easeInFraction, easeOutFraction
  - *Rotate
    - offsetRotation, time, ?easeInFraction, ?easeOutFraction
  - *LookAt
    - targetPos, time, easeInDur, easeOutDur
  - ScaleTo
    - targetScale, time, easeInFraction, easeOutFraction

  - FollowObject
    - targetObject, maxVel, innerRad, outerRad
  - *Serial
  - *Parallel
  - *Follow
  - RideCurve
    - targetPos, curveyPoints ???
  - Trigger
    - AABB (mins, maxs), triggerCommand
## Leer guion
- Integrar Lua
- Factory con Lua

# Pendientes
- Incluir navigation camera de Feeney
- Hacer la rotacion de luces mas como la rotacion de camara
- Poder cambiar entre camara de edicion y camara de navegacion