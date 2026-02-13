#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <gpiod.hpp>

int main() 
{
    //Path for Pi5 to gpiochip device. Should be the same for the others is running PiOS
    const std::filesystem::path path = "/dev/gpiochip0";
    
    gpiod::chip chip = gpiod::chip(path);
    if (!chip) //Check to see if valid object, error otherwise
    {
        std::perror("Chip is undefined! Does not exist.");
        return -1;
    }

    //Gets the gpiochip information, prints the name, its label, and the number of pins (lines)
    gpiod::chip_info info = chip.get_info();

    std::cout << info.name() << std::endl;
    std::cout << info.label() << std::endl;
    std::cout << info.num_lines() << std::endl;

    //Gets the line info for pin #0 (offset), prints object returned
    gpiod::line_info linfo = chip.get_line_info(0);
    std::cout << linfo << std::endl;

    //Closes chip and frees mem
    chip.close();

    return 1;
}