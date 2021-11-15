#pragma once
#include <cstdint>
#ifdef NR2003_EXPORTS
#define NR2003_API __declspec(dllexport)
#else
#define NR2003_API __declspec(dllimport)
#endif

// setup, returnst rue if setup was a success
extern "C" NR2003_API bool Setup();

// close the app
extern "C" NR2003_API bool EndTelemetry();

// Busy-Waits for NR2003 to run
extern "C" NR2003_API bool WaitForSimToRun();

// get new data
extern "C" NR2003_API void RequestData();

extern "C" NR2003_API uintptr_t GetGaugeData();
extern "C" NR2003_API uintptr_t GetCurrentWeekend();
extern "C" NR2003_API uintptr_t GetLapCrossing();
extern "C" NR2003_API uintptr_t GetDriverEntry();
extern "C" NR2003_API uintptr_t GetStandings();
extern "C" NR2003_API uintptr_t GetSessionInfo();

extern "C" NR2003_API void DataGatherBackGround();