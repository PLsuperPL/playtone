#define PLAYTONE_BUILD
#include "playtone.hpp"

#define PTINT32(wartosc) wartosc##L

PLAYTONEAPI uint64_t dajErr(std::string co, uint32_t val, std::string fn) //! NIE UZYWAC TAK NORMALNIE W HPP, TYLKO TU
{
    uint16_t funkcja = 0;
    uint32_t head = 0; // head(uint32)+val(uint32)=uint64
    uint16_t lCo;

    if(fn == "mono")
    {
        funkcja = 0b0000000000000000; // mono
    }
    else
    {
        funkcja = 0b1000000000000000; // stereo
    }

    if(co == "waveOutOpen")
    {
        lCo = 0b0000000000000000;
    }

    head = (static_cast<uint64_t>(funkcja) << 48) | (static_cast<uint64_t>(lCo) << 32);
    return static_cast<uint64_t>(head|val);
}

PLAYTONEAPI uint64_t zagrajMono(const buforMono& buf)
{
    //* SETUP *//
    HWAVEOUT hWaveOut;
    WAVEFORMATEX wfx;
    ZeroMemory(&wfx, sizeof(WAVEFORMATEX));

    wfx.wFormatTag = WAVE_FORMAT_PCM;
    wfx.nChannels = 1;               // mono
    wfx.nSamplesPerSec = DEFPR;
    wfx.wBitsPerSample = 16;         // 16-bit (sizeof(short)*8)
    wfx.nBlockAlign = (wfx.nChannels * wfx.wBitsPerSample) / 8;
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

    MMRESULT ret = waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    if(ret != 0)
    {
        return dajErr("waveOutOpen", static_cast<uint32_t>(ret), "mono"); // to daje kod bledu, najlepiej nie ruszac
    }

    //* INIT *//
    DWORD liczbaSampli = 0;
    for(size_t i = 0; i < buf.size(); ++i)
    {
        liczbaSampli += (DWORD)(buf[i].czas*DEFPR); // czas w sekundach * DEFPR Hz
    }
    std::vector<short> bufor(liczbaSampli);
    DWORD offset = 0;

    //* GENEROWANIE FALI *//
    for(size_t i = 0; i < buf.size(); ++i)
    {
        uint32_t sample = buf[i].czas*DEFPR;
        double amplituda = (buf[i].vol / 100.0) * (double)SHRT_MAX;

        for(DWORD j = 0; j < sample; ++j)
        {
            double t = (double)j / DEFPR;
            double val;
            double doVectora = sin(KOPIOWANIE * M_PI * buf[i].freq * t);

            if(buf[i].typ == TypFali::SIN)
            {
                val = doVectora;
            }
            else
            {
                if(doVectora >= 0.0)
                {
                    val = 1.0;
                }
                else
                {
                    val = -1.0;
                }
            }

            bufor[offset + j] = (short)(val * amplituda * pt_wzmocnienie * SHRT_MAX);
        }

        offset += sample;
    }

    //* PRZED ZAGRANIEM *//
    WAVEHDR wf;
    ZeroMemory(&wf, sizeof(WAVEHDR));
    wf.lpData = (LPSTR)&bufor[0];
    wf.dwBufferLength = (DWORD)(bufor.size() * sizeof(short));

    //* GRANIE *//
    waveOutPrepareHeader(hWaveOut, &wf, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &wf, sizeof(WAVEHDR));

    while(!(wf.dwFlags & WHDR_DONE)) // jesli nie skonczylo grac to ma grac
    {
        Sleep(5);
    }

    //* KONIEC *//
    waveOutClose(hWaveOut);
    waveOutPrepareHeader(hWaveOut, &wf, sizeof(WAVEHDR));

    return PT_POMYSLNIE;
}
