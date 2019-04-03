# RandomMapGeneration
I am [Nadine Gutiérrez](https://github.com/Nadine044), student of the [Bachelor’s Degree in Video Games by UPC at CITM](https://www.citm.upc.edu/). This content is generated for the second year’s subject Project 2, under supervision of lecturer [Ricard Pillosu](https://es.linkedin.com/in/ricardpillosu).

## About the project
This project contains a research about how to generate isometric random maps in C++. There are two map types implemented in the code: dungeon and squared rooms.

## Map Types
Depending on the map type that is played, the randomness algorithms will change. In this project is it focused a 2d isometric map algorithm.
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

## References
https://es.gizmodo.com/asi-se-generan-las-mazmorras-aleatorias-en-un-videojueg-1729131335

https://geoinnova.org/blog-territorio/mapas-de-videojuegos/

http://www.fsgamer.com/generacion-procedimental-el-futuro-de-los-videojuegos-20150130.html
