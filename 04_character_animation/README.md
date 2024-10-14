# Character Animation - Skeletal Animation

> Path of the scene 04_character_animation

This code present a more advanced scene setup with pre-loaded character animation. Your objective will be to add additional effects on the animated skeleton, in order to represent: smooth transitions, interactive user control to make the character walk, force the orientation of the head, and force the position of specific joint using Inverse Kinematics.

Once completed, your result should be similar to this [online version](https://imagecomputing.net/data/teaching/INF585/character_animation/web/no_soccer/index.html).

>[!NOTE]
> The online version has skinning for character models but thats for the next session, for this week focus on the movement. SAME FOR THIS SESSION, DONT BOTHER WITH THE SKINNING, THATS FOR THE NEXT SESSION.

## Description of the animated character structure

### General Representation 

The code provides a generic `character_structure`, and the scene is composed a few character accessible via their name (`characters[NAME]`).
The mesh, rig, skeleton hierarchy and animation of the animated characters are pre-stored in files in the directory `assets/`, and are loaded at the start of the program. All the models are from [mixamo.com](https://www.mixamo.com).
Each character is composed of a single skeleton hierarchy that can be associated to multiple pre-stored skeletal animations. Each character can also be composed of multiple rigged meshes. Each rigged mesh has its own set of skinning weights and dependence to a subpart of the skeleton. This dependence is stored using a sparse structure, i.e. for a given vertex we only store the joint index and associated weight which is not zero. 

### Details of code organization

![code_organization](code_organization.png)

A `character_structure` is itself a general container for the core structure of the animated model (mesh, rig, skeleton and its animation), a local timer used to run the animation cycle, and helper drawable classes used to display the mesh and skeleton.

```
    struct character_structure {

        // Core container of the animated model (mesh, rig, skeleton, animation)
        animated_model_structure animated_model;

        // Helper to run the animation
        timer_event_periodic timer;
        string current_animation_name;

        // Helper to draw the mesh and skeleton
        map<string, mesh_drawable> drawable;
        skeleton_drawable sk_drawable;
    };
```

The `animated_model` contains a unique skeleton (`skeleton_structure`), which is the one used to compute the skinning deformation. This skeleton is updated at each frame when animated, and it is the one to modify when adding additional effects like smooth transitions, or IK. The structure also stores all the possible animations associated to this skeleton (`skeleton_animation_structure`), and the set of all rigged mesh (`rigged_mesh_structure`) composing the character. The animation and rigged mesh are accessible via their name.

```
    struct animated_model_structure {
        
        // Current skeleton
        skeleton_structure skeleton; 

        // Store all preset animations
        map<string, skeleton_animation_structure> animation; 

        // Store all rigged meshes
        map<string, rigged_mesh_structure> rigged_mesh; 



        void skinning_lbs(mesh_name);
        void set_skeleton_from_animation(animation_name, t);
        void apply_transformation(translation, rotation, scaling);
    };
```

The `skeleton_structure` stores in arrays the list of all joint properties: the joint name, the joint matrix in local coordinates ( oordinates expressed relative to their parent frame) and global coordinates, and the index of the parent joint. All the joint are stored in the same order, and the index of the parent joint is always smaller that the child one.

```
    struct skeleton_structure {

        numarray<mat4> joint_matrix_local;
        numarray<mat4> joint_matrix_global;
        numarray<string> joint_name; 
        numarray<int> parent_index;

        // Number of joints
        int size() const;
        // fill joint_matrix_global from joint_matrix_local information
        void update_joint_matrix_local_to_global();
        // fill joint_matrix_local from joint_matrix_global information
        void update_joint_matrix_global_to_local();
    };
```

The `rigged_mesh` structure stores respectively the bind pose of the mesh (the non-deformed initial position of the vertices), the current deformed mesh which is displayed, and the rig controller, i.e. the dependence between the vertices and the joints of the skeleton.

```
    struct rigged_mesh_structure {
        mesh mesh_bind_pose;
        mesh mesh_deformed;
        controller_skinning_structure controller_skinning; 
    };
```

The `controller_skinning_structure` contains the set of skinning weight dependences stored in a sparse way (`vertex_to_joint_dependence`): the non-zeros weights are stored using the couple (joint index,weight value). As each mesh can be linked to a subset of joints of the skeleton, the joint index may correspond to a local index within this subset. An additional level of indirection allows to map such local index to the ones within the global skeleton. The correspondance map between these indices is stored in `rig_index_to_skeleton_index`. The structure also stores the inverse of the bind matrix in the global coordinate space, i.e. the inverse of the joint frame matrix in the bind pose of the mesh (or, alternatively, the deformation to apply on a mesh vertex to place it in the bind pose of the skeleton).

```
    struct controller_skinning_structure {
        // Vertex_to_joint_dependence[k_vertex][k_dependance] 
        numarray<numarray<skinning_weight_info> > vertex_to_joint_dependence; 
        
        // Mapping between local dependence index to global skeleton joint index
        numarray<int> rig_index_to_skeleton_index; 

        // Pre-store the inverse of the bind pose matrix
        numarray<mat4> inverse_bind_matrices;
    };

    // Pair (local dependence index, skinning weight value)
    struct skinning_weight_info {
        int joint_index;
        float weight;
    };
```
### Note on animation files

The files used by the code are specific ASCII-readable file structure which are easy to parse in C++ without the need of additional library.

These files are exported via the use of the python script `assets/export_from_dae.py` which read Collada file as input, and convert it to the ASCII files.

If you wish to add additional character, or animation, you may download Collada (.dae) characters from the [Mixamo](https://www.mixamo.com) website, and run the Python script on it. The output files can then be placed in the appropriate directories to be loaded in the C++ code. 

## The Exercise Starts Here

Again, don't bother with the skinning.

## Smooth Transition Effect

The first effect to implement is to compute a smooth transition between two animation when selecting a new one. Every time the user select a new animation, the structure `effect_transition` associated to the modified character is filled with the appropriated parameters: the name of the source and destination animation, and a timer starts in order to perform the interpolation between the two states.

Your objective is to complete the function `effect_transition_compute` in `effets/effects.cpp` in order to compute the intepolation between the appropriate joint transformation, and update the current character skeleton.

Notes:
- Think about which joint matrices should be interpolated, and how the interpolation should be computed.
- When working properly, the transition should not exhibits shrinkage of the character limbs.
- The parameter transition_time, accessible via the GUI, gives the total amount of time used for the transition.

![solutionsmoothtransition](solutionsmoothtransition.gif)

## Walk Effect

The second objective is to implement a way to control interactively the character to make it walk in the direction given by the keyboard. Each character is provided with a default `Idle` and `Walk` animation. The walk animation, however, is a default cycle making the character moving straight for a short duration, before going back to the original pose. The goal is to be able to transition and set the character translation according to the user control in being able to move straight, turn left or right.

This interaction for the walking effect is handled via two functions. First `effect_walking_keyboard_event`, which is already implemented, is called when a keyboard key is pressed or released.
- When the `up` key is pressed, the character animation start to shortly transition toward is `Walk` animation.
- When the `up` key is released, the character starts the transition to `Idle` animation.

The second effect allowing to control the character position and orientation must be coded in the function effect_walking. Fill this function in order to obtain the expected effect.

Notes:
- The `effect_walking_structure` allows you to store a root position, and a root angle.
- You may assume that initial "Lola" character is initially oriented along the `z` direction.
- You may neglect the default translation of the animation cycle, which can lead to a small sliding effect between the feets and the floor.

![solutionwalk](solutionwalkeffect.gif)

## Head Orientation

The third objective is to implement a change of head orientation such that the character look at a given position. Complete the function `effect_rotate_head_toward_objective_position` to achieve this effect. Once active, the current character should be looking at the camera position.

Notes:
- You can use a parameterization of the rotation along the two degrees of freedom of the neck.
- You may consider max limit angles that remains in plausible head orientation to avoid fully twisting the neck.

![solutionheadorientation](solutionheadorientation.gif)

## Inverse Kinematics

The last effect is to implement an Inverse Kinematics (IK) over a subset of the skeleton joints of a character.

The IK parameters are stored in `effect_ik_structure` describing the target joint, and the root joint of the kinematic chain. The objective position of the target joint is given as the sum of an initial position and an offset (that can be changed via the GUI). The function `effect_ik_start` pre-set the values of the objective position (current target joint position and zero offset).

The objective is to code the function `effect_ik_compute` to implement an IK deformation on an arbitrary kinematic chain defined between joint_root_ik and joint_target, for instance using Fabrik.

Hints:
- You may need to pre-store the set of joints and bones along the chain defined between `joint_root_ik` and `joint_target`.
- The GUI allows to interactively change `joint_root_ik` and `joint_target`. If the chain is not a valid one, i.e. `joint_root_ik` not being a parent of `joint_target`, you may consider the skeleton root instead.
- You may compute first the IK over the position only, and then convert the final into rotation to be applied on the joints

![solutioninversekinematics](solutionik1.gif)
![solutioninversekinematics](solutionik2.gif)