#include "gpiotest-matt.hpp"

int led_test() {
    const char* chip_path = "/dev/gpiochip0";
    const unsigned int offset = 27; // BCM GPIO 27 (physical pin 13)

    gpiod_chip* chip = gpiod_chip_open(chip_path);
    if (!chip) {
        std::cerr << "gpiod_chip_open failed: " << std::strerror(errno) << "\n";
        return 1;
    }

    gpiod_line_settings* settings = gpiod_line_settings_new();
    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);
    gpiod_line_settings_set_output_value(settings, GPIOD_LINE_VALUE_INACTIVE);

    gpiod_line_config* line_cfg = gpiod_line_config_new();
    if (gpiod_line_config_add_line_settings(line_cfg, &offset, 1, settings) < 0) {
        std::cerr << "add_line_settings failed: " << std::strerror(errno) << "\n";
        gpiod_line_config_free(line_cfg);
        gpiod_line_settings_free(settings);
        gpiod_chip_close(chip);
        return 1;
    }

    gpiod_request_config* req_cfg = gpiod_request_config_new();
    gpiod_request_config_set_consumer(req_cfg, "led");

    gpiod_line_request* req = gpiod_chip_request_lines(chip, req_cfg, line_cfg);

    // configs can be freed after request succeeds/fails
    gpiod_request_config_free(req_cfg);
    gpiod_line_config_free(line_cfg);
    gpiod_line_settings_free(settings);

    if (!req) {
        std::cerr << "chip_request_lines failed: " << std::strerror(errno) << "\n";
        gpiod_chip_close(chip);
        return 1;
    }

    // ON
    if (gpiod_line_request_set_value(req, offset, GPIOD_LINE_VALUE_ACTIVE) < 0) {
        std::cerr << "set_value(ON) failed: " << std::strerror(errno) << "\n";
    }
    sleep(3);

    // OFF
    if (gpiod_line_request_set_value(req, offset, GPIOD_LINE_VALUE_INACTIVE) < 0) {
        std::cerr << "set_value(OFF) failed: " << std::strerror(errno) << "\n";
    }

    gpiod_line_request_release(req);
    gpiod_chip_close(chip);
    return 0;
}