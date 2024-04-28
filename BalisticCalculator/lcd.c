#define EN PD1
#define RS PD0

#define LCD_Dir  DDRD			// Define LCD data port direction
#define LCD_Port PORTD			// Define LCD data port

void LCD_Command(unsigned char cmnd){
	LCD_Port = (LCD_Port & 0x0F) | (cmnd & 0xF0); // sending upper nibble
	LCD_Port &= ~ (1<<RS);		// RS=0, command reg.
	LCD_Port |= (1<<EN);		// Enable pulse
	_delay_us(1);
	LCD_Port &= ~ (1<<EN);

	_delay_us(200);

	LCD_Port = (LCD_Port & 0x0F) | (cmnd << 4);  // sending lower nibble
	LCD_Port |= (1<<EN);
	_delay_us(1);
	LCD_Port &= ~ (1<<EN);
	_delay_ms(2);
}

void LCD_Char(unsigned char data){
	LCD_Port  = (LCD_Port & 0x0F) | (data & 0xF0); // sending upper nibble
	LCD_Port |= (1<<RS);		// RS=1, data reg.
	LCD_Port |= (1<<EN);
	_delay_us(1);
	LCD_Port &= ~ (1<<EN);

	_delay_us(200);

	LCD_Port = (LCD_Port & 0x0F) | (data << 4); // sending lower nibble
	LCD_Port |= (1<<EN);
	_delay_us(1);
	LCD_Port &= ~ (1<<EN);
	_delay_ms(2);
}

void LCD_String(char const *str){
	for(int i=0; str[i]!=0; i++){
		LCD_Char(str[i]);
	}
}

void LCD_Init(void){
	LCD_Dir = 0xFF;			// Make LCD port direction as o/p
	_delay_ms(20);			// LCD Power ON delay always >15ms
	
	LCD_Command(0x02);		// send for 4 bit initialization of LCD
	LCD_Command(0x28);      // 2 line, 5*7 matrix in 4-bit mode
	LCD_Command(0x0c);      // Display on cursor off
	LCD_Command(0x06);      // Increment cursor (shift cursor to right)
	LCD_Command(0x01);      // Clear display screen
	_delay_ms(2);
}
