#ifndef __BCOMP_MAXRANGE__
#define __BCOMP_MAXRANGE__ (2000)
#endif

float GetRange(float* sln, int yardage) {
	float size = sln[__BCOMP_MAXRANGE__ * 10 + 1];
	if (yardage < size) {
		return sln[10 * yardage];
		} else {
		return 0;
	}
}

float GetPath(float* sln, int yardage) {
	float size = sln[__BCOMP_MAXRANGE__ * 10 + 1];
	if (yardage < size) {
		return sln[10 * yardage + 1];
		} else {
		return 0;
	}
}

float GetMOA(float* sln, int yardage) {
	float size = sln[__BCOMP_MAXRANGE__ * 10 + 1];
	if (yardage < size) {
		return sln[10 * yardage + 2];
		} else {
		return 0;
	}
}

float GetTime(float* sln, int yardage) {
	float size = sln[__BCOMP_MAXRANGE__ * 10 + 1];
	if (yardage < size) {
		return sln[10 * yardage + 3];
		} else {
		return 0;
	}
}

float GetWindage(float* sln, int yardage) {
	float size = sln[__BCOMP_MAXRANGE__ * 10 + 1];
	if (yardage < size) {
		return sln[10 * yardage + 4];
		} else {
		return 0;
	}
}

float GetWindageMOA(float* sln, int yardage) {
	float size = sln[__BCOMP_MAXRANGE__ * 10 + 1];
	if (yardage < size) {
		return sln[10 * yardage + 5];
	} else
	return 0;
}

float GetVelocity(float* sln, int yardage) {
	float size = sln[__BCOMP_MAXRANGE__ * 10 + 1];
	if (yardage < size) {
		return sln[10 * yardage + 6];
	} else
	return 0;
}

float GetVx(float* sln, int yardage) {
	float size = sln[__BCOMP_MAXRANGE__ * 10 + 1];
	if (yardage < size) {
		return sln[10 * yardage + 7];
	} else
	return 0;
}

float GetVy(float* sln, int yardage) {
	float size = sln[__BCOMP_MAXRANGE__ * 10 + 1];
	if (yardage < size) {
		return sln[10 * yardage + 8];
	} else
	return 0;
}