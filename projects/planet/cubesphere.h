Vector3 CubeToSphere(Vector3 c) {
	float x2 = c.x*c.x;
	float y2 = c.y*c.y;
	float z2 = c.z*c.z;
	float x = c.x * sqrt(1 - (y2 + z2) / 2 + (y2 * z2) / 3);
	float y = c.y * sqrt(1 - (z2 + x2) / 2 + (z2 * x2) / 3);
	float z = c.z * sqrt(1 - (x2 + y2) / 2 + (x2 * y2) / 3);
	return Vector3(x, y, z);
}

Vector3 SphereToCube(Vector3& vin, float facevalue = 1.0) {
	Vector3 position;
	double x,y,z;
	x = vin.x;
	y = vin.y;
	z = vin.z;

	double fx, fy, fz;
	fx = fabsf(x);
	fy = fabsf(y);
	fz = fabsf(z);

	const double inverseSqrt2 = 0.70710676908493042;

	if (fy >= fx && fy >= fz) {
		double a2 = x * x * 2.0;
		double b2 = z * z * 2.0;
		double inner = -a2 + b2 -3;
		double innersqrt = -sqrtf((inner * inner) - 12.0 * a2);

		if(x == 0.0 || x == -0.0) { 
			position.x = 0.0; 
		}
		else {
			position.x = sqrtf(innersqrt + a2 - b2 + 3.0) * inverseSqrt2;
		}

		if(z == 0.0 || z == -0.0) {
			position.z = 0.0;
		}
		else {
			position.z = sqrtf(innersqrt - a2 + b2 + 3.0) * inverseSqrt2;
		}

		if(position.x > 1.0) position.x = 1.0;
		if(position.z > 1.0) position.z = 1.0;

		if(x < 0) position.x = -position.x;
		if(z < 0) position.z = -position.z;

		if (y > 0) {
			// top face
			position.y = facevalue;
		}
		else {
			// bottom face
			position.y = -facevalue;
		}
	}
	else if (fx >= fy && fx >= fz) {
		double a2 = y * y * 2.0;
		double b2 = z * z * 2.0;
		double inner = -a2 + b2 -3;
		double innersqrt = -sqrtf((inner * inner) - 12.0 * a2);

		if(y == 0.0 || y == -0.0) { 
			position.y = 0.0; 
		}
		else {
			position.y = sqrtf(innersqrt + a2 - b2 + 3.0) * inverseSqrt2;
		}

		if(z == 0.0 || z == -0.0) {
			position.z = 0.0;
		}
		else {
			position.z = sqrtf(innersqrt - a2 + b2 + 3.0) * inverseSqrt2;
		}

		if(position.y > 1.0) position.y = 1.0;
		if(position.z > 1.0) position.z = 1.0;

		if(y < 0) position.y = -position.y;
		if(z < 0) position.z = -position.z;

		if (x > 0) {
			// right face
			position.x = facevalue;
		}
		else {
			// left face
			position.x = -facevalue;
		}
	}
	else {
		double a2 = x * x * 2.0;
		double b2 = y * y * 2.0;
		double inner = -a2 + b2 -3;
		double innersqrt = -sqrtf((inner * inner) - 12.0 * a2);

		if(x == 0.0 || x == -0.0) { 
			position.x = 0.0; 
		}
		else {
			position.x = sqrtf(innersqrt + a2 - b2 + 3.0) * inverseSqrt2;
		}

		if(y == 0.0 || y == -0.0) {
			position.y = 0.0;
		}
		else {
			position.y = sqrtf(innersqrt - a2 + b2 + 3.0) * inverseSqrt2;
		}

		if(position.x > 1.0) position.x = 1.0;
		if(position.y > 1.0) position.y = 1.0;

		if(x < 0) position.x = -position.x;
		if(y < 0) position.y = -position.y;

		if (z > 0) {
			// front face
			position.z = facevalue;
		}
		else {
			// back face
			position.z = -facevalue;
		}
	}
	return position;
}
