#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lcd.c"
#include "ballistics.h"

#define NUM_ROWS 4
#define NUM_COLS 4

char displayedString[32];  // Keep track of the displayed string
int displayedIndex = 0;

float number = 0.0;  // The number being entered
int decimalPlace = 0;  // The current decimal place, 0 means before decimal point

double values[8] = {0}; // Array to hold ballistic values
int current_value = 0;	//The current value that the user is entering
char* value_names[8] = {"Drag Coefficient", "Initial Velocity", "Sight Height", "Shooting Angle", "Wind Speed", "Wind Angle", "Zero Range", "yIntercept"};


// Keypad layout
char keypad[NUM_ROWS][NUM_COLS] = {
	{'1', '2', '3','A'},
	{'4', '5', '6','B'},
	{'7', '8', '9','C'},
	{'-', '0', '.','s'}
};

void initKeypad(){
	// Set rows as output and columns as input with pull-up resistors enabled
	DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3);
	DDRC &= ~((1 << PC0) | (1 << PC1) | (1 << PC2));
	PORTC = 0x00;//(1 << PC0) | (1 << PC1) | (1 << PC2);
	PORTB = 0x00;
}

int buttonPressed() {
	// Scan the matrix for the 'B' button
	for (int row = 0; row < NUM_ROWS; row++) {
		PORTB = 0x01 << row;  // Select the current row
		_delay_ms(10);  // Wait for the signal to stabilize

		for (int col = 0; col < NUM_COLS; col++) {
			if (PINC & (0x01 << col)) {  // Check if the current column is high
				if (keypad[row][col] == 'B') {  // Check if the 'B' button is pressed
					while (PINC & (0x01 << col));  // Wait for the button to be released
					return 1;  // Return 1 if the 'B' button is pressed
				}
			}
		}
	}

	return 0;  // Return 0 if the 'B' button is not pressed
}


void handleButtonPress(char button){
	if (button == '-'){
		if (displayedIndex > 0){
			displayedIndex--;
			char lastChar = displayedString[displayedIndex];
			displayedString[displayedIndex] = '\0';  // Delete the last character
			LCD_Command(0x01);  // Clear the display
			LCD_String(displayedString);  // Refresh the display

			// Update the number variable
			if (lastChar == '.'){
				decimalPlace = 0;
				} else if (decimalPlace > 0){
				decimalPlace--;
				number -= (lastChar - '0') / pow(10, decimalPlace);
				} else {
				number = (int)(number / 10);
			}
		}
	}
	else if (button == '.'){
		if (decimalPlace == 0){  // Only accept the first decimal point
			decimalPlace = 1;
			displayedString[displayedIndex] = button;
			displayedIndex++;
			LCD_Char(button);
		}
	}
	else if(button >= '0' && button <= '9'){ // Number button
		float digit = button - '0';
		if (decimalPlace > 0){
			for (int i = 0; i < decimalPlace; i++){
				digit /= 10.0;
			}
			decimalPlace++;
			} else {
			number *= 10.0;
		}
		number += digit;
		displayedString[displayedIndex] = button;
		displayedIndex++;
		LCD_Char(button);
	}
	else if (button == 's'){  // Next parameter button
		// Store the current number in the appropriate parameter
		values[current_value] = number;

		// Reset the current number and move to the next parameter
		number = 0.0;
		decimalPlace = 0;
		current_value++;

		// Display the prompt for the next parameter
		LCD_Command(0x01);  // Clear the display
		if (current_value < 7) {
			LCD_String("Enter ");
			LCD_String(value_names[current_value]);
			LCD_String(":");
		}
		else if (current_value >= 8){ 
			// Perform the ballistic calculations
			double zeroAngle = ZeroAngle(G1, values[0], values[1], values[2], values[6], values[7]);
			
			double* solution;
			int maxRange = SolveAll(G1, values[0], values[1], values[2], values[3], zeroAngle, values[4], values[5], &solution);

			// Display the results
			// Display the results
			for (int i = 0; i <= maxRange; i++) {
				double path = GetPath(solution, i);
				double moa = GetMOA(solution, i);
				double windage = GetWindage(solution, i);
				double windageMOA = GetWindageMOA(solution, i);

				// Convert the results to strings
				char resultStr[50];
				sprintf(resultStr, "Range: %d\nPath: %.2f\nMOA: %.2f\nWindage: %.2f\nWindage MOA: %.2f", i, path, moa, windage, windageMOA);

				// Display the results on the LCD
				LCD_Command(0x01);  // Clear the display
				LCD_String(resultStr);

				// Wait for a button press before displaying the next range
				while (!buttonPressed());
			}
		}

	}
}


void scanMatrix(){
	for (int row=0; row<NUM_ROWS; row++){
		PORTB = 0x01 << row;
		_delay_ms(10);
		
		for (int col=0; col<NUM_COLS; col++){
			if (PINC & (0x01 << col)){
				handleButtonPress(keypad[row][col]);

				//while(PINC & (0x01 << col));
				_delay_ms(200);
			}
		}

		PORTB = 0x00;
	}
}

int main(void) {
	LCD_Init();
	initKeypad();
	
	LCD_String("Enter Drag Coefficient:");
	
	while(1){
		scanMatrix();
		_delay_ms(10);
	}
}