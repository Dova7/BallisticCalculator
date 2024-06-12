float calcFR(float Temperature, float Pressure, float RelativeHumidity) {
	float VPw = roundf((4e-6 * pow(Temperature, 3) - 0.0004 * pow(Temperature, 2)
	+ 0.0234 * Temperature - 0.2517) * 100) / 100;
	float FRH = roundf(0.995 * (Pressure / (Pressure - 0.3783 * RelativeHumidity * VPw)) * 100) / 100;
	return FRH;
}

float calcFP(float Pressure) {
	float Pstd = 29.53; // in-hg
	float FP = roundf(((Pressure - Pstd) / Pstd) * 100) / 100;
	return FP;
}

float calcFT(float Temperature, float Altitude) {
	float Tstd = -0.0036 * Altitude + 59;
	float FT = roundf(((Temperature - Tstd) / (459.6 + Tstd)) * 100) / 100;
	return FT;
}

float calcFA(float Altitude) {
	float fa = roundf((-4e-15 * pow(Altitude, 3) + 4e-10 * pow(Altitude, 2) - 3e-5 * Altitude + 1) * 100) / 100;
	return 1 / fa;
}

float AtmCorrect(float DragCoefficient, float Altitude, float Barometer,
float Temperature, float RelativeHumidity) {

	float FA = calcFA(Altitude);
	float FT = calcFT(Temperature, Altitude);
	float FR = calcFR(Temperature, Barometer, RelativeHumidity);
	float FP = calcFP(Barometer);

	// Calculate the atmospheric correction factor
	float CD = roundf((FA * (1 + FT - FP) * FR) * 100) / 100;
	return DragCoefficient * CD;
}