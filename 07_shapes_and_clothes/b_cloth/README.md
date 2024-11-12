# Simulation - Mass-Springs Cloth

> Path of the scene is 07_shapes_and_clothes/b_clothes


A cloth surface is modeled as a mesh where vertices are initially placed on a regular grid.
So far, only the weight (and a drag/friction force) is applied on the particles.

Two vertices are constrained to remain fixed (see the variable `positional_constraints` to remove/add new positional constraints).

- A set of GUI-based controlers on the time scale multiplicator applied on $h$, stiffness, damping, mass, and wind force. These parameters are available inside the code (only time scale is used in the initial code).
- The possibility to desactivate the cloth texture and display the wireframe, which may be helpfull for debugging.

An automatic check is performed after each numerical integration to detect a possible divergence of the system (`detect_simulation_divergence`). In such case, the simulation is stopped (with an error message on the command line).

## Adding Spring Forces

The objective is to implement the network of springs able to model the behavior of a cloth.

*Add the forces related to springs* in the function `compute_forces`
- Add first the forces of structural springs only and make sure that you get a plausible behavior before adding more forces. Using wireframe mode with slow time step and coarse mesh (few particles) can help debugging if something is wrong.
- Then add forces modeled by shearing and bending springs.

*Reminders*:

Consider a spring of stifness $K$ and rest length $L_0$​ attached between a point $p_i$​ and $p_j$​. The force exerted by the spring on the point $p_i$​ is:

$\mathbf{F} = -K(\parallel p_i - p_j \parallel - L_0)\frac{p_i - p_j}{\parallel p_i - p_j \parallel}$

Note the effect of spring stiffness and mass on the resulting simulation.
- You may add more springs, linking for instance 2-, or 3-neighborhood, if you want to model more rigid and smooth behavior for higher resolution meshes.

Implement the *collision detection and response* with respect to the ground and the sphere in the function `collision_constraints`. Note that the parameters of the radius and position of the sphere as well as the height of the ground are available in the variable `collision_shapes`.

Implement the *action of the wind*. How do you model such force? (check `simulation.parameters`)

![Example interaction](solinteraction.gif)

![Example wind](solwind.gif)