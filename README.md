# Visual Particle Simulator
This program recreates interactions between simple particles(red) and black holes(green). They interact via gravitational attraction, and change their trajectories accordingly. <br>
The small, blue arrows(generators) randomly generate particles with an initial velocity and radius. <br>
As time progresses, the various gravitational interactions change the velocity of the particles, causing them to speed up(bright red) or slow down(dark red). <br>
Once a particle touches the center of a sink, it gets deleted from the simulation.
![overview](/images/overview.gif)
<br>
The main files used to generate the systems are:

- sim.c:<br>
  Proident eu sit velit do. Labore quis enim irure ad ipsum irure consequat ea. Nostrud mollit adipisicing eu qui do ad voluptate ut ad ullamco non irure. Tempor exercitation ipsum duis ex anim non aliqua ex et duis magna deserunt. Aliquip ad sint velit consequat anim proident nisi aliquip magna proident enim. Irure occaecat veniam quis reprehenderit ea sit nisi minim tempor sint voluptate. Ad nisi duis mollit eu in enim velit.
- particle.c: <br>
  Exercitation magna eiusmod irure id est amet tempor eu anim sit sit deserunt. Sit dolor anim irure ut sit sunt nostrud magna do irure. Laborum nostrud reprehenderit aliqua consectetur minim ea ex id excepteur. Excepteur dolore pariatur eiusmod pariatur fugiat ad excepteur enim. Id excepteur culpa dolor dolore esse aliquip nulla deserunt ipsum. Mollit id enim eiusmod fugiat sit dolore in consectetur esse cillum ullamco consequat nostrud. Anim ipsum officia reprehenderit quis mollit ex esse occaecat laborum veniam anim.
- black_hole.c: <br>
  Ut fugiat reprehenderit ullamco non ex commodo fugiat commodo esse veniam ex esse. Duis enim ad velit exercitation eu laboris veniam. Elit magna laboris culpa nulla laborum in quis excepteur officia officia nostrud Lorem duis in. Magna exercitation ullamco minim velit consectetur laboris enim. Aute anim incididunt proident veniam. Tempor laboris irure enim laborum pariatur dolor et nulla esse aliqua.  
- generator.c: <br>
  Enim magna velit elit nostrud do veniam consectetur dolore. Duis velit sit officia ad culpa nostrud laboris nulla in. Ullamco ea culpa consectetur sint. Ullamco ullamco tempor magna fugiat pariatur aliqua cillum dolor non. Aute voluptate pariatur anim ipsum qui reprehenderit esse consectetur aliquip duis. Ipsum labore consequat et cillum quis non veniam commodo do.

<br>

**NOTE:** This work was partially written in french. Some words in the code/UI use both languages. It should still be straightforward. 

The overall architecture/dependancy graph of the modules is as follows:
  ![architecture](/images/architecture.JPG)

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