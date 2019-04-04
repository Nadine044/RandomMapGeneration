# RandomMapGeneration
I am [Nadine Gutiérrez](https://github.com/Nadine044), student of the [Bachelor’s Degree in Video Games by UPC at CITM](https://www.citm.upc.edu/). This content is generated for the second year’s subject Project 2, under supervision of lecturer [Ricard Pillosu](https://es.linkedin.com/in/ricardpillosu).

## About the project
This project contains a research about how to generate isometric random maps in C++. There are two map types implemented in the code: dungeon and squared rooms.

The project goals are generate an algorithm similar than this one:

<img src="https://github.com/Nadine044/RandomMapGeneration/blob/master/docs/media/Algoritm.gif"/>

We will use a simple ordering rooms code depending on the X position of them. In the gif is it used a Physics engine to separate them.

## Map Types
Depending on the map type that is played, the randomness algorithms will change. In this project it is focused a 2d isometric map algorithm.
We can find maps and worlds just like those: 

### 2d map

![](https://github.com/Nadine044/RandomMapGeneration/blob/master/docs/media/super-mario-bros_2.jpg)
![](https://github.com/Nadine044/RandomMapGeneration/blob/master/docs/media/sonic-sega-900x600.jpg)

### 2d with perspective

![](https://github.com/Nadine044/RandomMapGeneration/blob/master/docs/media/megamanstarforce_12a.jpg)

### 3d map:

![](https://github.com/Nadine044/RandomMapGeneration/blob/master/docs/media/Zelda-Breath-of-the-Wild.jpg)

### Virtual World:

![](https://github.com/Nadine044/RandomMapGeneration/blob/master/docs/media/Beat-Saber-BennyDaBeast.jpg)

## First algorithms implementations
Some games used pseudorandom number generators were often used with predefined seed values in order to generate very large game worlds that appeared to be premade. The Sentinel supposedly to had 10.000 different levels stored in only 48 and 64 kilobytes. An extreme case was Elite, which was originally planned to contain an approximately 282 trillion galaxies with 256 solar systems each. However, the publisher was afraid that such a gigantic universe would cause disbelief in players, and eight of these galaxies were chosen for the final version. Other notable early examples include the 1985 game Rescue on Fractalus that used fractals to procedurally create, in real time, the craggy mountains of an alien planet.

![](https://github.com/Nadine044/RandomMapGeneration/blob/master/docs/media/149850.gif)

![](https://github.com/Nadine044/RandomMapGeneration/blob/master/docs/media/149981.gif)

Akalabeth: World of doom (1980)


![](https://github.com/Nadine044/RandomMapGeneration/blob/master/docs/media/Thesentinelabsorb.GIF)

The Sentinel (1986)


![](https://github.com/Nadine044/RandomMapGeneration/blob/master/docs/media/Elite_Animation.gif)

Elite (1984)


![](https://github.com/Nadine044/RandomMapGeneration/blob/master/docs/media/Fractalus_anim_small.gif)

Rescue on Fractalus (1985)


## Games with random maps

## Code Implementation
In this project it is shown how to generate an algorithm to generate those two map types.
### Grid creation
To generate the world space for the level we need to create a grid. In the code we have a struct for the map nodes (tiles) and we store every node into the nodes vector.
We will use the Get(x,y) function.

### Start de algorithm
In this point it starts de floor generation and stablish the grid limits by executing the algorithm.
The user stablish an iteratons number to get a terrain size aproximation. In each cycle it checks the nodes neighbours, which they have a 25% chance to be the next current node on the next cycle move.
When the algorithm finds a new node (which wasn't visited before), the iteration numer increases.

### Walls generation
Now that we have a terrain to walk in, we need to put walls to represent de level limits.
The algorithm will check the visited nodes boundaries, and if it is the case that a node have a different node neghbour, it will change it for a wall.

### Rooms generation
Depending on the user size preferences, the algorithm picks a random node on the grid and it checks if it has space enoguh to generate a room. It will generate as many rooms as you specified.
When all the rooms are created they will be ordered by their X position (left to right).

### Connecting rooms
When all the rooms are stablished we will use A* (pathfinding algorithm) to generate an entrance/exit of the room, and then connect this one with the next ordered room.

## TODOs Solutions
### TODO 1: Load elements through xml

We have to load all the elements that we stored in the mapData.xml. We will use the mapData variable that pertains to the struct RoomsMapInfo.

![](https://github.com/Nadine044/RandomMapGeneration/blob/master/docs/media/TODOs/1.PNG)

### TODO 2: Generate corridors between rooms using A* (pathfinding algorithm)

To generate the corridors between all our stablished rooms we use A*. The code checks a random valid point from the room and creates a pathfinding to the next valid point of the next room. 

The rooms are ordered by the X position component.

![](https://github.com/Nadine044/RandomMapGeneration/blob/master/docs/media/TODOs/2.PNG)

### TODO 3: Walls generation

Finally we check the current node boundaries, and if there is a WATER node it has to be changed by a WALL node.

![](https://github.com/Nadine044/RandomMapGeneration/blob/master/docs/media/TODOs/3.PNG)

## References
https://es.gizmodo.com/asi-se-generan-las-mazmorras-aleatorias-en-un-videojueg-1729131335

https://geoinnova.org/blog-territorio/mapas-de-videojuegos/

http://www.fsgamer.com/generacion-procedimental-el-futuro-de-los-videojuegos-20150130.html
