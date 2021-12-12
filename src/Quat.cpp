#include "../alledge/Quat.h"
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <string.h>

/**
 * Basic quaternion operations.
 */

Quaternion::Quaternion() {
	q[X] = 0;
	q[Y] = 0;
	q[Z] = 0;
	q[W] = 1;
}

const Quaternion& Quaternion::operator=(const Quaternion &in) {
	q[X] = in.q[X];
	q[Y] = in.q[Y];
	q[Z] = in.q[Z];
	q[W] = in.q[W];
	return in;
}

void Quaternion::From_axisangle(float x, float y, float z, float a) {
	q[X] = x * sin( a/2);
	q[Y] = y * sin( a/2);
	q[Z] = z * sin( a/2);
	q[W] = cos( a/2);
}

void Quaternion::From_axisangle(Vector3 v, float w) {
	From_axisangle(v.x, v.y, v.z, w);
}

void Quaternion::ComputeW() {
  float t = 1.0f - (q[X] * q[X]) - (q[Y] * q[Y]) - (q[Z] * q[Z]);

  if (t < 0.0f)
    q[W] = 0.0f;
  else
    q[W] = -sqrt (t);
}

void Quaternion::Normalize() {
	/* compute magnitude of the quaternion */
	float mag = sqrt ((q[X] * q[X]) + (q[Y] * q[Y]) + (q[Z] * q[Z]) + (q[W] * q[W]));

  /* check for bogus length, to protect against divide by zero */
	if (mag > 0.0f)
	{
		/* normalize it */
		float oneOverMag = 1.0f / mag;

		q[X] *= oneOverMag;
		q[Y] *= oneOverMag;
		q[Z] *= oneOverMag;
		q[W] *= oneOverMag;
	}
}

Quaternion Quaternion::MultQuat(const Quaternion &in) {
	const quat4_t &qb = in.q;
	Quaternion out;
  out.q[W] = (q[W] * qb[W]) - (q[X] * qb[X]) - (q[Y] * qb[Y]) - (q[Z] * qb[Z]);
  out.q[X] = (q[X] * qb[W]) + (q[W] * qb[X]) + (q[Y] * qb[Z]) - (q[Z] * qb[Y]);
  out.q[Y] = (q[Y] * qb[W]) + (q[W] * qb[Y]) + (q[Z] * qb[X]) - (q[X] * qb[Z]);
  out.q[Z] = (q[Z] * qb[W]) + (q[W] * qb[Z]) + (q[X] * qb[Y]) - (q[Y] * qb[X]);
  return out;
}

Quaternion Quaternion::MultVec(Vector3 v) {
	Quaternion out;
  out.q[W] = - (q[X] * v.x) - (q[Y] * v.y) - (q[Z] * v.z);
  out.q[X] =   (q[W] * v.x) + (q[Y] * v.z) - (q[Z] * v.y);
  out.q[Y] =   (q[W] * v.y) + (q[Z] * v.x) - (q[X] * v.z);
  out.q[Z] =   (q[W] * v.z) + (q[X] * v.y) - (q[Y] * v.x);
  return out;
}

Vector3 Quaternion::Quat_rotatePoint(const Vector3 &in) {
  Quaternion tmp;
  Quaternion inv;
  Quaternion final;

  inv.q[X] = -q[X];
  inv.q[Y] = -q[Y];
  inv.q[Z] = -q[Z];
  inv.q[W] =  q[W];

  inv.Normalize();

  tmp = MultVec(in);
  final = tmp.MultQuat(inv);

  return Vector3(final.q[X], final.q[Y], final.q[Z]);
}

float Quaternion::DotProduct(const Quaternion &qb) {
  return ((q[X] * qb.q[X]) + (q[Y] * qb.q[Y]) + (q[Z] * qb.q[Z]) + (q[W] * qb.q[W]));
}

Quaternion Quaternion::Slerp(const Quaternion &qb, float t) {
	Quaternion out;
	Quat_slerp(q, qb.q, t, out.q);
	return out;
}

Matrix4 To_matrix4() {
	return Matrix4();
}

Vector3 To_euler() {
	return Vector3();
}


void Quat_init(quat4_t q)
{
	q[X] = 0;
	q[Y] = 0;
	q[Z] = 0;
	q[W] = 1;
}

void Quat_copy(const quat4_t in, quat4_t out)
{
	out[X] = in[X];
	out[Y] = in[Y];
	out[Z] = in[Z];
	out[W] = in[W];
}

void Quat_from_axisangle(quat4_t q, float x, float y, float z, float a)
{
	q[X] = x * sin( a/2);
	q[Y] = y * sin( a/2);
	q[Z] = z * sin( a/2);
	q[W] = cos( a/2);
}

void
Quat_computeW (quat4_t q)
{
  float t = 1.0f - (q[X] * q[X]) - (q[Y] * q[Y]) - (q[Z] * q[Z]);

  if (t < 0.0f)
    q[W] = 0.0f;
  else
    q[W] = -sqrt (t);
}

void
Quat_normalize (quat4_t q)
{
  /* compute magnitude of the quaternion */
  float mag = sqrt ((q[X] * q[X]) + (q[Y] * q[Y])
		    + (q[Z] * q[Z]) + (q[W] * q[W]));

  /* check for bogus length, to protect against divide by zero */
  if (mag > 0.0f)
    {
      /* normalize it */
      float oneOverMag = 1.0f / mag;

      q[X] *= oneOverMag;
      q[Y] *= oneOverMag;
      q[Z] *= oneOverMag;
      q[W] *= oneOverMag;
    }
}

void
Quat_multQuat (const quat4_t qa, const quat4_t qb, quat4_t out)
{
  out[W] = (qa[W] * qb[W]) - (qa[X] * qb[X]) - (qa[Y] * qb[Y]) - (qa[Z] * qb[Z]);
  out[X] = (qa[X] * qb[W]) + (qa[W] * qb[X]) + (qa[Y] * qb[Z]) - (qa[Z] * qb[Y]);
  out[Y] = (qa[Y] * qb[W]) + (qa[W] * qb[Y]) + (qa[Z] * qb[X]) - (qa[X] * qb[Z]);
  out[Z] = (qa[Z] * qb[W]) + (qa[W] * qb[Z]) + (qa[X] * qb[Y]) - (qa[Y] * qb[X]);
}

void
Quat_multVec (const quat4_t q, const vec3_t v, quat4_t out)
{
  out[W] = - (q[X] * v[X]) - (q[Y] * v[Y]) - (q[Z] * v[Z]);
  out[X] =   (q[W] * v[X]) + (q[Y] * v[Z]) - (q[Z] * v[Y]);
  out[Y] =   (q[W] * v[Y]) + (q[Z] * v[X]) - (q[X] * v[Z]);
  out[Z] =   (q[W] * v[Z]) + (q[X] * v[Y]) - (q[Y] * v[X]);
}

void
Quat_rotatePoint (const quat4_t q, const vec3_t in, vec3_t out)
{
  quat4_t tmp, inv, final;

  inv[X] = -q[X]; inv[Y] = -q[Y];
  inv[Z] = -q[Z]; inv[W] =  q[W];

  Quat_normalize (inv);

  Quat_multVec (q, in, tmp);
  Quat_multQuat (tmp, inv, final);

  out[X] = final[X];
  out[Y] = final[Y];
  out[Z] = final[Z];
}

/**
 * More quaternion operations for skeletal animation.
 */

float
Quat_dotProduct (const quat4_t qa, const quat4_t qb)
{
  return ((qa[X] * qb[X]) + (qa[Y] * qb[Y]) + (qa[Z] * qb[Z]) + (qa[W] * qb[W]));
}

void
Quat_slerp (const quat4_t qa, const quat4_t qb, float t, quat4_t out)
{
  /* Check for out-of range parameter and return edge points if so */
  if (t <= 0.0)
    {
      memcpy (out, qa, sizeof(quat4_t));
      return;
    }

  if (t >= 1.0)
    {
      memcpy (out, qb, sizeof (quat4_t));
      return;
    }

  /* Compute "cosine of angle between quaternions" using dot product */
  float cosOmega = Quat_dotProduct (qa, qb);

  /* If negative dot, use -q1.  Two quaternions q and -q
     represent the same rotation, but may produce
     different slerp.  We chose q or -q to rotate using
     the acute angle. */
  float q1w = qb[W];
  float q1x = qb[X];
  float q1y = qb[Y];
  float q1z = qb[Z];

  if (cosOmega < 0.0f)
    {
      q1w = -q1w;
      q1x = -q1x;
      q1y = -q1y;
      q1z = -q1z;
      cosOmega = -cosOmega;
    }

  /* We should have two unit quaternions, so dot should be <= 1.0 */
  assert (cosOmega < 1.1f);

  /* Compute interpolation fraction, checking for quaternions
     almost exactly the same */
  float k0, k1;

  if (cosOmega > 0.9999f)
    {
      /* Very close - just use linear interpolation,
	 which will protect againt a divide by zero */

      k0 = 1.0f - t;
      k1 = t;
    }
  else
    {
      /* Compute the sin of the angle using the
	 trig identity sin^2(omega) + cos^2(omega) = 1 */
      float sinOmega = sqrt (1.0f - (cosOmega * cosOmega));

      /* Compute the angle from its sin and cosine */
      float omega = atan2 (sinOmega, cosOmega);

      /* Compute inverse of denominator, so we only have
	 to divide once */
      float oneOverSinOmega = 1.0f / sinOmega;

      /* Compute interpolation parameters */
      k0 = sin ((1.0f - t) * omega) * oneOverSinOmega;
      k1 = sin (t * omega) * oneOverSinOmega;
    }

  /* Interpolate and return new quaternion */
  out[W] = (k0 * qa[3]) + (k1 * q1w);
  out[X] = (k0 * qa[0]) + (k1 * q1x);
  out[Y] = (k0 * qa[1]) + (k1 * q1y);
  out[Z] = (k0 * qa[2]) + (k1 * q1z);
}

void Set_matrix4(matrix4_t m_matrix,
					float m11, float m12, float m13, float m14,
					float m21, float m22, float m23, float m24,
					float m31, float m32, float m33, float m34,
					float m41, float m42, float m43, float m44)
{
			m_matrix[0] = m11;
			m_matrix[1] = m12;
			m_matrix[2] = m13;
			m_matrix[3] = m14;
			m_matrix[4] = m21;
			m_matrix[5] = m22;
			m_matrix[6] = m23;
			m_matrix[7] = m24;
			m_matrix[8] = m31;
			m_matrix[9] = m32;
			m_matrix[10] = m33;
			m_matrix[11] = m34;
			m_matrix[12] = m41;
			m_matrix[13] = m42;
			m_matrix[14] = m43;
			m_matrix[15] = m44;
}

void Quat_to_matrix4 (const quat4_t q, matrix4_t mo)
{
	float qx = q[0];
	float qy = q[1];
	float qz = q[2];
	float qw = q[3];
	Set_matrix4(mo,
		(1 - 2 * qy * qy - 2 * qz * qz),
		(2 * qx * qy - 2 * qz * qw),
		(2 * qx * qz + 2 * qy * qw),
		0,

		(2 * qx * qy + 2 * qz * qw),
		(1 - 2 * qx * qx - 2 * qz * qz),
		(2 * qy * qz - 2 * qx * qw),
		0,

		(2 * qx * qz - 2 * qy * qw),
		(2 * qy * qz + 2 * qx * qw),
		(1 - 2 * qx * qx - 2 * qy * qy),
		0,
		
		0,
		0,
		0,
		1);
}

void Quat_to_euler(quat4_t q1, vec3_t out) {
	Quat_normalize(q1);
	float heading;
	float attitude;
	float bank;
	double test = q1[X]*q1[Y] + q1[Z]*q1[W];
	if (test > 0.499) { // singularity at north pole
		heading = 2 * atan2(q1[X],q1[W]);
		attitude = M_PI/2;
		bank = 0;
		return;
	}
	if (test < -0.499) { // singularity at south pole
		heading = -2 * atan2(q1[X],q1[W]);
		attitude = - M_PI/2;
		bank = 0;
		return;
	}
    double sqx = q1[X]*q1[X];
    double sqy = q1[Y]*q1[Y];
    double sqz = q1[Z]*q1[Z];
	attitude = asin(2*test);	
    heading = atan2(2*q1[Y]*q1[W]-2*q1[X]*q1[Z] , 1 - 2*sqy - 2*sqz); 
	bank = atan2(2*q1[X]*q1[W]-2*q1[Y]*q1[Z] , 1 - 2*sqx - 2*sqz); 
	
	out[0] = attitude; 	//Pitch
	out[1] = heading;  	//yaw
	out[2] = bank;		//Roll
}
