﻿// PlayerGUI.cpp
#include "PlayerGUI.h"

PlayerGUI::PlayerGUI()
{
    for (auto* btn : { &loadButton, &restartButton, &pauseButton, &playButton,
                      &goToStartButton, &endButton, &loopButton, &muteButton })
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
    }

    loopButton.setButtonText("Loop OFF");
    muteButton.setButtonText("Mute OFF");

    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);
}

PlayerGUI::~PlayerGUI() {}

void PlayerGUI::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    playerAudio.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerGUI::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    playerAudio.getNextAudioBlock(bufferToFill);
}

void PlayerGUI::releaseResources()
{
    playerAudio.releaseResources();
}

void PlayerGUI::resized()
{
    int y = 20;
    loadButton.setBounds(20, y, 100, 40);
    restartButton.setBounds(140, y, 80, 40);
    pauseButton.setBounds(240, y, 80, 40);
    playButton.setBounds(340, y, 80, 40);
    goToStartButton.setBounds(440, y, 80, 40);
    endButton.setBounds(540, y, 80, 40);
    muteButton.setBounds(640, 20, 80, 40);
    loopButton.setBounds(740, y, 80, 40);
    volumeSlider.setBounds(20, 100, getWidth() - 40, 30);
}

void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        fileChooser = std::make_unique<juce::FileChooser>(
            "Select an audio file...", juce::File{}, "*.wav;*.mp3");

        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file.existsAsFile())
                    playerAudio.loadFile(file);
            });
    }
    else if (button == &restartButton)
    {
        playerAudio.restart();
    }
    else if (button == &pauseButton)
    {
        playerAudio.pause();
    }
    else if (button == &playButton)
    {
        playerAudio.play();
    }
    else if (button == &goToStartButton)
    {
        playerAudio.goToStart();
    }
    else if (button == &endButton)
    {
        playerAudio.end();
    }
    else if (button == &loopButton)
    {
        bool newLoopState = !playerAudio.isLoopEnabled();
        playerAudio.setLoopEnabled(newLoopState);
        loopButton.setButtonText(newLoopState ? "Loop ON" : "Loop OFF");
        loopButton.setColour(juce::TextButton::buttonColourId,
            newLoopState ? juce::Colours::green : juce::Colours::lightgrey);
    }
    else if (button == &muteButton)
    {
        static float previousVolume = volumeSlider.getValue();

        if (muteButton.getButtonText() == "Mute OFF")
        {
            previousVolume = volumeSlider.getValue();
            volumeSlider.setValue(0.0);
            muteButton.setButtonText("Mute ON");
            muteButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
        }
        else
        {
            volumeSlider.setValue(previousVolume);
            muteButton.setButtonText("Mute OFF");
            muteButton.setColour(juce::TextButton::buttonColourId, juce::Colours::lightgrey);
        }
    }
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
        playerAudio.setGain((float)slider->getValue());
}