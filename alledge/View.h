#ifndef alledge_view_h
#define alledge_view_h

#include "Cameranode.h"
#include "shared_ptr.h"

//Pushes current projection matrix and sets up perspective projection.
void Init_perspective_view(float fov, float aspect, float near, float far);

//Pops a previously pushed view.
void Pop_view();

/* Gets 3D coordinate of the pixel rendered at x, y.
 * Usage:
 * 		Init_perspective_view(fov, width/height, near, far);
		Vector3 oglpoint = Unproject(mouse_x, mouse_y, camera);
		Pop_view();
 */
Vector3 Unproject(int x, int y, shared_ptr<Cameranode> camera);

#endif
