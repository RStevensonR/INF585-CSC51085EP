# Skinning - Skeletal Animation

This code present a more advanced scene setup with pre-loaded character animation. Your objective will be to add additional effects on the animated skeleton, in order to represent: smooth transitions, interactive user control to make the character walk, force the orientation of the head, and force the position of specific joint using Inverse Kinematics.

Once completed, your result should be similar to this [online version](https://imagecomputing.net/data/teaching/INF585/character_animation/web/no_soccer/index.html).

### Continuation with respect to lab 04_character_animation

This code is complemented with what you have done with respect to character animation. Before proceeding, it is advisable to finish first the exercises of `04_character_animation` and `05_skinning/a_skinning`.

## ${\color{cyan}\bf{TODO}}$: Applying LBS

Complete the linear blend skinning deformation to represent properly the models of the characters. The function to be completed is in the file `animated_character/animated_model/animated_model.cpp`, function `skinning_lbs()`

![Solution LBS](sollbs.gif)

## ${\color{cyan}\bf{TODO}}$: Effects

All the effects related to the lab (smooth transition, walk, head orientation and IK) were explained in `04_character_animation`. Replace the file `effects/effects.cpp` with your file from the previous laboratory to get all the effects.

### Smooth Transition Effect

![Solution Smooth Transition LBS](solsmoothtransitioneffectlbs.gif)

### Walk Effect

![Solution Walk LBS](solwalkeffectlbs.gif)

### Solution Head Orientation

![Solution Head LBS](solutionheadlbs.gif)

### Inverse Kinematics

![Solution IK LBS](solutioniklbs.gif)