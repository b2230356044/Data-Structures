#include "CelestialObject.h"
bool are_shapes_identical(const vector<vector<bool>> &shape1, const vector<vector<bool>> &shape2) {
    if (shape1.size() != shape2.size() || shape1[0].size() != shape2[0].size()) {
        return false;
    }
    for (size_t i = 0; i < shape1.size(); ++i) {
        for (size_t j = 0; j < shape1[i].size(); ++j) {
            if (shape1[i][j] != shape2[i][j]) {
                return false;
            }
        }
    }
    return true;
}

// Constructor to initialize CelestialObject with essential properties
CelestialObject::CelestialObject(const vector<vector<bool>> &shape, ObjectType type, int start_row,
                                 int time_of_appearance)
        : shape(shape), object_type(type), starting_row(start_row), time_of_appearance(time_of_appearance),
          next_celestial_object(nullptr), right_rotation(nullptr), left_rotation(nullptr) {
    CelestialObject *current = this;
    vector<vector<vector<bool>>> generated_shapes = {shape};
    for (int i = 0; i < 3; ++i) {
        size_t rows = current->shape.size();
        size_t cols = current->shape[0].size();
        vector<vector<bool>> rotatedShape(cols, vector<bool>(rows));


        for (size_t r = 0; r < rows; ++r) {
            for (size_t c = 0; c < cols; ++c) {
                rotatedShape[c][rows - r - 1] = current->shape[r][c];
            }
        }


        bool is_unique = true;
        for (const auto &existing_shape : generated_shapes) {
            if (are_shapes_identical(rotatedShape, existing_shape)) {
                is_unique = false;
                break;
            }
        }


        if (!is_unique) {
            break;
        }


        generated_shapes.push_back(rotatedShape);
        CelestialObject *newRotation = new CelestialObject(current);
        newRotation->shape = rotatedShape;

        current->right_rotation = newRotation;
        newRotation->left_rotation = current;
        current = newRotation;
    }
    current->right_rotation = this;
    left_rotation = current;
}

// Copy constructor for CelestialObject
CelestialObject::CelestialObject(const CelestialObject *other)
        : shape(other->shape), // Copy the 2D vector shape
          object_type(other->object_type), // Copy the object type
          starting_row(other->starting_row), // Copy the starting row
          time_of_appearance(other->time_of_appearance), // Copy the time of appearance
          next_celestial_object(nullptr),
          right_rotation(nullptr),
          left_rotation(nullptr)


{

}

// Function to delete rotations of a given celestial object. It should free the dynamically allocated
// memory for each rotation.
void CelestialObject::delete_rotations(CelestialObject *target) {
    if (!target || target->right_rotation == target) return;

    CelestialObject *current = target->right_rotation;
    while (current != target) {
        CelestialObject *next = current->right_rotation;
        delete current;
        current = next;
    }


    target->right_rotation = target;
    target->left_rotation = target;
}
