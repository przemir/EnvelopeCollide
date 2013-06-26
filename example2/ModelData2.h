
const float model_box_size = 1.0f;

// Model
const unsigned short model_box_tri[] = {
0,1,2, 1,2,3,
4,5,6, 5,6,7,
0,1,4, 1,4,5,
2,3,6, 3,6,7,
0,2,4, 2,4,6,
1,3,5, 3,5,7
};
const float model_box_texCoord[] = {
0.0f, 0.0f,
1.0f, 0.0f,
0.0f, 1.0f,
1.0f, 0.0f,
0.0f, 1.0f,
1.0f, 1.0f,

0.0f, 0.0f,
1.0f, 0.0f,
0.0f, 1.0f,
1.0f, 0.0f,
0.0f, 1.0f,
1.0f, 1.0f,

0.0f, 0.0f,
1.0f, 0.0f,
0.0f, 1.0f,
1.0f, 0.0f,
0.0f, 1.0f,
1.0f, 1.0f,

0.0f, 0.0f,
1.0f, 0.0f,
0.0f, 1.0f,
1.0f, 0.0f,
0.0f, 1.0f,
1.0f, 1.0f,

0.0f, 0.0f,
1.0f, 0.0f,
0.0f, 1.0f,
1.0f, 0.0f,
0.0f, 1.0f,
1.0f, 1.0f,

0.0f, 0.0f,
1.0f, 0.0f,
0.0f, 1.0f,
1.0f, 0.0f,
0.0f, 1.0f,
1.0f, 1.0f
};

const float model_box_vertex[] = {
0.0f          , 0.0f          , 0.0f,
model_box_size, 0.0f          , 0.0f,
0.0f          , model_box_size, 0.0f,
model_box_size, model_box_size, 0.0f,
0.0f          , 0.0f          , model_box_size,
model_box_size, 0.0f          , model_box_size,
0.0f          , model_box_size, model_box_size,
model_box_size, model_box_size, model_box_size
};
