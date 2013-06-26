/// Author: Przemysław Bągard

#ifndef MODEL_H
#define MODEL_H

#include "GL/gl.h"
#include "QImage"
#include "Geometry/Point.h"

/// Graphical model
class Model
{
public:
    Model();

    /// Initializes model (allocates memory).
    void initialize();

    /// Every frame routine.
    void processTime(float elapsed_time);

    /// Renders model.
    void render();

    /// Releases model memory.
    void remove();

    /// Calculates vertex coordinates in interpolation between frame '_frame' and next frame.
    void calculate(int _frame, float percent);

    /// Calculates vertex coordinates from given frame.
    void calculate(int _frame = 0);

    /// Creates texture from image.
    void createTexture(QImage &img);

    /// Sets model data: vertex coordinates array, texture coordinates array, vertex indices array creating triangles array, count of model vertexes, count of model triangles.
    void setModel(const float* vertex, const float* tex_coords, const unsigned short int* triangles, int vertex_n, int tri_n);

    /// Sets states array (array contains first frames of animation states), count of states, and initialize animation speed.
    void setAnimation(const int * states, int n, float speed = 0.0f);

    /// Sets animation speed.
    void setAnimationSpeed(float speed);

    /// Sets animation state.
    void setAnimationState(int stadium);

    /// Returns current animation state.
    int getAnimationState();

    /// Returns animation states based on current frame.
    int getCurrentState();

    /// Model data.
    struct ModelData{
        /// Vertex coordinates.
        const float* vertex;

        /// Texture coordinates.
        const float* tex_coords;

        /// Indices (each 3 indices creates a triangle)
        const unsigned short int* triangles;

        /// Vertex coordinates count.
        int vertex_n;

        /// Triangle coordinates count.
        int tri_n;
    };

private:
    /// Model texture.
    GLuint _tex;

    /// Model data.
    ModelData _model;

    /// Pointer to vertex coordinates array. Pointer indicate current animation frame.
    float* current;

    /// Vertex coordinates (each 9 creates a triangle), used by 'glDrawElement'
    float* coordinates;

    /// Animation data.
    struct {
        /// First frames of animation states (the last value is count of model frames).
        const int * states;

        /// Count of animation states.
        int n;

        /// Speed of interpolation between frames.
        float speed;
    } _animation;

    /// Frame data
    struct {
        /// Current frame number.
        int current;

        /// Percent of interpolation between frames, value in the range [0.0, 1.0).
        float percent;
    } _frame;

    /// Creates 'coordinates' array based on 'current' array.
    void generateSequence();

    /// Returns index of next frame - frames are looped within an animation state.
    int next_frame(int _frame);

public:
    /// Returns model data.
    const ModelData& getModelData() { return _model; }
};

#endif // MODEL_H
