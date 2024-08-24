#include <reg51.h>

// Define port pins for start and stop buttons
#define START_BUTTON P3^0
#define STOP_BUTTON P3^2

// Define output pins and their corresponding output and response times
#define NUM_OUTPUTS 5

typedef struct {
    unsigned char port;
    unsigned char pin;
    unsigned int output_time;
    unsigned int response_time;
} OutputStep;

OutputStep output_sequence[NUM_OUTPUTS] = {
    {1, 0, 10, 1},  // P1^0
    {1, 2, 17, 2},  // P1^2
    {2, 7, 30, 1},  // P2^7
    {3, 1, 32, 3},  // P3^1
    {2, 6, 40, 1}   // P2^6
};

// Function prototypes
void generate_output(OutputStep step);
void wait_ms(unsigned int ms);
void init_timer(void);
void timer_isr(void) interrupt 1;
unsigned char check_stop_button();

// Global variables for timing
unsigned int tick_count = 0;
unsigned int current_step = 0;
bit running = 0;

void main() {
    // Initialize timer and start button
    init_timer();
    
    // Main loop
    while (1) {
        // Check if start button pressed
        if (START_BUTTON) {
            if (!running) {
                running = 1;
                tick_count = 0; // Reset tick count
                current_step = 0; // Start with the first step
            }
        }

        // Check if stop button pressed
        if (check_stop_button()) {
            running = 0; // Stop the system
            current_step = 0; // Reset step
        }
        
        // Enter idle mode to save power
        PCON |= 0x01; // Enter idle mode
        
        // Wait for an interrupt to wake up
        NOP(); // No operation to keep the CPU idle
    }
}

void generate_output(OutputStep step) {
    // Turn on the output pin
    *(unsigned char *)(step.port * 0x08 + 0x80 + step.pin) = 1;
    // Wait for specified duration
    wait_ms(step.output_time);
    // Turn off the output pin
    *(unsigned char *)(step.port * 0x08 + 0x80 + step.pin) = 0;
}

void wait_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++) {
        for (j = 0; j < 112; j++); // Adjust this value for proper timing
    }
}

unsigned char check_stop_button() {
    // Check if stop button pressed
    if (!STOP_BUTTON) {
        // Abort within 10 ms
        wait_ms(10);
        return 1;
    }
    return 0;
}

void init_timer(void) {
    TMOD |= 0x01; // Timer 0 in mode 1 (16-bit timer mode)
    TH0 = 0xFF; // Initial timer value
    TL0 = 0x9C; // Timer tick interval (for 1ms tick)
    ET0 = 1; // Enable Timer 0 interrupt
    EA = 1; // Enable global interrupts
    TR0 = 1; // Start Timer 0
}

void timer_isr(void) interrupt 1 {
    TH0 = 0xFF; // Reload timer high byte
    TL0 = 0x9C; // Reload timer low byte

    if (running) {
        tick_count++;
        if (tick_count >= output_sequence[current_step].output_time) {
            generate_output(output_sequence[current_step]);
            tick_count = 0;
            current_step++;
            if (current_step >= NUM_OUTPUTS) {
                running = 0; // Stop if sequence is complete
                current_step = 0; // Reset step
            }
        }
    }
}
