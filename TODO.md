# Animation

## Cargar un modelo

1. Add a cSimpleAssimpSkinnedMesh to the gameObject (or the skinned object)
2. To load the mesh, call cSimpleAssimpSkinnedMesh->LoadMeshFromFile for the object that has the skin mesh.
3. get a sModelDrawInfo from CreateModelDrawInfoObjectFromCurrentModel from the cSimpleAssimpSkinnedMesh
4. Use that sModelDrawInfo and load it into the VAO with pTheVAOManager -> LoadModelDrawInfoIntoVAO
5. Load the animations with the LoadMeshAnimations of the cSimpleAssimpSkinnedMesh

- Introducir assimp
  - Importarlo
  - Hacer el translator hacia el DrawInfo
  - Leer los modelos del json
- Leer animacion
  - Especificarlas en el json
  - cargarlas con el helper
- Reproducir animacion
  - Empezar y detenerla
  - Cambiar de una animacion a otra
- Queue de animaciones