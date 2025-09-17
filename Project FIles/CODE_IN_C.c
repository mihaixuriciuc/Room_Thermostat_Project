#include <reg51.h>

// Define LCD connections
sbit RS = P0^0;
sbit RW = P0^1;
sbit EN = P0^2;
#define LCD_DATA P2  // LCD data lines connected to Port 2

#define ADC_INPUT P1 // ADC outputs connected to Port 1

// Function prototypes
void lcd_init(void);
void lcd_cmd(unsigned char);
void lcd_data(unsigned char);
void lcd_string(char *);
void lcd_busy(void);
void lcd_delay(unsigned int);

unsigned char read_adc(void);
unsigned int adc_to_temperature(unsigned char adc_value);

void main()
{
    unsigned char adc_value;
    unsigned int temperature;
    char buffer[5];

    lcd_init();  // Initialize LCD

    while (1)
    {
        adc_value = read_adc();      // Directly Read ADC value from P1
        temperature = adc_to_temperature(adc_value); // Convert to temperature

        lcd_cmd(0x80);               // Move cursor to beginning
        lcd_string("Temp: ");

        // Convert temperature to ASCII string manually
        buffer[0] = (temperature / 10) + '0';  // Tens
        buffer[1] = (temperature % 10) + '0';  // Units
        buffer[2] = 0;                         // Null terminator

        lcd_string(buffer);                    // Display temperature
        lcd_data(0xDF);                        // Display degree symbol (°)
        lcd_data('C');                         // Display 'C'

        lcd_delay(50000);                      // Small delay
    }
}

// LCD Functions

void lcd_init()
{
    lcd_cmd(0x38); // 8-bit mode, 2-line, 5x7 font
    lcd_cmd(0x0C); // Display ON, Cursor OFF
    lcd_cmd(0x06); // Entry mode
    lcd_cmd(0x01); // Clear display
    lcd_delay(2000);
}

void lcd_cmd(unsigned char cmd)
{
    lcd_busy();
    LCD_DATA = cmd;
    RS = 0;
    RW = 0;
    EN = 1;
    lcd_delay(1);
    EN = 0;
}

void lcd_data(unsigned char dat)
{
    lcd_busy();
    LCD_DATA = dat;
    RS = 1;
    RW = 0;
    EN = 1;
    lcd_delay(1);
    EN = 0;
}

void lcd_string(char *str)
{
    while (*str)
    {
        lcd_data(*str++);
    }
}

void lcd_busy()
{
    lcd_delay(10); // Simple fixed delay instead of checking busy flag
}

void lcd_delay(unsigned int time)
{
    while (time--);
}


unsigned char read_adc(void)
{
    return ADC_INPUT; // Directly read ADC data from P1
}

// Compute temperature based on ADC reading
unsigned int adc_to_temperature(unsigned char adc_value)
{
    unsigned int temp;

    // Temperature = (adc_value * 30) / 255 + 5
    temp = ((unsigned int)adc_value * 30) / 255;
    temp += 5;

    return temp;
}.  can you explain me how this code works entirely and explain me the concepts