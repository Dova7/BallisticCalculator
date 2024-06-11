#ifndef __BCOMP_MAXRANGE__
#define __BCOMP_MAXRANGE__ (5000)
#endif

#ifndef GRAVITY
#define GRAVITY (-32.194)
#endif

int SolveNext(int DragFunction, float DragCoefficient, float Vi,
float SightHeight, float ShootingAngle, float ZAngle,
float WindSpeed, float WindAngle, float* Solution) {

	float t = 0;
	float dt = 0.5 / Vi;
	float v = 0;
	float vx = 0, vx1 = 0, vy = 0, vy1 = 0;
	float dv = 0, dvx = 0, dvy = 0;
	float x = 0, y = 0;

	float headwind = HeadWind(WindSpeed, WindAngle);
	float crosswind = CrossWind(WindSpeed, WindAngle);

	float Gy = GRAVITY * cos(DegtoRad((ShootingAngle + ZAngle)));
	float Gx = GRAVITY * sin(DegtoRad((ShootingAngle + ZAngle)));

	vx = Vi * cos(DegtoRad(ZAngle));
	vy = Vi * sin(DegtoRad(ZAngle));

	y = -SightHeight / 12;

	for (t = 0;; t = t + dt) {
		vx1 = vx, vy1 = vy;
		v = pow(pow(vx, 2) + pow(vy, 2), 0.5);
		dt = 0.5 / v;

		// Compute acceleration using the drag function retardation
		dv = retard(DragFunction, DragCoefficient, v + headwind);
		dvx = -(vx / v) * dv;
		dvy = -(vy / v) * dv;

		// Compute velocity, including the resolved gravity vectors.
		vx = vx + dt * dvx + dt * Gx;
		vy = vy + dt * dvy + dt * Gy;

		if ((int)(x / 3) % 50 == 0) {
			Solution[0] = x / 3;                                          // Range in yds
			Solution[1] = y * 12;                                         // Path in inches
			Solution[2] = -RadtoMOA(atan(y / x));                         // Correction in MOA
			Solution[3] = t + dt;                                         // Time in s
			Solution[4] = Windage(crosswind, Vi, x, t + dt);               // Windage in inches
			Solution[5] = RadtoMOA(atan(Solution[4]));                     // Windage in MOA
			Solution[6] = v;                                              // Velocity (combined)
			Solution[7] = vx;                                             // Velocity (x)
			Solution[8] = vy;                                             // Velocity (y)
			Solution[9] = 0;                                              // Reserved
			return 0;  // Return success
		}

		// Compute position based on average velocity.
		x = x + dt * (vx + vx1) / 2;
		y = y + dt * (vy + vy1) / 2;

		if (fabs(vy) > fabs(3 * vx)) break;
	}

	return -1;  // Return -1 if no data point was calculated
}
