# Simulation - Stable FLuids

> Path of the scene is 09_stable_fluids

The objective of this scene is to simulate the "stable fluid" model (in 2D) proposed by Jos Stam in [[Stable Fluids. Jos Stam. SIGGRAPH 1999](https://imagecomputing.net/course/2023_2024/inf585/lab/content/12_stable_fluids/assets/stable_fluids.pdf)]. And later described with more algorithmic details in [[Real-Time Fluid Dynamics for Games. Jos Stam. Game Developers Conference, 2003](https://imagecomputing.net/course/2023_2024/inf585/lab/content/12_stable_fluids/assets/real_time_fluid.pdf)].

In the current state, the code encodes a velocity grid (the velocity of the fluid) and a density grid (the color/image which is visualized). The values of these grid are only advected through time, and your objective is to complete the three steps of the stable fluids method, namely 
- Diffusion(function `diffuse`) in `simulation.hpp`.
- Advection(function `advect`) in `simulation.hpp`.
- Projection of the velocity to a divergence free vector field(function `divergence_free`) in `simulation.cpp`.

These three steps are already called in the function `simulate` in `scene.cpp`. Note that diffusion and advection steps are applied to both velocity and density field, while the projection is only applied to the velocity.

Clicking on the left button of the mouse and dragging it allows to set the speed on the corresponding grid cell (but this speed doesn't evolve yet). We will not consider any other external forces applied on the fluid. 

## Diffusion

The function diffuse is set to solve one step of the equation $\frac{\delta f}{\delta t} = \mu \Delta f$, where $f$ can be either the velocity, or a density. To handle this generic input, the function is encoded to received a 2D grid of template values. The function is also defined in the header file for this reason.

The equation is solved numerically using an implicit scheme in order to ensure unconditional stability: 

$(1+4\Delta t \mu)f_{x,y}^t=f_{x,y}^{t-\Delta t} + \mu\Delta t(f_{x+1,y}^t + f_{x-1,y}^t + f_{x,y+1}^t + f_{x,y-1}^t)$

Implement the numerical solution of this equation using Gauss Seidel iterations (consider around 15 iterations). The function to complete use template arguments and is defined in the file `simulation.hpp`.

Note:
- For each Gauss Seidel iteration, you may set the values of the grid boundary using the pre-coded function `set_boundary` in `boundary.hpp`.
- The Gauss Seidel method consists in iterating over

    $\forall(x,y), (f_{x,y}^t)^{new}= \frac{1}{1+4\mu\Delta t}(f_{x,y}^{t-\Delta t}  + \mu\Delta t (f_{x+1,y}^t + f_{x-1,y}^t + f_{x,y+1}^t + f_{x,y-1}^t))$

    $\forall(x,y), f_{x,y}^t \leftarrow (f_{x,y}^t)^{new}$

![Solution Diffusion](soldiffusion.gif)

## Advection

The function `advect` is set to displace a field $f$ along a prescribed velocity $v$. $f$ can itself be a density, or the velocity itself.

To ensure stable advection, backtracing can be use in setting the value $f(p,t)$ where $p$ is a grid cell position, and $t$ the current time from the interpolated value of $f$ at previous time step at position $f(p-\Delta tv(p,t), t -\Delta t).$

The advection function is __already__ implemented in the function `advect` using bilinear interpolation at position $p-\Delta tv(p,t)$. Pay attention to not fetch values outside the boundaries of the grid.

You should be able to create visible motion from your mouse trajectory in your density. This motion should be smooth, but doesn't ensure yet incompressible behavior.

![Solution Advection](soladvection.gif)

## Divergence Free Projection

The last step ensuring incompressibility is to constraint the velocity field to be divergence free.

This step need only to be applied on the velocity field (and not on density), and consists in three sub-steps.

1. Compute the divergence of the input velocity $v_0$.
2. Compute a gradient fieldm $q$ satisfying Poisson equation $\Delta q = div(v)$
3. Compute divergent free vector field as $v=v_0 - \nabla q$

Similarily to the divergence computation, the Poisson equation is solved using a stable implicit numerical scheme, typically using the Gauss Seidel iterations. Between each of these steps, and at each iteration of the Gauss Seidel, boundaries of the different quantities can be reset using the `set_boundary_function`.

__Implement these three steps in the__ `divergence_free` __function__ to reach a divergence free vector field that you can interactively control.

![Solution Divergence Free](soldivergence.gif)