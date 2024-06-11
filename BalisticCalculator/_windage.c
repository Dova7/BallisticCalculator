float Windage(float WindSpeed, float Vi, float xx, float t) {
	float Vw = WindSpeed * 17.60; // Convert to inches per second.
	return (Vw * (t - xx / Vi));
}

// Headwind is positive at WindAngle=0
float HeadWind(float WindSpeed, float WindAngle) {
	float Wangle = DegtoRad(WindAngle);
	return (cos(Wangle) * WindSpeed);
}

// Positive is from Shooter's Right to Left (Wind from 90 degree)
float CrossWind(float WindSpeed, float WindAngle) {
	float Wangle = DegtoRad(WindAngle);
	return (sin(Wangle) * WindSpeed);
}