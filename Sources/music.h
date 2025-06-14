/*
 * music.h
 *
 *  Created on: May 23, 2025
 *      Author: ZCai2
 */

#ifndef SOURCES_MUSIC_H_
#define SOURCES_MUSIC_H_

#include "fsl_device_registers.h"
#include "ADC.h"
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MaxNotes 100

typedef struct {
	double rnote[MaxNotes]; // holds the frequency of note being played
	char segment[MaxNotes]; // holds the key on 7 segment display of the note
} Record;

// default at Octave 1
double C = 32.70;
double D = 36.71;
double E = 41.20;
double F = 43.65;
double G = 49.00;
double A = 55.00;
double B = 61.74;
double Csharp_Dflat = 34.65;
double Dsharp_Eflat = 38.89;
double Fsharp_Gflat = 46.25;
double Gsharp_Aflat = 51.91;
double Asharp_Bflat = 58.27;

// white keys on piano
unsigned char notes[7] = {0x8E,0x3D,0x8F,0x87,0x9F,0xB7,0x1F}; // C, D, E, F, G, A, B

// black keys on piano (sharps and flats)
unsigned char sharps[5] = {0x8E,0x3D,0x87,0x9F,0xB7}; // C#, D#, F#, G#, A# (raised by half step on piano)
unsigned char flats[5] = {0x3D,0x8F,0x9F,0xB7,0x1F}; // Db, Eb, Gb, Ab, Bb (lowered by half step on piano)

// utilizing the tick formula to calculate the PWM needed for each note
double ticks = 21000000.0/8.0;
double freq = 0.0;
double octave = 0.0;
double volume = 0.5;

bool stopPlay = false; // boolean to stop keys from playing during replay phase
bool noteOn = false; // boolean flag to check when button is pressed to play the note
Record play; // struct variable "play" to use for the notes and segment
char store = 0; // store the note for recording

// increment cnt for filling in arrays for recording as well as increment size for the size of the array
int size = 0;
int cnt = 0;

void software_delay(unsigned long delay)
{
	while (delay > 0) delay--;
}

// output the sharp or flat on the 7 segment display based on dip switch
void Sharp_or_Flat(int dipSwitch, int pos) {
	int input = dipSwitch & 0x04;
	if (input == 0) {
		GPIOC_PSOR = (unsigned int) sharps[pos];
		store = sharps[pos];
	}
	else {
		GPIOC_PSOR = (unsigned int) flats[pos];
		store = flats[pos];
	}
}

// black keys and white keys plus octave and volume changes
void Play_Display_Note(int whiteKeys, int blackKeys, int blackKey, int dipSwitch) {
	if ((dipSwitch & 0x400) == 0) { // checks if dip switch for playback is cleared or not to allow for notes to play
		stopPlay = false;
		// each correspond to each note: C, D, E, F, G, A, B, plus the sharps/flats
		if (whiteKeys & 0x01) {
			if (!noteOn) {
				octave = Octave_Change(); // octave change from ADC
				volume = Volume_Change(); // volume change from ADC
				freq = C * pow(2.0,octave); // calculates the frequency for each note in each octave
				FTM3_MOD = (int) (ticks/freq)-1.0; // PWM period times the ticks to obtain period, subtract 1 since period = FTM3_MOD + 1
				FTM3_C6SC = 0x28; // PWM active high
				FTM3_C6V = (int) (ticks/freq)*volume; // duty cycle = 50%, multiply by ticks to get pulse width
				FTM3_SC = 0x0B; // system clock prescaler set to 8
				GPIOC_PCOR = 0xBF;
				GPIOC_PSOR = (unsigned int) notes[0]; // turns on the
				store = notes[0]; // variable for storing the note when recording
				noteOn = true; // allows key to be played while held down until released
			}
		}
		else if (whiteKeys & 0x02) {
			if (!noteOn) {
				octave = Octave_Change();
				volume = Volume_Change();
				freq = D * pow(2.0,octave);
				FTM3_MOD = (int) (ticks/freq)-1.0;
				FTM3_C6SC = 0x28;
				FTM3_C6V = (int) (ticks/freq)*volume;
				FTM3_SC = 0x0B;
				GPIOC_PCOR = 0xBF;
				GPIOC_PSOR = (unsigned int) notes[1];
				store = notes[1];
				noteOn = true;
			}
		}
		else if (whiteKeys & 0x04) {
			if (!noteOn) {
				octave = Octave_Change();
				volume = Volume_Change();
				freq = E * pow(2.0,octave);
				FTM3_MOD = (int) (ticks/freq)-1.0;
				FTM3_C6SC = 0x28;
				FTM3_C6V = (int) (ticks/freq)*volume;
				FTM3_SC = 0x0B;
				GPIOC_PCOR = 0xBF;
				GPIOC_PSOR = (unsigned int) notes[2];
				store = notes[2];
				noteOn = true;
			}
		}
		else if (whiteKeys & 0x08) {
			if (!noteOn) {
				octave = Octave_Change();
				volume = Volume_Change();
				freq = F * pow(2.0,octave);
				FTM3_MOD = (int) (ticks/freq)-1.0;
				FTM3_C6SC = 0x28;
				FTM3_C6V = (int) (ticks/freq)*volume;
				FTM3_SC = 0x0B;
				GPIOC_PCOR = 0xBF;
				GPIOC_PSOR = (unsigned int) notes[3];
				store = notes[3];
				noteOn = true;
			}
		}
		else if (whiteKeys & 0x10) {
			if (!noteOn) {
				octave = Octave_Change();
				volume = Volume_Change();
				freq = G * pow(2.0,octave);
				FTM3_MOD = (int) (ticks/freq)-1.0;
				FTM3_C6SC = 0x28;
				FTM3_C6V = (int) (ticks/freq)*volume;
				FTM3_SC = 0x0B;
				GPIOC_PCOR = 0xBF;
				GPIOC_PSOR = (unsigned int) notes[4];
				store = notes[4];
				noteOn = true;
			}
		}
		else if (whiteKeys & 0x20) {
			if (!noteOn) {
				octave = Octave_Change();
				volume = Volume_Change();
				freq = A * pow(2.0,octave);
				FTM3_MOD = (int) (ticks/freq)-1.0;
				FTM3_C6SC = 0x28;
				FTM3_C6V = (int) (ticks/freq)*volume;
				FTM3_SC = 0x0B;
				GPIOC_PCOR = 0xBF;
				GPIOC_PSOR = (unsigned int) notes[5];
				store = notes[5];
				noteOn = true;
			}
		}
		else if (whiteKeys & 0x40) {
			if (!noteOn) {
				octave = Octave_Change();
				volume = Volume_Change();
				freq = B * pow(2.0,octave);
				FTM3_MOD = (int) (ticks/freq)-1.0;
				FTM3_C6SC = 0x28;
				FTM3_C6V = (int) (ticks/freq)*volume;
				FTM3_SC = 0x0B;
				GPIOC_PCOR = 0xBF;
				GPIOC_PSOR = (unsigned int) notes[6];
				store = notes[6];
				noteOn = true;
			}
		}
		else if (blackKeys & 0x100) {
			if (!noteOn) {
				octave = Octave_Change();
				volume = Volume_Change();
				freq = Csharp_Dflat * pow(2.0,octave);
				FTM3_MOD = (int) (ticks/freq)-1.0;
				FTM3_C6SC = 0x28;
				FTM3_C6V = (int) (ticks/freq)*volume;
				FTM3_SC = 0x0B;
				Sharp_or_Flat(dipSwitch,0);
				noteOn = true;
			}
		}
		else if (blackKeys & 0x200) {
			if (!noteOn) {
				octave = Octave_Change();
				volume = Volume_Change();
				freq = Dsharp_Eflat * pow(2.0,octave);
				FTM3_MOD = (int) (ticks/freq)-1.0;
				FTM3_C6SC = 0x28;
				FTM3_C6V = (int) (ticks/freq)*volume;
				FTM3_SC = 0x0B;
				Sharp_or_Flat(dipSwitch,1);
				noteOn = true;
			}
		}
		else if (blackKeys & 0x800) {
			if (!noteOn) {
				octave = Octave_Change();
				volume = Volume_Change();
				freq = Fsharp_Gflat * pow(2.0,octave);
				FTM3_MOD = (int) (ticks/freq)-1.0;
				FTM3_C6SC = 0x28;
				FTM3_C6V = (int) (ticks/freq)*volume;
				FTM3_SC = 0x0B;
				Sharp_or_Flat(dipSwitch,2);
				noteOn = true;
			}
		}
		else if (blackKeys & 0x10000) {
			if (!noteOn) {
				octave = Octave_Change();
				volume = Volume_Change();
				freq = Gsharp_Aflat * pow(2.0,octave);
				FTM3_MOD = (int) (ticks/freq)-1.0;
				FTM3_C6SC = 0x28;
				FTM3_C6V = (int) (ticks/freq)*volume;
				FTM3_SC = 0x0B;
				Sharp_or_Flat(dipSwitch,3);
				noteOn = true;
			}
		}
		else if (blackKey & 0x80) {
			if (!noteOn) {
				octave = Octave_Change();
				volume = Volume_Change();
				freq = Asharp_Bflat * pow(2.0,octave);
				FTM3_MOD = (int) (ticks/freq)-1.0;
				FTM3_C6SC = 0x28;
				FTM3_C6V = (int) (ticks/freq)*volume;
				FTM3_SC = 0x0B;
				Sharp_or_Flat(dipSwitch,4);
				noteOn = true;
			}
		}
		else if (noteOn) { // on release, turn off note and segment display; check dip switch for recording
			// clear the note and segment display
			FTM3_C6SC = 0x00;
			FTM3_C6V = 0x00;
			FTM3_SC = 0x00;
			GPIOC_PCOR = 0xBF;
			// check dip switch for recording; if yes, record current frequency and segment display in struct arrays
			if (dipSwitch & 0x08) {
				play.rnote[cnt] = freq;
				play.segment[cnt] = store;
				cnt++;
				size++;
			}
			noteOn = false; // flag set back to false for note playing detection
		}
	}
	else if ((dipSwitch & 0x400)) { // play back the recording when dip switch is 1
		if (stopPlay == false) {
			for (int i = 0; i < size; i++) {
				if ((dipSwitch & 0x400)) {
					freq = play.rnote[i];
					FTM3_MOD = (int) (ticks/freq) - 1.0;
					FTM3_C6SC = 0x28;
					FTM3_C6V = (int) (ticks/freq) * volume;
					FTM3_SC = 0x0B;

					GPIOC_PCOR = 0xBF;
					GPIOC_PSOR = (unsigned int) play.segment[i];

					software_delay(1000000); // software delay added for length of the note played

					FTM3_C6SC = 0x00;
					FTM3_C6V = 0x00;
					FTM3_SC = 0x00;
					GPIOC_PCOR = 0xBF;

					software_delay(200000); // software delay added for duration between each note played
				}
			}
			stopPlay = true; // sets stopPlay to transition to clearing the notes
		}
		else if (stopPlay == true) { // if true, turn off final note and clear the struct arrays as well as counters
			FTM3_C6SC = 0x00;
			FTM3_C6V = 0x00;
			FTM3_SC = 0x00;
			for (int i = 0; i < size; i++) {
				play.rnote[i] = 0.0; // clearing the doubles inside rnote[]
				play.segment[i] = '\0'; // clearing the chars inside segment[]
			}
			size = 0;
			cnt = 0;
		}
	}
}

#endif /* SOURCES_MUSIC_H_ */
