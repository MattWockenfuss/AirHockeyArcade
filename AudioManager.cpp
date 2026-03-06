#include "AudioManager.hpp"
#include "AssetManager.hpp"
#include "Context.hpp"

//Helper function for ALSA
int AudioManager::initPCM(const char *deviceName, short int speaker) {
    snd_pcm_t *handler;

    short int con;
    //Opens pcm to device name
    if ((con = snd_pcm_open(&handler, deviceName, SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK)) < 0) {
        std::cerr << "Failed to initialize " << deviceName << std::endl;
        return -1;
    }

    if (speaker < 0 || speaker > 1) {
        std::cerr << "Invalid option chosen for speaker value (must be 0 or 1)" << std::endl;
        return -1;
    }

    switch (speaker) {
        case 0:
        handler_1 = handler;
        h1 = true;
        break;
        case 1:
        handler_2 = handler;
        h2 = true;
        break;
    }
    
    std::cout << "Speaker PCM device" << deviceName << " success" << std::endl;

    return 0; //Success
}

AudioManager::AudioManager() {}

//Plays sound using default device
void AudioManager::playSound(const sf::SoundBuffer &sound) {
    sf::Sound snd = sf::Sound(sound);
    snd.play();
    if (snd.getStatus() == sf::Sound::Status::Playing) {
        std::cout << "Sound is playing!" << std::endl;
    }
    char p;
    std::cin>>p;
}

//Stops sound using default device
void AudioManager::stopSound(const sf::SoundBuffer &sound) {
    if (p_sounds.empty()) {
        //If sound vector is empty...then do nothing
        std::cerr << "No sound is playing!" << std::endl;
        return;
    }

    unsigned int index = 0;

    for (sf::Sound snd : p_sounds) {
        std::cout << p_sounds.size() << std::endl;
        std::cout << "Index: " << index << std::endl;
        std::cout << &snd << std::endl;
        //Clear p_sounds
        if (snd.getStatus() == sf::Sound::Status::Stopped) {
            std::cout << "Sound popped" << std::endl;
            std::swap(p_sounds[index], p_sounds.back());
            p_sounds.pop_back();
            index = 0;
            continue;
        }
        index++;
    }

    std::cout << p_sounds.empty() << std::endl;

    index = 0;
    
    for (sf::Sound snd : p_sounds) {
        std::cout << "Index: " << index << std::endl;
        sf::SoundBuffer buff = snd.getBuffer();
        std::cout << "Buffer: " << &buff << std::endl;
        std::cout << "Sound: " << &sound << std::endl;
        if (&buff == &sound) {
            std::cout << "Buffer and sound are equal!" << std::endl;
            //Same buffer, stop sound
            if (snd.getStatus() == sf::Sound::Status::Playing) {
                std::cout << "Sound is playing!" << std::endl;
                //Stop playing music, perform efficient swap to pop sound object, then free memory
                snd.stop();
                std::swap(p_sounds[index], p_sounds.back());
                p_sounds.pop_back();
                snd.~Sound();
                return;
            }
        }
        index++;
    }

    std::cerr << "Failed to stop playing sound!" << std::endl;
}

void AudioManager::stopAllSound() {

    if (p_sounds.empty()) {
        //If sound vector is empty...then do nothing
        std::cerr << "No sound is playing! Checking speakers..." << std::endl;
        if (h1 == false && h2 == false) {
            std::cerr << "PCM devices are not initialized..." << std::endl;
            return;
        }
        if (h1) {
            snd_pcm_drop(handler_1);
            snd_pcm_prepare(handler_1);
            return;
        }
        if (h2) {
            snd_pcm_drop(handler_2);
            snd_pcm_prepare(handler_2);
            return;
        }
    }

    unsigned int index = 0;

    for (sf::Sound snd : p_sounds) {
        //Same buffer, stop sound
        if (snd.getStatus() == sf::Sound::Status::Playing) {
            //Stop playing music, perform efficient swap to pop sound object, then free memory
            snd.pause();
            std::swap(p_sounds[index], p_sounds.back());
            p_sounds.pop_back();
            snd.~Sound();
            return;
        }
        index++;
    }

    std::cerr << "Failed to stop playing sound!" << std::endl;
}

void AudioManager::playSound(sf::SoundBuffer &sound, short int speaker) {

    if (h1 == false && h2 == false) {
        std::cerr << "No audio device has been initalized." << std::endl;
        playSound(sound);
        return;
    }
    
    if (speaker == 0 && h1 == true) {
        //Checks to see if PCM device is already playing something, we MUST make it stop.
        if (snd_pcm_state(handler_1) == SND_PCM_STATE_RUNNING) {
            stopSound(handler_1);
        }

        //Set hardware specs for the audio we're about to play
        snd_pcm_hw_params_t *hwt;

        //Free any previous config and allocate new memory to PCM hardware
        snd_pcm_hw_params_free(hwt);
        snd_pcm_hw_params_alloca(&hwt);

        //Start with loading all configs, and then narrowing down based on our sound
        if (
        (snd_pcm_hw_params_any(handler_1, hwt) < 0) ||
        (snd_pcm_hw_params_set_access(handler_1, hwt, SND_PCM_ACCESS_RW_INTERLEAVED) < 0) ||
        (snd_pcm_hw_params_set_format(handler_1, hwt, SND_PCM_FORMAT_S16_LE) < 0) ||
        (snd_pcm_hw_params_set_channels(handler_1, hwt, sound.getChannelCount()) < 0)
        ) {
            std::cerr << "Warning! Error encountered setting hardware parameters for handler_1" << std::endl;
        }
        

        //Prepare pcm device for writing audio to it
        if (snd_pcm_state(handler_1) != SND_PCM_STATE_PREPARED) {
            if (snd_pcm_prepare(handler_1) < 0) {
                std::cerr << "Failed to prepare handler_1 for playback!" << std::endl;
            }
        }
        
        //Write audio to PCM device
        if (snd_pcm_writei(handler_1, sound.getSamples(), sound.getSampleCount()) == -EPIPE) {
            //Buffer Underrun or worse
            short int err = snd_pcm_recover(handler_1, -EPIPE, 0);
            if (err < 0) {
                //Can't recover
                std::cerr << "Error playing sound from handler_1!" << std::endl;
                snd_pcm_hw_params_free(hwt); 
            }
        }
    }

    if (speaker == 1 && h2 == true) {
       //Checks to see if PCM device is already playing something, we MUST make it stop.
        if (snd_pcm_state(handler_2) == SND_PCM_STATE_RUNNING) {
            stopSound(handler_2);
        }

        //Set hardware specs for the audio we're about to play
        snd_pcm_hw_params_t *hwt;

        //Free any previous config and allocate new memory to PCM hardware
        snd_pcm_hw_params_free(hwt);
        snd_pcm_hw_params_alloca(&hwt);

        //Start with loading all configs, and then narrowing down based on our sound
        if (
        (snd_pcm_hw_params_any(handler_2, hwt) < 0) ||
        (snd_pcm_hw_params_set_access(handler_2, hwt, SND_PCM_ACCESS_RW_INTERLEAVED) < 0) ||
        (snd_pcm_hw_params_set_format(handler_2, hwt, SND_PCM_FORMAT_S16_LE) < 0) ||
        (snd_pcm_hw_params_set_channels(handler_2, hwt, sound.getChannelCount()) < 0)
        ) {
            std::cerr << "Warning! Error encountered setting hardware parameters for handler_2" << std::endl;
        }
        

        //Prepare pcm device for writing audio to it
        if (snd_pcm_state(handler_2) != SND_PCM_STATE_PREPARED) {
            if (snd_pcm_prepare(handler_2) < 0) {
                std::cerr << "Failed to prepare handler_2 for playback!" << std::endl;
            }
        }
        
        //Write audio to PCM device
        if (snd_pcm_writei(handler_2, sound.getSamples(), sound.getSampleCount()) == -EPIPE) {
            //Buffer Underrun or worse
            short int err = snd_pcm_recover(handler_2, -EPIPE, 0);
            if (err < 0) {
                //Can't recover
                std::cerr << "Error playing sound from handler_2!" << std::endl;
                snd_pcm_hw_params_free(hwt); 
            }
        }
    }
}

void AudioManager::playSyncSound(sf::SoundBuffer &sound) { 
    //Check to see if both PCM devices have been initialized...
    if (h1 == false || h2 == false) {
        std::cerr << "One or more PCM devices has not been initialized! Playing on default speakers." << std::endl;
        playSound(sound);
        return;
    }
    
    //Checks to see if PCM device is already playing something, we MUST make it stop.
    if (snd_pcm_state(handler_1) == SND_PCM_STATE_RUNNING) {
        stopSound(handler_1);
    }

    //Set hardware specs for the audio we're about to play
    snd_pcm_hw_params_t *hwt_1;

    //Free any previous config and allocate new memory to PCM hardware
    snd_pcm_hw_params_free(hwt_1);
    snd_pcm_hw_params_alloca(&hwt_1);

    //Start with loading all configs, and then narrowing down based on our sound
    if (
    (snd_pcm_hw_params_any(handler_1, hwt_1) < 0) ||
    (snd_pcm_hw_params_set_access(handler_1, hwt_1, SND_PCM_ACCESS_RW_INTERLEAVED) < 0) ||
    (snd_pcm_hw_params_set_format(handler_1, hwt_1, SND_PCM_FORMAT_S16_LE) < 0) ||
    (snd_pcm_hw_params_set_channels(handler_1, hwt_1, sound.getChannelCount()) < 0)
    ) {
        std::cerr << "Warning! Error encountered setting hardware parameters for handler_1" << std::endl;
    }
    

    //Prepare pcm device for writing audio to it
    if (snd_pcm_state(handler_1) != SND_PCM_STATE_PREPARED) {
        if (snd_pcm_prepare(handler_1) < 0) {
            std::cerr << "Failed to prepare handler_1 for playback!" << std::endl;
        }
    }
    
    //Write audio to PCM device
    if (snd_pcm_writei(handler_1, sound.getSamples(), sound.getSampleCount()) == -EPIPE) {
        //Buffer Underrun or worse
        short int err = snd_pcm_recover(handler_1, -EPIPE, 0);
        if (err < 0) {
            //Can't recover
            std::cerr << "Error playing sync sound from handler_1! Aborting..." << std::endl;
            snd_pcm_hw_params_free(hwt_1);
            return; 
        }
    }

    //Checks to see if PCM device is already playing something, we MUST make it stop.
    if (snd_pcm_state(handler_2) == SND_PCM_STATE_RUNNING) {
        stopSound(handler_2);
    }

    //Set hardware specs for the audio we're about to play
    snd_pcm_hw_params_t *hwt_2;

    //Free any previous config and allocate new memory to PCM hardware
    snd_pcm_hw_params_free(hwt_2);
    snd_pcm_hw_params_alloca(&hwt_2);

    //Start with loading all configs, and then narrowing down based on our sound
    if (
    (snd_pcm_hw_params_any(handler_2, hwt_2) < 0) ||
    (snd_pcm_hw_params_set_access(handler_2, hwt_2, SND_PCM_ACCESS_RW_INTERLEAVED) < 0) ||
    (snd_pcm_hw_params_set_format(handler_2, hwt_2, SND_PCM_FORMAT_S16_LE) < 0) ||
    (snd_pcm_hw_params_set_channels(handler_2, hwt_2, sound.getChannelCount()) < 0)
    ) {
        std::cerr << "Warning! Error encountered setting hardware parameters for handler_2" << std::endl;
    }
    

    //Prepare pcm device for writing audio to it
    if (snd_pcm_state(handler_2) != SND_PCM_STATE_PREPARED) {
        if (snd_pcm_prepare(handler_2) < 0) {
            std::cerr << "Failed to prepare handler_2 for playback!" << std::endl;
        }
    }
    
    //Write audio to PCM device
    if (snd_pcm_writei(handler_2, sound.getSamples(), sound.getSampleCount()) == -EPIPE) {
        //Buffer Underrun or worse
        short int err = snd_pcm_recover(handler_2, -EPIPE, 0);
        if (err < 0) {
            //Can't recover
            std::cerr << "Error playing sound from handler_2!" << std::endl;
            snd_pcm_hw_params_free(hwt_2); 
        }
    }
}

void AudioManager::stopSound(short int speaker) {
    if (speaker == 0 && h1 == true) { //Handler 1
        snd_pcm_drop(handler_1);
        snd_pcm_prepare(handler_1);
        return;
    }

    if (speaker == 1 && h2 == true) { //Handler 2
        snd_pcm_drop(handler_2);
        snd_pcm_prepare(handler_2);
    }

    std::cerr << "Passed argument " << speaker << " is undefined or has not yet been initialized" << std::endl;
}

void AudioManager::stopSyncSound() {
    if (h1 == true && h2 == true) {
        //Stop playing any current sound
        snd_pcm_drop(handler_1);
        snd_pcm_drop(handler_2);

        //Prepare hand1 and hand2 for next write
        snd_pcm_prepare(handler_1);
        snd_pcm_prepare(handler_2);
    }
    else {
        std::cerr << "One or more PCM devices has not been initialized. Stopping sound using default speaker(s)" << std::endl;
        stopAllSound();
        return;
    }
}
        
void AudioManager::stopSound(snd_pcm_t *handler) {
    snd_pcm_drop(handler);
    snd_pcm_prepare(handler);
}