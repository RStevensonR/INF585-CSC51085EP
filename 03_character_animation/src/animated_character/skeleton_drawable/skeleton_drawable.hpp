#pragma once

#include "cgp/cgp.hpp"

#include "../skeleton_structure/skeleton_structure.hpp"

struct skeleton_drawable
{
    skeleton_drawable();
    skeleton_drawable(skeleton_structure const& skeleton);
    void clear();
    void update(skeleton_structure const& skeleton);

    float size_frame = 0.05f;
    float size_sphere = 0.1f;
    bool display_segments = true;
    bool display_joint_frame = true;
    bool display_joint_sphere = true;

    cgp::curve_drawable segments;
    cgp::mesh_drawable joint_frame;
    cgp::mesh_drawable joint_sphere;
		int head_joint_index;
		int left_eye_index = -1;
		int right_eye_index = -1;
		cgp::mesh_drawable head_mesh;
    cgp::numarray<cgp::mat4> data;
};

template <typename SCENE>
void draw(skeleton_drawable const& skeleton, SCENE const& scene);

template <typename SCENE>
void draw(skeleton_drawable const& skeleton, SCENE const& scene)
{
	if(skeleton.display_segments)
		draw(skeleton.segments, scene);
	
	size_t const N = skeleton.data.size();

	if(skeleton.display_joint_frame)
	{
		cgp::mesh_drawable joint_frame_temp = skeleton.joint_frame;
		joint_frame_temp.model.scaling = skeleton.size_frame;
		for (size_t k = 0; k < N; ++k)
		{
			
			joint_frame_temp.model = cgp::affine::from_matrix(skeleton.data[k]).set_scaling(skeleton.size_frame);
			draw(joint_frame_temp, scene);

			if (k == 15)
				{
					cgp::mesh_drawable arrow;
					cgp::vec3 base = skeleton.data[k].get_block_translation();
					cgp::vec3 dir = skeleton.data[k].get_block_linear() * cgp::vec3(0,0,1);
					arrow.initialize_data_on_gpu(mesh_primitive_arrow(base, base + 0.5f * dir, 0.01f));
					arrow.material.phong.specular = 0;
					arrow.material.color = cgp::vec3(1, 0, 0);
					draw(arrow, scene);
				}
		}
	}
	
	if (skeleton.display_joint_sphere)
	{
		cgp::mesh_drawable joint_sphere_temp = skeleton.joint_sphere;
		cgp::mesh_drawable head_mesh_temp = skeleton.head_mesh;
		joint_sphere_temp.model.scaling = skeleton.size_sphere;
		head_mesh_temp.model.scaling = 0.01f;
		for (size_t k = 0; k < N; ++k)
		{
			if (k == skeleton.head_joint_index)
			{
				head_mesh_temp.model.translation = skeleton.data[k].get_block_translation();
				head_mesh_temp.model.rotation = cgp::rotation_transform::from_matrix(skeleton.data[k].get_block_linear());
				head_mesh_temp.material.phong.specular = 0.0f;
				draw(head_mesh_temp, scene);
			}
			else if (k != skeleton.left_eye_index && k != skeleton.right_eye_index)
			{
				joint_sphere_temp.model.translation = skeleton.data[k].get_block_translation();
				joint_sphere_temp.model.scaling = (0.2f + 0.8f * exp(-((float) k * k / (0.2 * N * N)))) * skeleton.size_sphere;
				joint_sphere_temp.material.phong.specular = 0.0f;
				draw(joint_sphere_temp, scene);
			}
		}
		joint_sphere_temp.model.scaling = skeleton.size_sphere;
	}
}
