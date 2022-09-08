/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

enum Slope{
    Slope_12,
    Slope_24,
    Slope_36,
    Slope_48
};

struct ChainSettings{
    float peakFreq {0}, peakGainInDecibels{0}, peakQuality{1.f};
    float lowCutFreq{0}, highCutFreq{0};
    Slope lowCutSlope{Slope::Slope_12}, highCutSlope{Slope::Slope_12};
};

ChainSettings getChainSettings(juce::AudioProcessorValueTreeState& apvts);
//==============================================================================
/**
*/
class SimpleEqAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    SimpleEqAudioProcessor();
    ~SimpleEqAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    juce::AudioProcessorValueTreeState apvts {*this,nullptr,"Parameters", createParameterLayout() };

private:
    
    using Filter = juce::dsp::IIR::Filter<float>;
    
    using CutFilter = juce::dsp::ProcessorChain<Filter,Filter,Filter,Filter>;
    
    using MonoChain = juce::dsp::ProcessorChain<CutFilter, Filter, CutFilter>;
    
    MonoChain leftChain, rightChain;
    
    enum ChainPositions{
        LowCut,
        Peak,
        HighCut
    };
    
    void updatePeakFilter(const ChainSettings& chainSettings);
    
    using Coefficients = Filter::CoefficientsPtr;
    static void updateCoefficients(Coefficients& old, const Coefficients& replacements);
    
    template<typename ChainType, typename CoefficientType>
    void updateCutFilter(ChainType& leftLowCut, const CoefficientType& cutCoefficients, const Slope& lowCutSlope){
            leftLowCut.template setBypassed<0>(true);
            leftLowCut.template setBypassed<1>(true);
            leftLowCut.template setBypassed<2>(true);
            leftLowCut.template setBypassed<3>(true);
        
            switch(lowCutSlope){
                case Slope_12:
                    *leftLowCut.template get<0>().coefficients = *cutCoefficients[0];
                    leftLowCut.template setBypassed<0>(false);
                    break;
                case Slope_24:
                    *leftLowCut.template get<0>().coefficients = *cutCoefficients[0];
                    leftLowCut.template setBypassed<0>(false);
                    *leftLowCut.template get<1>().coefficients = *cutCoefficients[1];
                    leftLowCut.template setBypassed<1>(false);
                    break;
                case Slope_36:
                    *leftLowCut.template get<0>().coefficients = *cutCoefficients[0];
                    leftLowCut.template setBypassed<0>(false);
                    *leftLowCut.template get<1>().coefficients = *cutCoefficients[1];
                    leftLowCut.template setBypassed<1>(false);
                    *leftLowCut.template get<2>().coefficients = *cutCoefficients[2];
                    leftLowCut.template setBypassed<2>(false);
                    break;
                case Slope_48:
                    *leftLowCut.template get<0>().coefficients = *cutCoefficients[0];
                    leftLowCut.template setBypassed<0>(false);
                    *leftLowCut.template get<1>().coefficients = *cutCoefficients[1];
                    leftLowCut.template setBypassed<1>(false);
                    *leftLowCut.template get<2>().coefficients = *cutCoefficients[2];
                    leftLowCut.template setBypassed<2>(false);
                    *leftLowCut.template get<3>().coefficients = *cutCoefficients[3];
                    leftLowCut.template setBypassed<3>(false);
                    break;
            }
//            auto& rightLowCut = leftChain.get<ChainPositions::LowCut>();
//
//            rightLowCut.template setBypassed<0>(true);
//            rightLowCut.template setBypassed<1>(true);
//            rightLowCut.template setBypassed<2>(true);
//            rightLowCut.template setBypassed<3>(true);
//
//            switch(lowCutSlope){
//                case Slope_12:
//                    *rightLowCut.get<0>().coefficients = *cutCoefficients[0];
//                    rightLowCut.setBypassed<0>(false);
//                    break;
//                case Slope_24:
//                    *rightLowCut.get<0>().coefficients = *cutCoefficients[0];
//                    rightLowCut.setBypassed<0>(false);
//                    *rightLowCut.get<1>().coefficients = *cutCoefficients[1];
//                    rightLowCut.setBypassed<1>(false);
//                    break;
//                case Slope_36:
//                    *rightLowCut.get<0>().coefficients = *cutCoefficients[0];
//                    rightLowCut.setBypassed<0>(false);
//                    *rightLowCut.get<1>().coefficients = *cutCoefficients[1];
//                    rightLowCut.setBypassed<1>(false);
//                    *rightLowCut.get<2>().coefficients = *cutCoefficients[2];
//                    rightLowCut.setBypassed<2>(false);
//                    break;
//                case Slope_48:
//                    *rightLowCut.get<0>().coefficients = *cutCoefficients[0];
//                    rightLowCut.setBypassed<0>(false);
//                    *rightLowCut.get<1>().coefficients = *cutCoefficients[1];
//                    rightLowCut.setBypassed<1>(false);
//                    *rightLowCut.get<2>().coefficients = *cutCoefficients[2];
//                    rightLowCut.setBypassed<2>(false);
//                    *rightLowCut.get<3>().coefficients = *cutCoefficients[3];
//                    rightLowCut.setBypassed<3>(false);
//                    break;
//            }
    }
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleEqAudioProcessor)
};
