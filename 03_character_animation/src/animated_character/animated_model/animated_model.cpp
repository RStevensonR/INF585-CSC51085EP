#include "animated_model.hpp"

using namespace cgp;

void animated_model_structure::set_skeleton_from_animation(std::string const& animation_name, float t)
{
    auto const& current_animation = animation[animation_name];
    int N_joint = current_animation.joint_index.size();
    for(int k=0; k<N_joint; ++k) {
        int joint = current_animation.joint_index[k];
        mat4 M = current_animation.evaluate(k, t);
        skeleton.joint_matrix_local[joint] = M;
    }
    skeleton.update_joint_matrix_local_to_global();
}

void animated_model_structure::apply_transformation(vec3 const& translation, rotation_transform rotation, float scaling)
{
    if(skeleton.joint_matrix_local.size()>0) {
        skeleton.joint_matrix_local[0] = rotation * skeleton.joint_matrix_local[0];
        skeleton.joint_matrix_local[0].apply_scaling_to_block_linear(scaling);
        skeleton.joint_matrix_local[0].apply_translation(translation);
        skeleton.update_joint_matrix_local_to_global();
    }

    for(auto& anim_entry : animation) {
        auto& animation_matrices = anim_entry.second.matrix;
        for(int kt=0; kt<animation_matrices[0].size(); ++kt) {
            mat4& M = animation_matrices[0][kt];
            M = rotation*M;
            M.apply_scaling_to_block_linear(scaling);
            M.apply_translation(translation);
        }
    }

}