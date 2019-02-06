# MVD_ToolScripting_Finalwork
## 1. Design your own level (Whitebox)
## 2. Export scene information into the engine using the tools we created.
## 3. Create a new component (movable platform e.g).
### New component Faller
Faller component added.  
If a collision is detected, wait 10sec and fall with given speed.  
Parameter: speed  
### Add new component button
Add new component: Rotator, Tag, Faller
## 4. Complete the renderer debug menu (material parameters support)
Now you can change the ambient vector, diffuse vector and specular vector.
## 5. Add script logic on console editor
Script “sayhello”: print “Hello!!!” on console.
  
Script “moveObject”:  
Parameters: object name, translate x, translate y, translate z  
Example: moveObject Lvl01Box002 10 0 4  
  
Script “rotateObject”:  
Parameters: object name, angle in degrees, axis (x, y or z)  
Example: rotateObject Lvl01Box002 90 y  
## 6. Finish loading json in max (scene changes between engine/max)
WIP...
## 7. Add a button to delete entity
Button added to delete entities.  
If you try to delete an entity with a camera it will show a message on the console.
## Extra
Added the name of the entity on the inspector.
