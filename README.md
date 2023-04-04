# Visual Particle Simulator
This program recreates interactions between simple particles(red) and black holes(green). They interact via gravitational attraction, and change their trajectories accordingly. <br>
The small, blue arrows(generators) randomly generate particles with an initial velocity and radius. <br>
As time progresses, the various gravitational interactions change the velocity of the particles, causing them to speed up(bright red) or slow down(dark red). <br>
Once a particle touches the center of a black hole, it gets deleted from the simulation.<br>
[Run the code!](#running-the-code)

![overview](/images/overview.gif)
<br>
The main files used to generate the systems are:

- [sim.c](/source/sim.c):<br>
  This code interprets user command line input and calls the appropriate modules to process the data depending on the type of execution mode(Simulation by default, more details in [Debug Modes](#debug-modes)). It also reads input data from a `load_file.txt` to run a preset simulation. This includes functions for drawing and updating particle, generators, and black hole positions, as well as functions for selecting, releasing, and deleting particles. It can also write output data to a `save_file.txt`, and calculate viewport values.
- [particle.c](/source/particle.c): <br>
  This creates and manipulates particles, which are represented as linked-list structures containing data such as position, velocity, and radius. The code includes functions for generating and deleting particles, testing and storing particle data, and calculating forces between particles, as well as a function for calculating the acceleration of particles given their force and radius. It also calculates the force between a particle and a black hole given their distance. The simulation takes place in a two-dimensional space, and particles interact with each other through attractive forces.
- [generator.c](/source/generator.c): <br>
  This reads, stores and tests the data for generators interpreted by  `sim.c`. It stores data such as the radius of particles generated, their positions, and their initial velocities. They are stored in a linked list, and can access, insert, and delete generators. It can draw the generators and save them.  Upon detecting an error in the generator data, an error message is shown, followed by the termination of the program. When visualised, they are indicated by a small blue arrow, pointing in the direction they generate particles.
- [black_hole.c](/source/black_hole.c): <br>
  Similar to `generator.c`, this stores and tests black hole data. It defines a `struct` and a linked list to store them. This includes functions to read, save, draw and calculate the max and min positions of the black holes. It also includes functions to insert, delete and empty the linked list. If an error is detected in the black hole data, an error message is shown, followed by the termination of the program. Visualised, they are green circles. The dotted outer circle is the _event horizon_, where particles cannot escape.

<br>

[main.cpp](/source/main.cpp) is responsible for setting up the UI using the `glut`/`glui` libraries, as well as handling the user prompts from the command line/UI(sending them to the appropriate modules). 

**NOTE:** _This work was partially written in french. Some words in the code/UI use both languages. It should still be straightforward._

The overall architecture/dependency graph of the modules is as follows:
  ![architecture](/images/architecture.jpg)

The remaining modules,  [vector.c](/source/vector.c), [error.c](/source/error.c), and [graphic.c](/source/graphic.c), are responsible for setting up the elementary components for mathematical/graphical calculations, and giving out the appropriate error messages.
# Running the code

## 1. System requirements & Package dependencies
This code is intended to be run on Linux systems, in my case I used Ubuntu 20.04.<br>

**Note:** _In the future, some of these libraries may cease support, so It wont be installable from apt. Find an archive to download the appropriate `.deb` files, and install them with `dpkg`_

Out of the box, Ubuntu should already have a lot of the necessary libraries, but in particular, two of them are obsolete and require manual installation(`glui`, replaced with `glui2`)
<br>
Before downloading the repo, run the following bash commands:
```bash
$ sudo apt-get update
$ sudo apt-get install libglu1-mesa-dev freeglut3-dev mesa-common-dev
```
This will install the `glut` library, which is responsible for under-the-hood UI management on older systems. <br>

Then we will download more older libraries:

```bash
$ sudo apt-get libxmu-dev
$ sudo apt-get libxi-dev
```
These also help with under-the-hood management for older graphical elements, and are required.
<br><br>
Finally, go to the [debian archives]( https://debian-archive.at.mirror.anexia.com/debian/pool/main/g/glui/), and download [libglui2c2_2.36-4](libglui2c2_2.36-4_amd64.deb ) and [libglui-dev_2.36-4](libglui-dev_2.36-4_amd64.deb) (in my case I have an `amd64` architecture, so you can choose the appropriate one for your system).<br>
Once in your download folder, open a terminal and run the commands:
```bash
$ sudo dpkg -i libglui-dev_2.36-4_amd64.deb
$ sudo dpkg -i libglui2c2_2.36-4_amd64.deb
```
With different endings depending on your download & system architecture(`amd64`/`armel`/`i386`/etc...)
<br><br>
When finished, your system has all the necessary packages. If your system still complains when you run the program, try installing the missing packages with `apt` or look in other debian package archives.

## 2. Compiling the repo 
Once the repo is downloaded, extract it where you want and open a terminal in the directory. Make sure you are in the directory with the top-level makefile.<br>
Simply type `make`, and you should see similar output:
![good_make](/images/make_good.JPG)
`simulation.x` should appear in the same directory, make sure you enable permissions to execute it as a program.
<br>
To make sure it works, type `./simulation.x`, and the UI should appear. Type `P1.txt` in the `filename` field, press `load`, and press `start`. The simulation should begin.

## 3. Using the UI
Relatively simple and self descriptive. To run a simulation, it must first be loaded in the `load` field. By default, there are four `load files`: `P1.txt`, `P2.txt`, `P3.txt`, and `P4.txt`. Click `load` and `start` to watch the system evolve.<br>
Click `stop` to freeze the simulation, and `step` to watch the incremental evolutions.<br>
If you are satisfied with a given state and want to save it for later, enter a name in the `save` field and click save. A new `save file` will appear in the `load_files` directory. <br>

![ui](/images/ui.JPG)

In the information window, we keep track of the number of particles/generators/black holes currently present in the simulation. 

## 4. Custom simulations
Creating a custom `load file` is quite simple. Make sure its a `.txt` file in the `load_files` directory. For example, consider the `load file` of [P1.txt](/load_files/P1.txt):

![custom_sim](/images/custom_sim.JPG)

Anything starting with `#` is ignored.
The data for the black holes, generators and particles is separated with the `FIN_LISTE` indicator.<br>
The first data read pertains to the generators, followed by black holes, and last by particles. <br>
A template `load file` could be the following
```bash
#Template save file

n_generators # number of generators we want

r_generated x_gen y_gen x_vel y_vel # data for a single generator

# r_generated = radius of generated particles
# x_gen y_gen = x/y placement of the generator in the viewport
# x_vel y_vel = initial x/y velocity of generated particles. 
# In the visualisation, the arrowhead of the generator will point
# in the direction of the initial velocities
...
FIN_LISTE

n_black_holes # number of black holes we want

x_hole y_hole # data for a single black hole

#x_black y_black = x/y center coordinated of the black hole 
...
FIN_LISTE

n_particles # number of particles we want

r_particle x_part y_part x_vel y_vel # data for a single particle
# r_particle = radius of the particle
# x_gen y_gen = x/y center of the particle in the viewport
# x_vel y_vel = initial x/y velocity of the particle
...
FIN_LISTE
```
If any mistakes are made when you write your own `load file`, the error messages should catch it and point it out.

# Other interesting leftovers

## Debug modes
During development, several modes where developed to debug the project at various steps. These are `Error`, `Force`, `Graphic`, `Integration`, and `Simulation`. These are exemplified in the following image:

![debug](/images/debug_modes.JPG)
## Error check files







