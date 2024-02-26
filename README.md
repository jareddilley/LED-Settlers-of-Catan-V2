# LED-Settlers-of-Catan-V2
![image](https://github.com/jareddilley/LED-Settlers-of-Catan-V2/blob/main/Media/Catan-V2-Demo-2.gif)

Fully 3D printed 5-6 person LED Settlers of Catan - Fully interactive game board that's fun for the whole family.

This is version two of my LED Settlers of Catan that I design and built from scratch. For those who have played Catan know that setting up the game takes a while. This board removed that with set up in an instant. Along with instant set up, the LEDs add to a great game aesthetic. This is the version 2 of the board. Version 2 is better is every way: plays up to 6 people, more compact, better aesthetics, simpler robust construction. Version 2 also has an ESP32 microcontroller so in the future I can possibly make an app that Bluetooth to the board. This GitHub contains all the code and 3D files if anyone wants to try and build their own. If you are interested in how it was build and want to be entertained at the same time, watch the YouTube video: https://www.youtube.com/watch?v=XWqmaW1D9r8

Below is the Fritzing [Schematics](https://github.com/jareddilley/LED-Settlers-of-Catan-V2/tree/main/Schematics) wiring diagram for anyone looking to build thier own.
![image](https://github.com/jareddilley/LED-Settlers-of-Catan-V2/blob/main/Media/schematic-Catan-V2.PNG)

# Game Modes
![image](https://github.com/jareddilley/LED-Settlers-of-Catan-V2/blob/main/Media/2-Game-modes.gif)

There is a switch to choose between two different game board layouts. The first mode randomly generates the game board, but makes sure no resource tile of the same type are next to each other. The second made also randomly generates the game board, but there are no restrictions and same resource types can be next to each other. The game modes can only be changed before powering up the board. The switch allows people to choose how they want to play, and it also changes what happens when a 7 is rolled. 

# Rolling a 7
![image](https://github.com/jareddilley/LED-Settlers-of-Catan-V2/blob/main/Media/Rolling-a-7.gif)

With this new board, I chose to remove the idea of the robber. In addition to no robber, there is also a new hexagon that replaces one of the deserts. The hexagon is purple and allows you to choose a resource when the number on the spot is rolled. Now when a seven is rolled it changes the location of the desert and the all hex. For the evenly spread out game mode, the desert and the all hex switch places. For the fully random game mode, the desert and the all spot trade places with a random spot on the board. This changes the way the board is played, either setting people back or allowing them to pull forward. This also reduces anger between players since you can't get picked on by the robber. 

# Graphical Displays
![image](https://github.com/jareddilley/LED-Settlers-of-Catan-V2/blob/main/Media/Catan-V2-Graphs.png)

The version 2 upgraded the pre-existing info displays. Version 1 of the board had a list of each resource and how much each has been rolled. Version 2 has two graphical displays. One is a bar char that shows the resources rolled, and the other is a bar chart showing the distribution of the dice rolls. These graphical displays add stats that allow you to see which dice rolls are being favored and which resources are dominating. 

# LED PCBs
![image](https://github.com/jareddilley/LED-Settlers-of-Catan-V2/blob/main/Media/Catan-3D-PCB.PNG)

Building these boards from scratch takes a lot of time. Version 1 used LED strips for the lighting that was very time-consuming to wire up. In this build, I went with designing my own PCBs for the LEDs that allowed for quick and easy assembly. The new PCB are white, which allows them to be LEDs and reflectors in one. I ordered my PCBs from JLCPCB, so if anyone is interested in ordering the same design, I put the zipped folder in the [Schematics](https://github.com/jareddilley/LED-Settlers-of-Catan-V2/tree/main/Schematics) directory. 
