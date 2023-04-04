# Visual Particle Simulator
This program recreates interactions between simple particles(red) and black holes(green). They interact via gravitational attraction, and change their trajectories accordingly. <br>
The small, blue arrows(generators) randomly generate particles with an initial velocity and radius. <br>
As time progresses, the various gravitational interactions change the velocity of the particles, causing them to speed up(bright red) or slow down(dark red). <br>
Once a particle touches the center of a black hole, it gets deleted from the simulation.
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

## 1. System requirements & Package dependancies

## 2. Compiling the repo 

## 3. Using the UI
![ui](/images/ui.JPG)

## 4. Custom simulations
![custom_sim](/images/custom_sim.JPG)
# Other interesting leftovers

## Debug modes
![debug](/images/debug_modes.JPG)
## Error check files







