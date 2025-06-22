/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

auto getPhaserRateName() { return juce::String("Phaser RateHz"); }
auto getPhaserCenterFreqName() { return juce::String("Phaser Center FreqHz"); }
auto getPhaserDepthName() { return juce::String("Phaser Depth %"); }
auto getPhaserFeedbackName() { return juce::String("Phaser Feedback %"); }
auto getPhaserMixName() { return juce::String("Phaser Mix %"); }

auto getChorusRateName() { return juce::String("Chorus RateHz"); }
auto getChorusDepthName() { return juce::String("Chorus Depth %"); }
auto getChorusCenterDelayName() { return juce::String("Chorus Center Delay Ms"); }
auto getChorusFeedbackName() { return juce::String("Chorus Feedback %"); }
auto getChorusMixName() { return juce::String("Chorus Mix %"); }

auto getOverdriveSaturationName() { return juce::String("Overdrive Saturation"); }

auto getLadderFilterModeName() { return juce::String("Ladder Filter Mode"); }
auto getLadderFilterCutoffName() { return juce::String("Ladder Filter Cutoff Hz"); }
auto getLadderFilterResonanceName() { return juce::String("Ladder Filter Resonance"); }
auto getLadderFilterDriveName() { return juce::String("Ladder Filter Drive"); }

auto getLadderFilterChoices() {
    return juce::StringArray {
        "LPF12",  // low-pass  12 dB/octave
        "HPF12",  // high-pass 12 dB/octave
        "BPF12",  // band-pass 12 dB/octave
        "LPF24",  // low-pass  24 dB/octave
        "HPF24",  // high-pass 24 dB/octave
        "BPF24"   // band-pass 24 dB/octave
    };
}

auto getGeneralFilterChoices() {
    return juce::StringArray {
        "Peak",
        "Bandpass",
        "Notch",
        "Allpass",
    };
}

auto getGeneralFilterModeName() { return juce::String("General Filter Mode"); }
auto getGeneralFilterFreqName() { return juce::String("General Filter Freq Hz"); }
auto getGeneralFilterQualityName() { return juce::String("General Filter Quality"); }
auto getGeneralFilterGainName() { return juce::String("General Filter Gain"); }

//==============================================================================
JucetutorialsAudioProcessor::JucetutorialsAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    auto floatParams = std::array {
        &phaserRateHz,
        &phaserCenterFreqHz,
        &phaserDepthPercent,
        &phaserFeedbackPercent,
        &phaserMixPercent,

        &chorusRateHz,
        &chorusDepthPercent,
        &chorusCenterDelayMs,
        &chorusFeedbackPercent,
        &chorusMixPercent,

        &overdriveSaturation,

        &ladderFilterCutoffHz,
        &ladderFilterResonance,
        &ladderFilterDrive,

        &generalFilterFreqHz,
        &generalFilterQuality,
        &generalFilterGain,
    };

    auto floatNameFuncs = std::array {
        &getPhaserRateName,
        &getPhaserCenterFreqName,
        &getPhaserDepthName,
        &getPhaserFeedbackName,
        &getPhaserMixName,

        &getChorusRateName,
        &getChorusDepthName,
        &getChorusCenterDelayName,
        &getChorusFeedbackName,
        &getChorusMixName,

        &getOverdriveSaturationName,

        &getLadderFilterCutoffName,
        &getLadderFilterResonanceName,
        &getLadderFilterDriveName,

        &getGeneralFilterFreqName,
        &getGeneralFilterQualityName,
        &getGeneralFilterGainName,
    };

    for (size_t i = 0; i < floatParams.size(); ++i) {
        auto ptrToParamPtr = floatParams[i];
        *ptrToParamPtr = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter(floatNameFuncs[i]()));
        jassert(*ptrToParamPtr != nullptr);
    }

    auto choiceParams = std::array {
        &ladderFilterMode,
        &generalFilterMode,
    };

    auto choiceNameFuncs = std::array {
        &getLadderFilterModeName,
        &getGeneralFilterModeName,
    };

    for (size_t i = 0; i < choiceParams.size(); ++i) {
        auto ptrToParamPtr = choiceParams[i];
        *ptrToParamPtr = dynamic_cast<juce::AudioParameterChoice*>(apvts.getParameter(choiceNameFuncs[i]()));
        jassert(*ptrToParamPtr != nullptr);
    }
}

JucetutorialsAudioProcessor::~JucetutorialsAudioProcessor()
{
}

//==============================================================================
const juce::String JucetutorialsAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool JucetutorialsAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool JucetutorialsAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool JucetutorialsAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double JucetutorialsAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int JucetutorialsAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int JucetutorialsAudioProcessor::getCurrentProgram()
{
    return 0;
}

void JucetutorialsAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String JucetutorialsAudioProcessor::getProgramName (int index)
{
    return {};
}

void JucetutorialsAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void JucetutorialsAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void JucetutorialsAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool JucetutorialsAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

juce::AudioProcessorValueTreeState::ParameterLayout JucetutorialsAudioProcessor::createParameterLayout() {
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // Pseudo-code pattern :
    // name = nameFunction();
    // layout.add(std::make_unique<juce::AudioParameterFloat>(
    //     juce::ParameterID{ name, versionHint },
    //     name,
    //     parameterRange,
    //     defaultValue,
    //     unitSuffix
    // ));

    const int versionHint = 1;

    // Phaser rate: 0.01 - 2.0 Hz, default 0.2 Hz
    auto name = getPhaserRateName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ name, versionHint },
                                                           name,
                                                           juce::NormalisableRange<float>(0.01f, 2.f, 0.01f, 1.f),
                                                           0.2f,
                                                           "Hz"));

    // Phaser depth: 0 - 1, default 0.05
    name = getPhaserDepthName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ name, versionHint },
                                                           name,
                                                           juce::NormalisableRange<float>(0.01f, 1.f, 0.01f, 1.f),
                                                           0.05f,
                                                           "%"));

    // Phaser center freq: 20 - 20k Hz, default 1000 Hz
    name = getPhaserCenterFreqName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ name, versionHint },
                                                           name,
                                                           juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 1.f),
                                                           1000.f,
                                                           "Hz"));

    // Phaser feedback: -1 - +1, default 0
    name = getPhaserFeedbackName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ name, versionHint },
                                                           name,
                                                           juce::NormalisableRange<float>(-1.f, 1.f, 0.01f, 1.f),
                                                           0.0f,
                                                           "%"));

    // Phaser mix: 0 - 1, default 0.05
    name = getPhaserMixName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ name, versionHint },
                                                           name,
                                                           juce::NormalisableRange<float>(0.01f, 1.f, 0.01f, 1.f),
                                                           0.05f,
                                                           "%"));

    // Chorus rate: 0.01 - 100.0 Hz, default 0.2 Hz
    name = getChorusRateName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ name, versionHint },
                                                           name,
                                                           juce::NormalisableRange<float>(0.01f, 100.f, 0.01f, 1.f),
                                                           0.2f,
                                                           "Hz"));

    // Chorus depth: 0 - 1, default 0.05
    name = getChorusDepthName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ name, versionHint },
                                                           name,
                                                           juce::NormalisableRange<float>(0.01f, 1.f, 0.01f, 1.f),
                                                           0.05f,
                                                           "%"));

    // Chorus center delay: 1 - 100 Ms, default 1000 Hz
    name = getChorusCenterDelayName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ name, versionHint },
                                                           name,
                                                           juce::NormalisableRange<float>(1.f, 100.f, 0.1f, 1.f),
                                                           7.f,
                                                           "%"));

    // Chorus feedback: -1 - +1, default 0
    name = getChorusFeedbackName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ name, versionHint },
                                                           name,
                                                           juce::NormalisableRange<float>(-1.f, 1.f, 0.01f, 1.f),
                                                           0.0f,
                                                           "%"));

    // Chorus mix: 0 - 1, default 0.05
    name = getChorusMixName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ name, versionHint },
                                                           name,
                                                           juce::NormalisableRange<float>(0.01f, 1.f, 0.01f, 1.f),
                                                           0.05f,
                                                           "%"));

    // Overdrive: 1 - 100, uses the drive portion of the ladder filter class for now
    name = getOverdriveSaturationName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ name, versionHint },
                                                           name,
                                                           juce::NormalisableRange<float>(1.f, 100.f, 0.1f, 1.f),
                                                           1.f,
                                                           ""));

    // Ladder filter mode: LadderFilterMode enum (int)
    name = getLadderFilterModeName();
    auto choices = getLadderFilterChoices();
    layout.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{ name, versionHint },
                                                            name,
                                                            choices,
                                                            0));

    // Ladder filter cutoff: 20 - 20k Hz
    name = getLadderFilterCutoffName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ name, versionHint },
                                                           name,
                                                           juce::NormalisableRange<float>(20.f, 20000.f, 0.1f, 1.f),
                                                           20000.f,
                                                           "Hz"));

    // Ladder filter resonance: 0 - 1
    name = getLadderFilterResonanceName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ name, versionHint },
                                                           name,
                                                           juce::NormalisableRange<float>(0.f, 1.f, 0.01f, 1.f),
                                                           0.f,
                                                           ""));

    // Ladder filter drive: 1 - 100
    name = getLadderFilterDriveName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ name, versionHint },
                                                           name,
                                                           juce::NormalisableRange<float>(1.f, 100.f, 0.1f, 1.f),
                                                           1.f,
                                                           ""));

    // General filter mode: peak, bandpass, notch, allpass
    name = getGeneralFilterModeName();
    choices = getGeneralFilterChoices();
    layout.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{ name, versionHint },
                                                            name,
                                                            choices,
                                                            0));

    // General filter freq: 20 - 20k Hz, 1Hz increments
    name = getGeneralFilterFreqName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ name, versionHint },
                                                           name,
                                                           juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 1.f),
                                                           750.f,
                                                           "Hz"));

    // General filter quality: 0.1 - 10, 0.05 increments
    name = getGeneralFilterQualityName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ name, versionHint },
                                                           name,
                                                           juce::NormalisableRange<float>(0.1f, 10.f, 0.05f, 1.f),
                                                           1.f,
                                                           ""));

    // General filter gain: -24 - +24 dB, 0.5dB increments
    name = getGeneralFilterGainName();
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ name, versionHint },
                                                           name,
                                                           juce::NormalisableRange<float>(-24.f, 24.f, 0.5f, 1.f),
                                                           0.f,
                                                           "dB"));

    return layout;
}

void JucetutorialsAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    //DONE: add APVTS
    //Done: create audio parameters for all dsp choices
    //TODO: update DSP here from audio parameters
    //TODO: save/load settings
    //TODO: save/load DSP order
    //TODO: Drag-To-Reorder GUI
    //TODO: GUI design for each DSP instance ?
    //TODO: metering
    //TODO: prepare all DSP
    //TODO: wet/dry knob [BONUS]
    //TODO: mono & stereo versions [mono is BONUS]
    //TODO: modulators [BONUS]
    //TODO: thread-safe filter updating [BONUS]
    //TODO: pre/post filtering [BONUS]
    //TODO: delay module [BONUS]

    auto newDSPOrder = DSP_Order();

    // Try to pull
    while (dspOrderFifo.pull(newDSPOrder)) {}

    // If you pulled, replace dspOrder
    if (newDSPOrder != DSP_Order())
        dspOrder = newDSPOrder;

    // Convert dspOrder into an array of pointers.
    DSP_Pointers dspPointers;

    for (size_t i = 0; i < dspPointers.size(); ++i) {
        switch (dspOrder[i]) {
        case DSP_Option::Phase:
            dspPointers[i] = &phaser;
            break;
        case DSP_Option::Chorus:
            dspPointers[i] = &chorus;
            break;
        case DSP_Option::Overdrive:
            dspPointers[i] = &overdrive;
            break;
        case DSP_Option::LadderFilter:
            dspPointers[i] = &ladderFilter;
            break;
        case DSP_Option::GeneralFilter:
            dspPointers[i] = &generalFilter;
            break;
        case DSP_Option::END_OF_LIST:
            jassertfalse;
            break;
        }
    }

    // Process
    auto block = juce::dsp::AudioBlock<float>(buffer);
    auto context = juce::dsp::ProcessContextReplacing<float>(block);

    for (size_t i = 0; i < dspPointers.size(); ++i) {
        if (dspPointers[i] != nullptr) {
            dspPointers[i]->process(context);
        }
    }
}

//==============================================================================
bool JucetutorialsAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* JucetutorialsAudioProcessor::createEditor()
{
    return new JucetutorialsAudioProcessorEditor (*this);
}

//==============================================================================
void JucetutorialsAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void JucetutorialsAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JucetutorialsAudioProcessor();
}
