#include "Model.h"
#include "GL/glu.h"
#include "QGLWidget"

Model::Model()
{
    _animation.states = 0;
    _animation.n = 0;
    _model.vertex = 0;
    _model.tex_coords = 0;
    _model.triangles = 0;
    _animation.speed = 0.25f;
    _frame.percent = 0.0f;
    _frame.current = 0;
}

void Model::initialize()
{
    current = new float[_model.vertex_n*3];
    coordinates = new float[_model.tri_n*9];
}

void Model::processTime(float elapsed_time)
{
    if(_animation.states != 0)
    {
        _frame.percent += elapsed_time*_animation.speed;
        while(_frame.percent >= 1.0f)
        {
            /// Next frame.
            _frame.current = next_frame(_frame.current);
            _frame.percent -= 1.0f;
        }

        /// Calculate position of model vertexes.
        calculate(_frame.current, _frame.percent);
    }
}

void Model::remove()
{
    delete[] current;
    current = 0;
    delete[] coordinates;
    coordinates = 0;
}

int Model::next_frame(int frame)
{
    int stadium = 0;
    /// Finds animation state for given parameter 'frame'.
    for(int l1 = 0; l1 < _animation.n-1; ++l1)
    {
        if(frame >= _animation.states[l1])
            stadium = l1;
    }

    if(frame == _animation.states[stadium+1]-1)
    {
        /// Last frame of animation state. Skips to first frame of this animation state.
        return _animation.states[stadium];
    }
    else
        return frame+1;
}

void Model::generateSequence()
{
    for(int i = 0; i < _model.tri_n; ++i)
    {
        coordinates[i*9+0] = current[_model.triangles[i*3+0]*3+0];
        coordinates[i*9+1] = current[_model.triangles[i*3+0]*3+1];
        coordinates[i*9+2] = current[_model.triangles[i*3+0]*3+2];
        coordinates[i*9+3] = current[_model.triangles[i*3+1]*3+0];
        coordinates[i*9+4] = current[_model.triangles[i*3+1]*3+1];
        coordinates[i*9+5] = current[_model.triangles[i*3+1]*3+2];
        coordinates[i*9+6] = current[_model.triangles[i*3+2]*3+0];
        coordinates[i*9+7] = current[_model.triangles[i*3+2]*3+1];
        coordinates[i*9+8] = current[_model.triangles[i*3+2]*3+2];
    }
}

void Model::setAnimationState(int stadium)
{
    _frame.current = _animation.states[stadium];
}

void Model::setModel(const float *vertex, const float *tex_coords, const unsigned short *triangles, int vertex_n, int tri_n)
{
    _model.vertex = vertex;
    _model.tex_coords = tex_coords;
    _model.triangles = triangles;
    _model.vertex_n = vertex_n;
    _model.tri_n = tri_n;
}

void Model::setAnimation(const int *states, int n, float speed)
{
    _animation.states = states;
    _animation.n = n;
    _animation.speed = speed;
}

void Model::setAnimationSpeed(float speed)
{
    _animation.speed = speed;
}

int Model::getAnimationState()
{
    return _frame.current;
}

int Model::getCurrentState()
{
    int stadium = 0;
    /// Finds animation state for current frame.
    for(int l1 = 0; l1 < _animation.n-1; ++l1)
    {
        if(_frame.current >= _animation.states[l1])
            stadium = l1;
    }
    return stadium;
}

void Model::calculate(int frame, float percent)
{
    int nframe = next_frame(frame);
    for(int i = 0; i < _model.vertex_n*3; i++)
    {
        current[i] = _model.vertex[i + frame*_model.vertex_n*3] + (_model.vertex[i + nframe*_model.vertex_n*3] - _model.vertex[i + frame*_model.vertex_n*3]) * percent;
    }

    generateSequence();
}

void Model::calculate(int frame)
{
    for(int i = 0; i < _model.vertex_n*3; i++)
        current[i] = _model.vertex[i + frame*_model.vertex_n*3];

    generateSequence();
}

void Model::createTexture(QImage& img)
{
    QImage image = QGLWidget::convertToGLFormat(img);

    glGenTextures( 1, &_tex);
    glBindTexture( GL_TEXTURE_2D, _tex );
    glTexImage2D( GL_TEXTURE_2D, 0, 4, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
}

void Model::render()
{
    glBindTexture(GL_TEXTURE_2D, _tex);
    glBegin(GL_TRIANGLES);
    for (int l1 = 0; l1 < _model.tri_n; ++l1) {
        for (int l2 = 0; l2 < 3; ++l2) {
            glTexCoord2f(_model.tex_coords[l1*6+l2*2], _model.tex_coords[l1*6+l2*2+1]);
            glVertex3f(coordinates[l1*9+l2*3+0], coordinates[l1*9+l2*3+1], coordinates[l1*9+l2*3+2]);
        }
    }
    glEnd();

    /*
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, coordinates);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, _model.tex_coords);

    glDrawArrays(GL_TRIANGLES, 0, _model.tri_n*3);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    */
}
