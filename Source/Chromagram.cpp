//=======================================================================
/** @file Chromagram.cpp
 *  @brief Chromagram - a class for calculating the chromagram in real-time
 *  @author Adam Stark
 *  @copyright Copyright (C) 2008-2014  Queen Mary University of London
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
//=======================================================================

#include "Chromagram.h"

//==================================================================================
Chromagram::Chromagram (int frameSize, int fs)
 :  referenceFrequency (130.81278265),
    bufferSize (8192),
    numHarmonics (2),
    numOctaves (2),
    numBinsToSearch (2)
{
    // calculate note frequencies
    for (int i = 0; i < 12; i++)
    {
        noteFrequencies[i] = referenceFrequency * pow (2,(((float) i) / 12));
    }
    
    // set up FFT
    setupFFT();
    
    // set buffer size
    buffer.resize (bufferSize);
    
    // setup chromagram vector
    chromagram.resize (12);
    
    // initialise chromagram
    for (int i = 0; i < 12; i++)
    {
        chromagram[i] = 0.0;
    }
    
    // setup magnitude spectrum vector
    magnitudeSpectrum.resize ((bufferSize/2)+1);
    
    // make window function
    makeHammingWindow();
    
    // set sampling frequency
    setSamplingFrequency (fs);
    
    // set input audio frame size
    setInputAudioFrameSize (frameSize);
    
    // initialise num samples counter
    numSamplesSinceLastCalculation = 0;
    
    // set chroma calculation interval (in samples at the input audio sampling frequency)
    chromaCalculationInterval = 4096;
    
    // initialise chroma ready variable
    chromaReady = false;
}

//==================================================================================
Chromagram::~Chromagram()
{
    // free the Kiss FFT configuration
    free (cfg);
    delete [] fftIn;
    delete [] fftOut;
}

//==================================================================================
void Chromagram::processAudioFrame (double* inputAudioFrame)
{
    // create a vector
    std::vector<double> v;
    
    // use the array to initialise it
    v.assign (inputAudioFrame, inputAudioFrame + inputAudioFrameSize);
    
    // process the vector
    processAudioFrame (v);
}

//==================================================================================
void Chromagram::processAudioFrame (std::vector<double> inputAudioFrame)
{
    // our default state is that the chroma is not ready
    chromaReady = false;
    
    // downsample the input audio frame by 4
    downSampleFrame(inputAudioFrame);
    
    // move samples back
    for (int i = 0; i < bufferSize - downSampledAudioFrameSize; i++)
    {
        buffer[i] = buffer[i + downSampledAudioFrameSize];
    }
    
    int n = 0;
    
    // add new samples to buffer
    for (int i = (bufferSize - downSampledAudioFrameSize); i < bufferSize; i++)
    {
        buffer[i] = downsampledInputAudioFrame[n];
        n++;
    }
    
    // add number of samples from calculation
    numSamplesSinceLastCalculation += inputAudioFrameSize;
        
    // if we have had enough samples
    if (numSamplesSinceLastCalculation >= chromaCalculationInterval)
    {
        // calculate the chromagram
        calculateChromagram();
        
        // reset num samples counter
        numSamplesSinceLastCalculation = 0;
    }
    
}

//==================================================================================
void Chromagram::setInputAudioFrameSize (int frameSize)
{
    inputAudioFrameSize = frameSize;
    
    downsampledInputAudioFrame.resize (inputAudioFrameSize / 4);
    
    downSampledAudioFrameSize = (int) downsampledInputAudioFrame.size();
}

//==================================================================================
void Chromagram::setSamplingFrequency (int fs)
{
    samplingFrequency = fs;
}

//==================================================================================
void Chromagram::setChromaCalculationInterval (int numSamples)
{
    chromaCalculationInterval = numSamples;
}

//==================================================================================
std::vector<double> Chromagram::getChromagram()
{
    return chromagram;
}

//==================================================================================
bool Chromagram::isReady()
{
    return chromaReady;
}

//==================================================================================
void Chromagram::setupFFT()
{
    // ------------------------------------------------------
    // initialise the fft time and frequency domain audio frame arrays
    fftIn = new kiss_fft_cpx[bufferSize];
    fftOut = new kiss_fft_cpx[bufferSize];
    cfg = kiss_fft_alloc (bufferSize,0,0,0);
}


//==================================================================================
void Chromagram::calculateChromagram()
{
    calculateMagnitudeSpectrum();
    
    double divisorRatio = (((double) samplingFrequency) / 4.0) / ((double)bufferSize);
    
    for (int n = 0; n < 12; n++)
    {
        double chromaSum = 0.0;
        
        for (int octave = 1; octave <= numOctaves; octave++)
        {
            double noteSum = 0.0;
            
            for (int harmonic = 1; harmonic <= numHarmonics; harmonic++)
            {
                int centerBin = round ((noteFrequencies[n] * octave * harmonic) / divisorRatio);
                int minBin = centerBin - (numBinsToSearch * harmonic);
                int maxBin = centerBin + (numBinsToSearch * harmonic);
                
                double maxVal = 0.0;
                
                for (int k = minBin; k < maxBin; k++)
                {
                    if (magnitudeSpectrum[k] > maxVal)
                    {
                        maxVal = magnitudeSpectrum[k];
                    }
                }
            
                noteSum += (maxVal / (double) harmonic);
            }
            
            chromaSum += noteSum;
        }
        
        chromagram[n] = chromaSum;
    }
    
    chromaReady = true;
}

//==================================================================================
void Chromagram::calculateMagnitudeSpectrum()
{   
    // -----------------------------------------------
    // KISS FFT VERSION
    // -----------------------------------------------
    int i = 0;
    
    for (int i = 0;i < bufferSize; i++)
    {
        fftIn[i].r = buffer[i] * window[i];
        fftIn[i].i = 0.0;
    }
    
    // execute kiss fft
    kiss_fft (cfg, fftIn, fftOut);
    
    // compute first (N/2)+1 mag values
    for (i = 0; i < (bufferSize / 2) + 1; i++)
    {
        magnitudeSpectrum[i] = sqrt (pow (fftOut[i].r, 2) + pow (fftOut[i].i, 2));
        magnitudeSpectrum[i] = sqrt (magnitudeSpectrum[i]);
    }
}

//==================================================================================
void Chromagram::downSampleFrame (std::vector<double> inputAudioFrame)
{
    std::vector<double> filteredFrame (inputAudioFrameSize);
    
    float b0,b1,b2,a1,a2;
    float x_1,x_2,y_1,y_2;
    
    b0 = 0.2929;
    b1 = 0.5858;
    b2 = 0.2929;
    a1 = -0.0000;
    a2 = 0.1716;
    
    x_1 = 0;
    x_2 = 0;
    y_1 = 0;
    y_2 = 0;
    
    for (int i = 0; i < inputAudioFrameSize; i++)
    {
        filteredFrame[i] = inputAudioFrame[i] * b0 + x_1 * b1 + x_2 * b2 - y_1 * a1 - y_2 * a2;
        
        x_2 = x_1;
        x_1 = inputAudioFrame[i];
        y_2 = y_1;
        y_1 = filteredFrame[i];
    }
    
    for (int i = 0; i < inputAudioFrameSize / 4; i++)
    {
        downsampledInputAudioFrame[i] = filteredFrame[i * 4];
    }
}

//==================================================================================
void Chromagram::makeHammingWindow()
{
    // set the window to the correct size
    window.resize (bufferSize);
    
    // apply hanning window to buffer
    for (int n = 0; n < bufferSize;n++)
    {
        window[n] = 0.54 - 0.46 * cos (2 * M_PI * (((double) n) / ((double) bufferSize)));
    }
}

//==================================================================================
double Chromagram::round (double val)
{
    return floor (val + 0.5);
}