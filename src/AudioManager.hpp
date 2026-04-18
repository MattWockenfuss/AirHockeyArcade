#pragma once

#include <SFML/Audio.hpp>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <list>
//#include <alsa/asoundlib.h>

class AudioManager {
    public:
        //Default constructor
        AudioManager();

        //Audio play
        //void playSound(sf::Sound &sound);
        void playSound(sf::Music &sound);

        //Specify USB device to send audio to, uses ALSA lib
        //void playSound(sf::SoundBuffer &sound, short int speaker);
        //void playSyncSound(sf::SoundBuffer &sound);
        void stopSound();
        void stopSound(short int speaker);
        void stopSyncSound();
        void stopAllSound();
    
        sf::Sound *snd;
    private:
        //Definitions for both handlers for both speaker devices.
        //snd_pcm_t *handler_1, *handler_2;
        bool h1 = false, h2 = false;
        int initPCM(const char *deviceName, short int speaker);
        //void stopSound(snd_pcm_t *handler);
        std::list<sf::Music*> muslist = {};
        void checkSound();
};