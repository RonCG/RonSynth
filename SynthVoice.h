/*
  ==============================================================================

    SynthVoice.h
    Created: 12 Feb 2019 11:32:03am
    Author:  RonnyCG

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthSound.h"
#include "maximilian.h"

class SynthVoice : public SynthesiserVoice
{
public:
	bool canPlaySound(SynthesiserSound* sound)
	{
		return dynamic_cast<SynthSound*>(sound) != nullptr;
	}

	/*=================== Oscillators =================*/

	void getOsc1(float* osc1Selection, float* osc1Semi) //Done
	{
		osc1Wave = *osc1Selection;
		osc1Semitone = *osc1Semi;
	}

	void getOsc2(float* osc2Selection, float* osc2Semi, float* osc2FineTuning, float* osc2FMIntensity) //Done
	{
		osc2Wave = *osc2Selection;	
		osc2Semitone = *osc2Semi;
		osc2Fine = *osc2FineTuning;
		osc2Fm = *osc2FMIntensity;
	}

	double setOsc1()//Done
	{
		double oscWaveReturn, finalFrequency, fmValue;
		finalFrequency = MidiMessage::getMidiNoteInHertz(midiNoteNumberST + osc1Semitone); //Frequency using GUI semitone

		/*If LFO1 is applied to Osc1 Frequency*/
		if (lfo1Dest == 2 && lfo1Intensity > 0 && osc1Wave == 0)//se puede controlar aqui q solo se aplique el lfo a la freq cuando el osc sea sine
			finalFrequency = finalFrequency * (setLFO1()* lfo1Intensity);

		/*If LFO2 is applied to Osc1 Frequency*/
		if (lfo2Dest == 2 && lfo2Intensity > 0 && osc1Wave == 0)
			finalFrequency = finalFrequency * (setLFO2()* lfo2Intensity);

		/*If both LFOs are applied to Osc1  Frequency*/
		if (lfo1Dest == 2 && lfo1Intensity > 0 && lfo2Dest == 2 && lfo2Intensity > 0 && osc1Wave == 0)
			finalFrequency = finalFrequency * ((setLFO1()* lfo1Intensity/10) + (setLFO2()* lfo2Intensity/10));

	
		/*FM parameter from Oscillator 2*/
		if (osc2Fm > 0)
			fmValue = (setFMOscillator()*(osc2Fm));
		else
			fmValue = 1;

		switch (osc1Wave)
		{
		case 0:
			oscWaveReturn = osc1.sinewave(finalFrequency*fmValue);
			break;
		case 1:
			oscWaveReturn = osc1.saw(finalFrequency*fmValue);
			break;
		case 2:
			oscWaveReturn = osc1.triangle(finalFrequency*fmValue);
			break;
		case 3:
			oscWaveReturn = osc1.square(finalFrequency*fmValue);
			break;
		case 4:
			oscWaveReturn = osc1.phasor(finalFrequency*fmValue);
			break;
		default:
			oscWaveReturn = osc1.sinewave(finalFrequency*fmValue);
		}

		/*If LFO1 is applied to Osc1 Amp*/
		if (lfo1Dest == 0 && lfo1Intensity > 0)
			oscWaveReturn = oscWaveReturn * (setLFO1()* lfo1Intensity);

		/*If LFO2 is applied to Osc1 Amp*/
		if (lfo2Dest == 0 && lfo2Intensity > 0)
			oscWaveReturn = oscWaveReturn * (setLFO2()* lfo2Intensity);

		/*If both LFOs are applied to Osc1 Amp*/
		if (lfo1Dest == 0 && lfo1Intensity > 0 && lfo2Dest == 0 && lfo2Intensity > 0)
			oscWaveReturn = oscWaveReturn * ((setLFO1()* lfo1Intensity/10) + (setLFO2()* lfo2Intensity/10));

		return oscWaveReturn;
	
	}

	double setOsc2()
	{
		double oscWaveReturn, finalFrequency , nextFrequency, maxFineTuningVariation, fineTuningVariation;
		finalFrequency = MidiMessage::getMidiNoteInHertz(midiNoteNumberST + osc2Semitone); //Frequency using GUI semitone
		nextFrequency = MidiMessage::getMidiNoteInHertz(midiNoteNumberST + osc2Semitone + 1);
		maxFineTuningVariation = (nextFrequency - finalFrequency)/2; //Fine tuning at max = half way to the next frequency
		fineTuningVariation = maxFineTuningVariation * (osc2Fine/10);

		/*If LFO1 is applied to OsC2 Frequency*/
		if (lfo1Dest == 3 && lfo1Intensity > 0 && osc2Wave == 0)
			finalFrequency = finalFrequency * (setLFO1()* lfo1Intensity);

		/*If LFO2 is applied to Osc2 Frequency*/
		if (lfo2Dest == 3 && lfo2Intensity > 0 && osc2Wave == 0)
			finalFrequency = finalFrequency * (setLFO2()* lfo2Intensity);

		/*If both LFOs are applied to Osc2 Frequency*/
		if (lfo1Dest == 3 && lfo1Intensity > 0 && lfo2Dest == 3 && lfo2Intensity > 0 && osc2Wave == 0)
			finalFrequency = finalFrequency * ((setLFO1()* lfo1Intensity/10) + (setLFO2()* lfo2Intensity/10));

		switch (osc2Wave)
		{
		case 0:
			oscWaveReturn = osc2.sinewave(finalFrequency + fineTuningVariation);
			break;
		case 1:
			oscWaveReturn = osc2.saw(finalFrequency + fineTuningVariation);
			break;
		case 2:
			oscWaveReturn = osc2.triangle(finalFrequency + fineTuningVariation);
			break;
		case 3:
			oscWaveReturn = osc2.square(finalFrequency + fineTuningVariation);
			break;
		case 4:
			oscWaveReturn = osc2.phasor(finalFrequency + fineTuningVariation);
			break;
		default:
			oscWaveReturn = osc2.sinewave(finalFrequency + fineTuningVariation);
		}

		/*If LFO1 is applied to Osc2 Amp*/
		if (lfo1Dest == 1 && lfo1Intensity > 0)
			oscWaveReturn = oscWaveReturn * (setLFO1()* lfo1Intensity);

		/*If LFO2 is applied to Osc2 Amp*/
		if (lfo2Dest == 1 && lfo2Intensity > 0)
			oscWaveReturn = oscWaveReturn * (setLFO2()* lfo2Intensity);

		/*If both LFOs are applied to Osc2 Amp*/
		if (lfo1Dest == 1 && lfo1Intensity > 0 && lfo2Dest == 1 && lfo2Intensity > 0)
			oscWaveReturn = oscWaveReturn * ((setLFO1()* lfo1Intensity/10) + (setLFO2()* lfo2Intensity/10));

		return oscWaveReturn;
	}

	double setSubOsc()
	{
		double subOscReturn, finalFrequency;
		finalFrequency = MidiMessage::getMidiNoteInHertz(midiNoteNumberST-12);
		subOscReturn = subOsc.square(finalFrequency);

		/*If LFO1 is applied to SubOsc Amp*/
		if (lfo1Dest == 4 && lfo1Intensity > 0)
			subOscReturn = subOscReturn * (setLFO1()* lfo1Intensity);

		/*If LFO2 is applied to SubOsc Amp*/
		if (lfo2Dest == 4 && lfo2Intensity > 0)
			subOscReturn = subOscReturn * (setLFO2()* lfo2Intensity);

		/*If both LFOs are applied to SubOsc Amp*/
		if (lfo1Dest == 4 && lfo1Intensity > 0 && lfo2Dest == 4 && lfo2Intensity > 0)
			subOscReturn = subOscReturn * ((setLFO1()* lfo1Intensity/10) + (setLFO2()* lfo2Intensity/10));

		return subOscReturn; //oscilador una octava abajo de la frecuencia original
	}

	double setNoise()
	{
		return noise.noise();
	}
	
	double setFMOscillator()
	{
		double oscWaveReturn, finalFrequency;
		finalFrequency = MidiMessage::getMidiNoteInHertz(midiNoteNumberST + osc2Semitone); //Frequency using GUI semitone

		switch (osc2Wave)
		{
		case 0:
			oscWaveReturn = oscFM.sinewave(finalFrequency);
			break;
		case 1:
			oscWaveReturn = oscFM.saw(finalFrequency);
			break;
		case 2:
			oscWaveReturn = oscFM.triangle(finalFrequency);
			break;
		case 3:
			oscWaveReturn = oscFM.square(finalFrequency);
			break;
		case 4:
			oscWaveReturn = oscFM.phasor(finalFrequency);
			break;
		default:
			oscWaveReturn = oscFM.sinewave(finalFrequency);
		}

		return oscWaveReturn;

	}

	double setOscillatorsMix()
	{
		double oscillatorsMixReturn;
		oscillatorsMixReturn = ((setOsc1()*(mixOsc1Lvl / 10)) + (setOsc2()*(mixOsc2Lvl / 10)) + (setSubOsc()*(mixSubOscLvl / 10)) + (setNoise()*(mixNoiseLvl / 10))) * mixMasterLvl / 10; //revisar por el tema logaritmico de volumen
		
	    /*If LFO1 is applied to Volume*/
		if (lfo1Dest == 6 && lfo1Intensity > 0)
			oscillatorsMixReturn = oscillatorsMixReturn * (setLFO1()* lfo1Intensity);

		/*If LFO2 is applied to Volume*/
		if (lfo2Dest == 6 && lfo2Intensity > 0)
			oscillatorsMixReturn = oscillatorsMixReturn * (setLFO2()* lfo2Intensity);

		/*If both LFOs are applied to Volume*/
		if (lfo1Dest == 6 && lfo1Intensity > 0 && lfo2Dest == 6 && lfo2Intensity > 0)
			oscillatorsMixReturn = oscillatorsMixReturn * ((setLFO1()* lfo1Intensity/10) + (setLFO2()* lfo2Intensity/10));
		
		return oscillatorsMixReturn;
	}

	/*-------------------------------------------------*/



	/*==================== Filter =====================*/

	void getFilter1(float* filterType, float* filterCut, float* filterRes)
	{
		filter1Choice = *filterType;
		filter1CutOff = *filterCut;
		filter1Resonance = *filterRes/1.5;
	}

	double setFilter1()
	{
		double filterReturn;

		/*double wave1 = setOsc1();
		double wave2 = setOsc2();

		double filtered1 = filter1.lores(wave1, env2Cut.adsr(filter1CutOff, env2Cut.trigger), filter1Resonance);
		double filtered2 = filter1.lores(wave2, env2Cut.adsr(filter1CutOff, env2Cut.trigger), filter1Resonance);

		return (env1Amp.adsr(filtered1, env1Amp.trigger)+ env1Amp.adsr(filtered2, env1Amp.trigger));*/

		switch (filter1Choice)
		{
		case 0:
			filterReturn = filter1.lores(setOscillatorsMix(), setEnvelope2(), filter1Resonance);
			break;
		case 1:
			filterReturn = filter1.hires(setOscillatorsMix(), setEnvelope2(), filter1Resonance);
			break;
		case 2:
			filterReturn = filter1.bandpass(setOscillatorsMix(), setEnvelope2(), filter1Resonance);
			break;
		default:
			filterReturn = filter1.lores(setOscillatorsMix(), setEnvelope2(), filter1Resonance);
		}

		
		return filterReturn;
	}

	/*-------------------------------------------------*/



	/*==================== Envelope ====================*/

	void getEnvelope1(float* attack, float* decay, float* sustain, float* release)
	{
		env1Amp.setAttack(double(*attack));
		env1Amp.setDecay(double(*decay));
		env1Amp.setSustain(double(*sustain));
		env1Amp.setRelease(double(*release));
	}

	void getEnvelope2(float* attack, float* decay, float* sustain, float* release)
	{
		env2Cut.setAttack(double(*attack));
		env2Cut.setDecay(double(*decay));
		env2Cut.setSustain(double(*sustain));
		env2Cut.setRelease(double(*release));
	}


	double setEnvelope1()
	{
		return env1Amp.adsr(setFilter1(), env1Amp.trigger);
	}

	double setEnvelope2()
	{
		double filter1CutOffReturn;

		filter1CutOffReturn = filter1CutOff;

		/*If LFO1 is applied to Cutoff*/
		if (lfo1Dest == 5 && lfo1Intensity > 0)
			filter1CutOffReturn = filter1CutOffReturn + (setLFO1()  * lfo1Intensity * lfo1Intensity * lfo1Intensity);

		/*If LFO2 is applied to Cutoff*/
		if (lfo2Dest == 5 && lfo2Intensity > 0)
			filter1CutOffReturn = filter1CutOffReturn + (setLFO2() * lfo2Intensity * lfo2Intensity * lfo2Intensity);

		/*If both LFOs are applied to Cutoff*/
		if (lfo1Dest == 5 && lfo1Intensity > 0 && lfo2Dest == 5 && lfo2Intensity > 0)
			filter1CutOffReturn = filter1CutOffReturn + ((setLFO1()  * lfo1Intensity/10 ) + (setLFO2() * lfo2Intensity/10 ));

		return env2Cut.adsr(filter1CutOffReturn, env2Cut.trigger);
	}
	
	/*-------------------------------------------------*/

	

	/*===================== Mixer ======================*/

	void getMixer(float *osc1Lvl, float *osc2Lvl, float *subOscLvl, float *noiseLvl, float *masterLvl) 
	{
		mixOsc1Lvl = *osc1Lvl;
		mixOsc2Lvl = *osc2Lvl;
		mixSubOscLvl = *subOscLvl;
		mixNoiseLvl = *noiseLvl;
		mixMasterLvl = *masterLvl;
	}

	/*-------------------------------------------------*/



	/*===================== LFOs =======================*/

	void getLFO1(float* lfo1Selection, float* lfo1Destination, float* lfo1RateValue, float* lfo1IntesityValue)
	{
		lfo1Wave = *lfo1Selection;
		lfo1Dest = *lfo1Destination;
		lfo1Rate = *lfo1RateValue;
		lfo1Intensity = *lfo1IntesityValue;
	}

	void getLFO2(float* lfo2Selection, float* lfo2Destination, float* lfo2RateValue, float* lfo2IntesityValue)
	{
		lfo2Wave = *lfo2Selection;
		lfo2Dest = *lfo2Destination;
		lfo2Rate = *lfo2RateValue;
		lfo2Intensity = *lfo2IntesityValue;
	}

	double setLFO1()
	{
		double lfoWaveReturn;

		switch (lfo1Wave)
		{
		case 0:
			lfoWaveReturn = lfo1.sinewave(lfo1Rate);
			break;
		case 1:
			lfoWaveReturn = lfo1.saw(lfo1Rate);
			break;
		case 2:
			lfoWaveReturn = lfo1.square(lfo1Rate);
			break;
		case 3:
			lfoWaveReturn = lfo1.triangle(lfo1Rate);
			break;

		default:
			lfoWaveReturn = lfo1.sinewave(lfo1Rate);
		}

		return lfoWaveReturn;
	}

	double setLFO2()
	{
		double lfoWaveReturn;

		switch (lfo2Wave)
		{
		case 0:
			lfoWaveReturn = lfo2.sinewave(lfo2Rate);
			break;
		case 1:
			lfoWaveReturn = lfo2.saw(lfo2Rate);
			break;
		case 2:
			lfoWaveReturn = lfo2.square(lfo2Rate);
			break;
		case 3:
			lfoWaveReturn = lfo2.triangle(lfo2Rate);
			break;

		default:
			lfoWaveReturn = lfo2.sinewave(lfo2Rate);
		}

		return lfoWaveReturn;
	}
	/*-------------------------------------------------*/

	void startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition)
	{
		env1Amp.trigger = 1;
		env2Cut.trigger = 1;
		level = velocity;
		midiNoteNumberST = midiNoteNumber;
		frequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);

		test = midiNoteNumber;
	}
	

	void stopNote(float velocity, bool allowTailOff)
	{
		env1Amp.trigger = 0;
		env2Cut.trigger = 0;
		allowTailOff = true;

		if (velocity == 0)
			clearCurrentNote();
	}


	void pitchWheelMoved(int newPitchWheelValue)
	{

	}


	void controllerMoved(int controllerNumber, int newControllerValue)
	{

	}


	void renderNextBlock(AudioBuffer<float> &outputBuffer, int startSample, int numSamples)
	{

		for (int sample = 0; sample < numSamples; sample++)
		{
			for (int channel = 0; channel < outputBuffer.getNumChannels(); channel++)
			{
				outputBuffer.addSample(channel, startSample, setEnvelope1()/**level*/);
			}
			startSample++;
		}
	}


private:
	/*General*/
	double level;
	double frequency;
	int midiNoteNumberST;

	/*Osciladores*/
	int osc1Wave, osc2Wave, osc1Semitone, osc2Semitone;
	float osc2Fine, osc2Fm;

	/*Filtro*/
	int filter1Choice;
	float filter1CutOff;
	float filter1Resonance;

	/*Mixer*/
	float  mixOsc1Lvl, mixOsc2Lvl, mixSubOscLvl, mixNoiseLvl, mixMasterLvl;

	/*LFOs*/
	int lfo1Wave, lfo2Wave, lfo1Dest, lfo2Dest;
	float lfo1Rate, lfo2Rate, lfo1Intensity, lfo2Intensity;

	/*Maximillian Variables*/
	maxiOsc osc1, osc2, lfo1, lfo2, subOsc, noise, oscFM;
	maxiEnv env1Amp, env2Cut;
	maxiFilter filter1;

	int test;
};