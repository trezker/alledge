#ifndef __QUAT_H__
#define __QUAT_H__

#include "Vector3.h"
#include "Matrix4.h"

typedef float vec3_t[3];

/* Quaternion (x, y, z, w) */
typedef float quat4_t[4];

typedef float matrix4_t[16];

enum {
  X, Y, Z, W
};

/**
 * Quaternion prototypes
 */
void Quat_init(quat4_t q);
void Quat_copy(const quat4_t in, quat4_t out);
void Quat_from_axisangle(quat4_t q, float x, float y, float z, float w);

void Quat_computeW (quat4_t q);
void Quat_normalize (quat4_t q);
void Quat_multQuat (const quat4_t qa, const quat4_t qb, quat4_t out);
void Quat_multVec (const quat4_t q, const vec3_t v, quat4_t out);
void Quat_rotatePoint (const quat4_t q, const vec3_t in, vec3_t out);
float Quat_dotProduct (const quat4_t qa, const quat4_t qb);
void Quat_slerp (const quat4_t qa, const quat4_t qb, float t, quat4_t out);

void Quat_to_matrix4 (const quat4_t qa, matrix4_t mo);
void Quat_to_euler(quat4_t q1, vec3_t out);

class Quaternion {
private:
  quat4_t q;
public:
  Quaternion();
  const Quaternion& operator=(const Quaternion &q);
  void From_axisangle(float x, float y, float z, float a);
  void From_axisangle(Vector3 v, float w);
  void ComputeW();
  void Normalize();
  Quaternion MultQuat(const Quaternion &qb);
  Quaternion MultVec(Vector3 v);
  Vector3 Quat_rotatePoint(const Vector3 &in);
  float DotProduct(const Quaternion &qb);
  Quaternion Slerp(const Quaternion &qb, float t);

  Matrix4 To_matrix4();
  Vector3 To_euler();
};

#endif
