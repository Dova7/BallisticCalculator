#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include "lcd.c"

#define NUM_ROWS 4
#define NUM_COLS 4

char displayedString[32];  // Keep track of the displayed string
int displayedIndex = 0;

float number = 0.0;  // The number being entered
int decimalPlace = 0;  // The current decimal place, 0 means before decimal point



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

void handleButtonPress(char button){
	if (button == '-'){
		if (displayedIndex > 0){
			displayedIndex--;
			displayedString[displayedIndex] = '\0';  // Delete the last character
			LCD_Command(0x01);  // Clear the display
			LCD_String(displayedString);  // Refresh the display
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
	else if (button == 's'){  // Submit button
		// Display the saved number
		char numStr[32];
		char buffer[64];
		dtostrf(number, 3, 2, numStr);
		sprintf(buffer, "number = %s", numStr);  // Use numStr here
		LCD_Command(0x01);  // Clear the display
		LCD_String(buffer);  // Display the saved number

		// Reset the number and displayed string
		number = 0.0;
		decimalPlace = 0;
		displayedIndex = 0;
		displayedString[displayedIndex] = '\0';
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
	
	while(1){
		scanMatrix();
		_delay_ms(10);
	}
}
