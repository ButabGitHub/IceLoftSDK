#pragma once

#include "../Node.h"

#include <glm/glm.hpp>

class Transform : public Node {
private:
	glm::vec3 local_position;
	glm::vec3 local_size;
	glm::vec3 local_rotation;

	// Matrices
	glm::mat4 local_matrix;
	glm::mat4 global_matrix;

    // Matrice updates
    void update_local_matrix();
    // Updates the global_matrix based on parent's global transform and this node's local transform
    void update_global_matrix();

public:

	// Constructor
	Transform();
	
    // Local Transform setters
    void set_position(const glm::vec3& _position);
    void set_rotation_euler(const glm::vec3& _euler_radians);
    void set_scale(const glm::vec3& _scale);
    void set_velocity(const glm::vec3& _velocity);

    // Local Transform getters
    glm::vec3 get_position() const;
    glm::vec3 get_rotation_euler() const;
    glm::vec3 get_scale() const;
    glm::vec3 get_velocity() const;

    // Matrix Getters
    glm::mat4 get_local_matrix() const;
    glm::mat4 get_global_matrix() const;

    // Global Transformation Getters (derived from global_matrix)
    glm::vec3 get_global_position() const;
    glm::vec3 get_global_rotation_euler() const;
    glm::vec3 get_global_scale() const;

    // Orients the node to look at a target position in global space.
    // This modifies the local rotation to achieve the global look-at.
    void look_at(const glm::vec3& target_global, const glm::vec3& up_global = glm::vec3(0.0f, 1.0f, 0.0f));

};

