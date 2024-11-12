# Simulation - Shape Matching

> Path of the scene 07_shapes_and_clothes/a_shape_matching

The objective of this exercise is to implement a Rigid Body simulation using PPD approach and Shape Matching. This approach relates to the publications:
- [1](https://matthias-research.github.io/pages/publications/MeshlessDeformations_SIG05.pdf) Meshless Deformations Based on Shape Matching. 
- [2](https://matthias-research.github.io/pages/publications/flex.pdf) Unified Particle Physics for Real-Time Applications.

![Example version](exampleprogram.gif)

Once completed, the result should be similar to this [online version](https://imagecomputing.net/course/2023_2024/inf585/lab/content/11_shape_matching/web/index.html).

## Program structure

The current program applies a standard integration using the Position Based Dynamics approach over object (cubes, cylinder, cones, etc.) made from multiple particles positionned at the vertex of their respective mesh. This integration is performed in the file `simulation/simulation.cpp`.

During the simulation, the positions are constrained with three functions:
- `collision_with_walls`: Ensure that the particles don't enter in the ground and the two walls.
- `collision_between_particles`: Handle the collision between particles belonging to different objects.
- `shape_matching`: Ensure the particle of a given object preserves its original shape.

So far, only `collision_with_walls` is completed, and the other two functions need to be filled.

## Shape Matching

First start by implementing the function `shape_matching` for a rigid transformation.

*Reminders*:

We consider an object initially defined by a reference shape with vertex positions $p_i^{ref}​$, and barycenter $c^{ref}$.

We call $p_i$​ the vertex position, and $c$ the barycenter of the current deformed object state.

We call $T$ the transformation matrix such that

$T = \sum_i (p_i - c)(p_i^{ref} - c^{ref})^T$

This matrix represents the transformation that minimizes $\parallel T (p_i^{ref} - c^{ref}) - (p_i - c)\parallel^2$

The shape matching approach for rigid deformation consists in:
1. Compute the rotation matrix $R$ given as the polar decomposition of T, i.e. the rotation approximating at best the transformation $T$.
2. And use it to set the new vertex positions $q_i$ as $q_i = R(p_i^{ref} - c^{ref}) + c$.

Once implemented, the new object you create should preserve their shape and collide with the walls.

## Collision Between Particles

Now add the collision between shapes. To this end, we can consider that each particle is associated to a colliding sphere with a specific radius (`simulation_parameter.collision_radius`). Handling the collision between the object consists in handling collision between two spheres belonging to different shapes. Fill the function `collision_between_particles` in order to handle such collision.

Notes:
- The treatment of the collision should be handled via positional modification, i.e. projecting collising spheres onto their contact surface.
- When projecting shapes at high speed, some collisions between shapes can be missed with particles traversing fully through another one. To reduce this effect, you can:
    - Increase the `collision_steps`: number of iterations handling collisions + shape matching at each simulation step.
    - Decrease the `time_step`.
    - Increase the `collision_radius`.

## Elasticity and Plasticity

Extend the model to handle a notion of elasticity and plasticity to deformation.

- Elasticity can be modeled in considering the following deformation

    $q_i = (1 - \alpha)R(p_i^{ref} - c^{ref}) + c + \alpha p_i$

    $\alpha$ is the elasticity parameter (in the code: `param.elasticity`)

![Collision between particles](solparticlecollision.gif)

- Plasticity can be modeled in considering a modification of $p_i^{ref}$ (and $c^{ref}$) when a large deformation occurs.

    Ex:
    If $\parallel R(p_i^{ref} - c^{ref}) - (p_i - c) \parallel \geq \epsilon$:
    
    Then update $p_i^{ref}$ and $c^{ref}$ to take into account such deformation in their new state.

[Example of elastic deformation](solelasticdeform.gif)

[Example of plastic deformation](solplasticdeform.gif)