//! SZANOWNY UZYTKOWIKU BIBLIOTEKI PLAYTONE
//! Najlepiej w ogole nie ruszac plikow z folderu z githuba czy czego tam bo sie moze zepsuc
//! i trzeba bedzie poprawiac #include w playtone.cpp, chyba ze kompilowane to jest ok, ale
//! jak bedziesz kompilowal to zostaw

#pragma once

#ifndef __cplusplus //! NIE RUSZAC
    #error Ta biblioteka wymaga C++
#endif

#ifdef PLAYTONE_BUILD
    #define PLAYTONEAPI __declspec(dllexport)
#else
    #define PLAYTONEAPI __declspec(dllimport)
#endif

#define _USE_MATH_DEFINES // <cmath>

#include <vector>     // buforMono
#include <cmath>      // sin() M_PI
#include <cstdint>    // uintXX_t (32, 8)
#include <cstddef>    // size_t (implementacja)
#include <windows.h>  // do <mmsystem.h>
#include <mmsystem.h> // dzwiek
#include <string>     // std::string
#include <climits>    // X_MAX albo X_MIN

#pragma comment(lib, "winmm.lib") // przy kompilacji jeszcze -lwinmm

#define DEFPR 44100 // 44,1kHz
#define KOPIOWANIE 2.0 //! NAJLEPIEJ UZYWAC TYLKO W IMPLEMENTACJI

constexpr uint64_t PT_POMYSLNIE = 0;

uint8_t pt_wzmocnienie = 1;

enum TypFali {SIN, SQ}; // kolejno: sinusoida, kwadrat

typedef uint64_t blad; // kod bledu ma 64 bity

typedef struct {
    uint32_t freq; // Czestotliwosc dzwieku; Hz
    uint8_t vol;  // Glosnosc; %
    uint32_t czas; // Czas trwania; s
    TypFali typ;   // SIN/SQ
} KonlumnaMono;

typedef std::vector<KonlumnaMono> buforMono; // buforX y = {{z}, {z}, ...}

extern "C" // dostepne z innych jezykow
{
    PLAYTONEAPI blad zagrajMono(const buforMono&);
    //PLAYTONEAPI blad zagrajStereo(const buforStereo&);
    //PLAYTONEAPI blad nuta(char dzwiek, uint8_t oktawa);
    //PLAYTONEAPI std::string errTxt(blad err);
    //moze kiedys zrobie te trzy funkcje jak mi sie zachce
}
