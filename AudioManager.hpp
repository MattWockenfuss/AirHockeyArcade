#pragma once

#include <SFML/Audio.hpp>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <alsa/asoundlib.h>

class Context;

class AudioManager {
    public:
        //Default constructor/destructor
        AudioManager();

        //Audio play
        void playSound(const sf::SoundBuffer &sound);

        //Specify USB device to send audio to, uses ALSA lib
        void playSound(sf::SoundBuffer &sound, short int speaker);
        void playSyncSound(sf::SoundBuffer &sound);
        void stopSound(short int speaker);
        void stopSyncSound();
        void stopAllSound();
    
    private:
        //Definitions for both handlers for both speaker devices.
        snd_pcm_t *handler_1, *handler_2;
        bool h1 = false, h2 = false;
        int initPCM(const char *deviceName, short int speaker);
        void stopSound(snd_pcm_t *handler);
        sf::Sound *snd;
};