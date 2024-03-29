/*
  ==============================================================================

    Definitions.h
    Created: 10 May 2023 1:51:46pm
    Author:  Sonderbo

  ==============================================================================
*/

#pragma once

//ADSR
#define ID_String_Attack "attack"
#define ID_String_Decay "decay"
#define ID_String_Sustain "sustain"
#define ID_String_Release "release"
#define ID_String_ADSR_Toggle "adsr_toggle"

#define NAME_String_Attack "Attack"
#define NAME_String_Decay "Decay"
#define NAME_String_Sustain "Sustain"
#define NAME_String_Release "Release"
#define NAME_String_ADSR_Toggle "Adsr toggle"

//String parameter definitions
#define ID_String_T "string_T"
#define ID_String_Rho "string_rho"
#define ID_String_Sigma_0 "string_sigma0"
#define ID_String_Sigma_1 "string_sigma1"

#define NAME_String_T "Tension"
#define NAME_String_Rho "Material density"
#define NAME_String_Sigma_0 "Freq independent damping"
#define NAME_String_Sigma_1 "Freq dependent damping"

//Plate parameter definitions
#define ID_Plate_H "plate_H"
#define ID_Plate_Rho "plate_rho"
#define ID_Plate_Sigma_0 "plate_sigma0"
#define ID_Plate_Sigma_1 "plate_sigma1"

#define NAME_Plate_H "Thickness"
#define NAME_Plate_Rho "Material density"
#define NAME_Plate_Sigma_0 "Freq independent damping"
#define NAME_Plate_Sigma_1 "Freq dependent damping"

//Connection parameter definitions
#define ID_Connection_K1 "k1"
#define ID_Connection_K3 "k3"
#define ID_Connection_R "r"

#define NAME_Connection_K1 "Linear spring coefficient"
#define NAME_Connection_K3 "Nonlinear spring coefficient"
#define NAME_Connection_R "Damping coefficient"

//Input Amplitude
#define ID_String_Input_Amp "excitation_amp"
#define ID_String_Input_Width "excitation_width"
#define ID_String_Input_Loc "excitation_loc"

#define NAME_String_Input_Amp "Amplitude"
#define NAME_String_Input_Width "Width"
#define NAME_String_Input_Loc "Loc"