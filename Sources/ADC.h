/*
 * ADC.h
 *
 *  Created on: May 23, 2025
 *      Author: ZCai2
 */

#ifndef SOURCES_ADC_H_
#define SOURCES_ADC_H_

#include "fsl_device_registers.h"

//double C_freq[7] = {32.70, 65.41, 130.81, 261.63, 523.25, 1046.50, 2093.00};
//double D_freq[7] = {36.71, 73.42, 146.83, 293.66, 587.33, 1174.66, 2349.32};
//double E_freq[7] = {41.20, 82.41, 164.81, 329.63, 659.25, 1318.51, 2637.02};
//double F_freq[7] = {43.65, 87.31, 174.61, 349.23, 698.46, 1396.91, 2793.83};
//double G_freq[7] = {49.00, 98.00, 196.00, 392.00, 783.99, 1567.98, 3135.96};
//double A_freq[7] = {55.00, 110.00, 220.00, 440.00, 880.00, 1760.00, 3520.00};
//double B_freq[7] = {61.74, 123.47, 246.94, 493.88, 987.77, 1975.53, 3951.07};
//double Csharp_Dflat_freq[7] = {34.65, 69.30, 138.59, 277.18, 554.37, 1108.73, 2217.46};
//double Dsharp_Eflat_freq[7] = {38.89, 77.78, 155.56, 311.13, 622.25, 1244.51, 2489.02};
//double Fsharp_Gflat_freq[7] = {46.25, 92.50, 185.00, 369.99, 739.99, 1479.98, 2959.96};
//double Gsharp_Aflat_freq[7] = {51.91, 103.83, 207.65, 415.30, 830.61, 1661.22, 3322.44};
//double Asharp_Bflat_freq[7] = {58.27, 116.54, 233.08, 466.16, 932.33, 1864.66, 3729.31};

// frequencies in order by octave 1-7 (0-6 for indices)
int data1 = 0;
int data2 = 0;

// function for reading data using ADC
unsigned short ADC_read16b(void)
{
	ADC0_SC1A = 0x00; // Write to SC1A to start conversion from ADC0
	while(ADC0_SC2 & ADC_SC2_ADACT_MASK); // Conversion in progress
	while(!(ADC0_SC1A & ADC_SC1_COCO_MASK)); // Until Conversion complete
	return ADC0_RA;
}

// changes volume of passive buzzer through potentiometer
double Volume_Change() {
	// duty cycle set to 0% or 100% results in no sound
	double volume = 0.0;
	data1 = ADC_read16b(); // period is FTM3_MOD
	if (data1 > 0 && data1 < 1000) {
		volume = 0.0; // duty cycle is 0%
	}
	else if (data1 > 1000 && data1 < 64535) {
		volume = 0.5; // duty cycle is 50%
	}
	else if (data1 > 64535 && data1 < 65535) {
		volume = 1.0; // duty cycle is 100%
	}
	return volume;
}

// changes the octave of note being played through potentiometer (7 octaves chosen)
double Octave_Change() {
	double octave = 0;
	data2 = ADC_read16b();
	if (data2 > 0 && data2 < 9362) {
		octave = 0.0;
	}
	else if (data2 > 9362 && data2 < 18724) {
		octave = 1.0;
	}
	else if (data2 > 18724 && data2 < 28086) {
		octave = 2.0;
	}
	else if (data2 > 28086 && data2 < 37449) {
		octave = 3.0;
	}
	else if (data2 > 37449 && data2 < 46811) {
		octave = 4.0;
	}
	else if (data2 > 46811 && data2 < 56173) {
		octave = 5.0;
	}
	else if (data2 > 56173 && data2 < 65535) {
		octave = 6.0;
	}
	return octave;
}

#endif /* SOURCES_ADC_H_ */

