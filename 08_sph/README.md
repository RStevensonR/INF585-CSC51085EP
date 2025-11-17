# Simulation - Smoothed Particle Hydrodynamics


The objective is to model a fluid simulation using the SPH model. So far, only the gravity force is applied on the particles.

The blue color around the particle comes from a field function set as $f(p)=\sum_i exp(-(\parallel p - p_i \parallel / \sigma)^2)$ and displayed as a colored texture to give the visual impression of continuous fluid material. You can find the base code for the simulation in `simulation/simulation.cpp`.

![Initial state of the program](baseprogram.gif)

<ins>Initial state of the program</ins>

## ${\color{cyan}\bf{TODO}}$: Add Density

__Fill the function__ `update_density`, which sets the density at each particle position $\rho_i$ using a smooth kernel.

$W_h(p) = \frac{315}{64\pi h^3} (1 - (\frac{\parallel p \parallel}{h})^2)^3, \parallel p \parallel \leq h$

Note that this kernel is already implemented in the function `W_density`.

__Complete the function__ `update_force` to add the force component related to the *pressure*, and check that the solution seems reasonable (before adding viscosity forces).

The pressure force is defined as:

$F_{pressure} = -\frac{m_i}{\rho_i} \sum_{j=0,j\neq i}^{N-1} m_j \frac{p_{r_j} + p_{r_i}}{2\rho_j} \nabla W_h^{p_r}(\parallel p_i - p_j \parallel)$

Where $m_i$ is the mass, $\rho_i$ is the density, $p_i$ is the position and $p_r$ is the pressure of particle $i$. $W_h^{p_r}(d) = \frac{15}{\pi h^6}(h - d)^3$ is the spiky kernel for the pressure gradient with influence distance $h$.

__Fill the function__ `W_gradient_pressure` to be the gradient of the spiky kernel, and use it in the computation of the pressure force.

![Example obtained without viscosity using 20 integration steps for each frame](solpressure.gif)

<ins>Example obtained without viscosity using 20 integration steps for each frame</ins>

## ${\color{cyan}\bf{TODO}}$: Add Viscosity

**Add the viscosity component** to the acceleration and observe the behavior for various viscosity (you may need to decrease the integration time step for large viscosity values).

The viscosity force is defined as:

$F_{viscosity} = m_i \mu \sum_{j=0,j\neq i}^{N-1} m_j\frac{v_j - v_i}{\rho_j} \Delta W_h(\parallel p_i - p_j \parallel)$

Where $\mu$ is the viscosity parameter, $v_i$ the velocity. $W_h(d) = \frac{15}{2 \pi h^6}(h - d)^3$ is the spiky kernel for the viscosity Laplacian with influence distance $h$.

![Adding viscosity](solviscosity1.gif)

<ins>Adding viscosity</ins>

![Example with more particles](solviscosity2.gif)

<ins>Example with more particles</ins>

## Extensions

- Note that particles have been initialized in the $(x,y)$ plane, which leads to a 2D simulation. But if you initialize particles in 3D (or add a small $z$ component in their initial position), the simulation will seamlessly extend to 3D.
- You can also implement an acceleration grid structure to avoid comparing all pairs of particles (ex. uniform grid). 