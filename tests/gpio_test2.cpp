#include <gpiod.h>
#include <iostream>
#include <unistd.h>   // sleep()

int main(){
    const char* chipname = "gpiochip0";  // Default Pi GPIO chip
    unsigned int output_pin = 17;        // BCM 17
    unsigned int input_pin  = 27;        // BCM 27

    // Open GPIO chip
    gpiod_chip* chip = gpiod_chip_open_by_name(chipname);
    if (!chip){
        std::cerr << "Failed to open GPIO chip\n";
        return 1;
    }

    // Get lines (pins)
    gpiod_line* out_line = gpiod_chip_get_line(chip, output_pin);
    gpiod_line* in_line  = gpiod_chip_get_line(chip, input_pin);

    if (!out_line || !in_line){
        std::cerr << "Failed to get GPIO lines\n";
        gpiod_chip_close(chip);
        return 1;
    }

    // Request output line
    if (gpiod_line_request_output(out_line, "gpio_example", 0) < 0){
        std::cerr << "Failed to request output line\n";
        gpiod_chip_close(chip);
        return 1;
    }

    // Request input line
    if (gpiod_line_request_input(in_line, "gpio_example") < 0){
        std::cerr << "Failed to request input line\n";
        gpiod_chip_close(chip);
        return 1;
    }

    // --- WRITE ---
    std::cout << "Setting output HIGH\n";
    gpiod_line_set_value(out_line, 1);
    sleep(1);

    std::cout << "Setting output LOW\n";
    gpiod_line_set_value(out_line, 0);

    // --- READ ---
    int value = gpiod_line_get_value(in_line);
    if (value < 0)
        std::cerr << "Failed to read input\n";
    else
        std::cout << "Input value: " << value << "\n";

    
        // Cleanup
    gpiod_line_release(out_line);
    gpiod_line_release(in_line);
    gpiod_chip_close(chip);

    return 0;
}