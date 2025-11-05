#include "deformation.hpp"

using namespace cgp;


void build_matrix(linear_system_structure& linear_system, constraint_structure const& constraints, mesh const& shape, numarray<vec3> const& initial_position, numarray<numarray<int> > const& one_ring)
{
    size_t const N = shape.position.size();
	size_t const N_constraints = constraints.fixed.size() + constraints.target.size();

    // TO DO: Setup RHS and Guess vectors, and build the matrix M
    //
    //  1) Initialize the right-hand-size vector and the initial solution guess
    //  2) Fill the matrix M using the differential coordinates and the Laplacian constraints
    // Hint:
    // - For the rhs consider also the number of constraints
    // - You can use the initial position for the rhs and the initial guess
    // - Remember to check the functions of Eigen to build M


    linear_system.M.makeCompressed();
    // Factorization
    linear_system.solver.compute(linear_system.M);
    linear_system.solver.setTolerance(1e-6f);
}

void update_deformation(linear_system_structure& linear_system, constraint_structure const& constraints, cgp::mesh& shape,  cgp::mesh_drawable& visual, cgp::numarray<cgp::vec3> const& initial_position, cgp::numarray<cgp::numarray<int> > const& one_ring, cgp::numarray<cgp::mat3> & rotations_arap, bool arap)
{
    size_t const N = shape.position.size();
    size_t N_iteration = 4;

    // TO DO: Update the RHS with new constraints, add the arap modification and solve the system
    //
    // For every iteration you should:
    // 0.5) If arap is used:
    //       - Compute the covariance matrix for each vertex
    //       - Compute the optimal rotations using SVD (check functions of Eigen)
    //       - Store the rotations
    //   1) Compute the differential coordinates
    //   2) Add the constraint terms
    //   3) Solve the linear system (check solveWithGuess() in the Eigen library)
    //   4) Update the guess solution and the shape positions
    //
    // Hint:
    // - It is recommended to do the initial solution without ARAP, once you have the initial solution you can add the ARAP.
    // - For the traditional Laplacian deformation, you use multiple iterations to converge to the solution, but is slightly different for ARAP.

    for(int k_iteration=0; k_iteration<N_iteration; ++k_iteration)
	{
        // Fill with your solution...
    }

    // Shape updates
    // To be clear, this section of code DOES NOT solve the problem, it only updates the visual representation
    shape.normal_update();
    visual.vbo_position.update(shape.position); 
    visual.vbo_normal.update(shape.normal);

}
