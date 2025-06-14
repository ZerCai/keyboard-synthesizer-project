/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "fsl_device_registers.h"
#include "math.h"
#include "music.h"
#include "ADC.h"
#include <stdio.h>

int main(void) {
	// Port D initializations
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;
	PORTD_GPCLR = 0x00FF0100;
	GPIOD_PDDR = 0x00000000;

	// Port B initializations
    SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
    PORTB_GPCLR = 0x040C0100;
    GPIOB_PDDR = 0x00000000;

	// Port C initializations
    SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
    PORTC_GPCLR = 0x0BBF0100;
    PORTC_GPCHR = 0x00010100;
    GPIOC_PDDR = 0x000000BF;

    // ADC Initializations
    SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK; // 0x8000000u - Enable ADC0 Clock
    ADC0_CFG1 = 0x0C; // 16 bits ADC; Bus Clock
    ADC0_SC1A = 0x1F; // Disable the module during init, ADCH = 11111

    // PWM Clock Initializations
    SIM_SCGC3 |= SIM_SCGC3_FTM3_MASK;
    PORTC_PCR10 = 0x300;
    FTM3_MODE = 0x5; // Enable FTM3

    int whiteKeys = 0; // port D pin 7 is for A#/Bb black key (last one on the right)
    int blackKeys = 0;
    int blackKey = 0;
    int dipSwitch = 0;

    while (1) {
    	whiteKeys = GPIOD_PDIR & 0x7F; // checks the white keys
    	blackKeys = GPIOC_PDIR & 0x10B00; // checks the black keys
    	blackKey = GPIOD_PDIR & 0x80; // checks last black key on PORTD

    	dipSwitch = GPIOB_PDIR & 0x40C; // 0x800 for pin 10, 0x08 for pin 3, 0x04 for pin 2

    	Play_Display_Note(whiteKeys,blackKeys,blackKey,dipSwitch);
    }
}
