#ifndef alledge_view_h
#define alledge_view_h

//Pushes current projection matrix and sets up perspective projection.
void Init_perspective_view(float fov, float aspect, float near, float far);

//Pops a previously pushed view.
void Pop_view();

#endif
