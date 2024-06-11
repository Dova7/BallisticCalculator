// Specialty angular conversion functions
float DegtoMOA(float deg) {
	return deg * 60;
}
float DegtoRad(float deg) {
	return deg * 3.14159265358979323846 / 180;
}
float MOAtoDeg(float moa) {
	return moa / 60;
}
float MOAtoRad(float moa) {
	return moa / 60 * 3.14159265358979323846 / 180;
}
float RadtoDeg(float rad) {
	return rad * 180 / 3.14159265358979323846;
}
float RadtoMOA(float rad) {
	return rad * 60 * 180 / 3.14159265358979323846;
}