=== ECS Mining ===

1. Controls:
------------
  The camera controls are as specified in the spec:
    Up       - speed up camera
    Down     - slow down camera
    left     - turn camera left
    right    - turn camera right
    pageUp   - increase elevation of camera
    pagedown - decrease elevation of camera
    space    - stops all camera motion
  
  The mouse is not used. Other keys:
    P        - Returns camera to vantage point
    H        - Outputs help text to stdout
    Q / ESC  - Quits program
    T        - Starts tour
    E        - Exits tour
    Y        - Returns camera to start view point
    C        - Prints current camera location to stdout (mainly for
               debug but I thought I'd leave it in)

2. Files:
---------
  2.1 Code:
    main.cpp         - Main code file
    utils.cpp        - Shader / BMP loaders
    entity.cpp       - The Camera object
    utils.h          - Required header definitions for utils and entity
    Material.(cpp|h) - Loads a .mtl file
    Model.(cpp|h)    - Loads a .obj file
    Object.(cpp|h)   - Represents an Object in worldspace
    TerribleArray.(cpp|h) - Basic Hashmap of textures and texture files
    TerribleString.(cpp|h) - String hashing function used by TerribleArray
  
  2.2 Models:
    untitled.(obj|mtl)    - World object (the rocky terrain, building, track,
                                crane base, satalitte, stairs and space ship)
    container.(obj|mtl)   - Object File for the container which is loaded into
                                the spaceship
    crane.(obj|mtl)       - The object for the top of the crane
    crane_hook.(obj|mtl)  - The crane hook which moves back and forth
    door_left.(obj|mtl)   - The left door that opens and closes
    door_right.(obj|mtl)  - The right door that opens and closes
    rocks.(obj|mtl)       - The rocks which go along the conveyer belt
    sky.(obj|mtl)         - The skybox
    
  2.3 Shaders:
    main.frag      - The only fragment shader - does the following lightings:
                        + Ambiant
                        + Sun Light
                        + Spectral
                        + Up to six spot lights
    main.vert      - The only vertex shader - calculates eye and spot
                        directions
  
  2.4 Textures:
    Asphalt_New.bmp                - Runway
    authorised_sign.bmp            - Authorised People Only
    Brick_Colored_Blue.bmp         - Wall of building
    Concrete_Aggregate_Smoke.bmp   - Roof of building
    Concrete_Form_4x8.bmp          - Container loading area
    Concrete_Scored_Jointless.bmp  - Top of building's wall
    container.bmp                  - Sides Container
    crystal.bmp                    - The rock / crystal
    danger.bmp                     - The danger "seek airpockets" sign
    Metal_Aluminium_Anodized.bmp   - The drill housing / hand rails
    Metal_Corrogated_Shiney.bmp    - The container doors
    Metal_Embossed.bmp             - The conveyour belt
    Metal_Panel.bmp                - The side of the space ship
    Metal_Rough.bmp                - Lights / Signs / Crane
    Metal_Rusted.bmp               - Top of space ship
    Metal_Seamed.bmp               - Sides of roof door
    Metal_Steel_Textured_White.bmp - Stairs and roof of ship cargo bay
    remember.bmp                   - "Remember!" sign
    signA.bmp                      - "Space Suits must be worn..." sign
    sky.bmp                        - Stary sky without sun
    sky_with_sun.bmp               - Stary sky with sun
    Steel.bmp                      - Side of one of the doors
    Stone_Marble.bmp               - The ground
    welcome.bmp                    - The welcome sign
    Wood_Bamboo_Medium.bmp         - All wooden doors
  
  2.5 Misc:
    screenshot.jpg - A screenshot of the map
    readme.txt     - This file
    credits.txt    - Credits file
    config.lua     - Build stuff

3. How to build:
----------------
  Pretty much just hit compile (with openGL libraries etc of course)
  All files are at the same level

4. Program Overview:
--------------------
  This program defines two main concepts: Models and Objects. Models
  load an .obj file from disk, along with its associated .mtl and .bmp
  files, and represents a model. The model class includes a draw()
  which accepts a transform matrix. Objects are instances of models in
  the world. As such, Objects keep track of time and create a model
  transform matrix for its Model to use. Lights are specified by textures
  with illum=4 in the texture file, their spot point is the calculated
  centre of the object in question, and their direction is given by the
  face's normal. Lights are only on during the "day" and only the 6
  closest in-frame lights are on at a time.

5. Sources:
-----------
  The object loading method is a modified version of a tutorial.
  The file loader for the shaders is a neatened version of the code
  given to us for coursework1


