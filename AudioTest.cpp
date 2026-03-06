#include "AudioManager.hpp"

//Test file for Audio Manager

int main() {
    sf::SoundBuffer horn1, horn2, intermission;
    
    //Load each sound for testing.
    if (!horn1.loadFromFile("assets/sounds/goal_horn.wav")) {
        std::cerr << "Error loading sound file goal_horn.wav" << std::endl;
    }

    if (!horn2.loadFromFile("assets/sounds/goal_horn_2.wav")) {
        std::cerr << "Error loading sound file goal_horn_2.wav" << std::endl;
    }

    if (!intermission.loadFromFile("assets/sounds/intermission.wav")) {
        std::cerr << "Error loading sound file intermission.wav" << std::endl;
    }

    std::cout << "Printing addresses..." << std::endl;
    std::cout << "Horn1: " << &horn1 << std::endl;
    std::cout << "Horn2: " << &horn2 << std::endl;
    std::cout << "Intermission: " << &intermission << std::endl;

    //Individual sound testing.
    char p;
    
    AudioManager adm = AudioManager();

    adm.playSound(horn1);

    std::cout << "Press any character to continue...";
    std::cin >> p;
    

    adm.playSound(horn2);

    std::cout << "Press any character to continue...";
    std::cin >> p;

    adm.playSound(intermission);

    std::cout << "Press any character to continue...";
    std::cin >> p;

    //Test synchronous sound
    adm.playSound(horn1);
    adm.playSound(horn2);
    adm.playSound(intermission);

    std::cout << "Press any character to continue...";
    std::cin >> p;

    //Test start/stop sound
    adm.playSound(horn1);
    adm.stopSound(horn1);
    adm.playSound(horn2);
    adm.playSound(intermission);
    adm.stopSound(horn2);
    adm.stopSound(intermission);
    adm.playSound(horn1);
    adm.playSound(horn2);
    adm.playSound(intermission);
    adm.stopSound(horn2);
    adm.stopSound(horn1);
    adm.stopSound(intermission);

    std::cout << "Press any character to continue...";
    std::cin >> p;

    //End default audio playback test...
    std::cout << "Default audio playback...success" << std::endl;

    //Begin PCM Device test
    
    return 0;
}