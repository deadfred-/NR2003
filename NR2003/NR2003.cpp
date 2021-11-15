#include "pch.h"
#include "papytelemapp.h"
#include "NR2003.h"
#include <math.h>
#include <thread>

// DLL internal state variables

// If you set this to be true, then the telemetry output will be in
// english units.  Otherwise they are output in SI units.
const bool englishUnits = true;

// When false, sampling is done at the game tick rate (36Hz, currently).
// When true, sampling is done at the physics rate (288Hz, currently).
const bool sampleAtPhysicsRate = false;

// So that we don't have to qualify our use of the telem API
// components with "PapyTelem::".  
using namespace PapyTelem;

const eSimDataType desired[] =
{

			kSampleHeader,
			kChassisData,
			kWheelData,
			kTireData,
			kDrivelineData,
			kDriverInput,

			kGaugeData,
			kOpponentCarData,

			kCurrentWeekend,		// Want state information, too
			kDriverInCar,
			kGameIsPaused,
			kCarSetup,
			kPitStop,
			kDriverEntry,
			kDriverWithdrawl,
			kSessionInfo,
			kLapCrossing,
			kStandings
};


const GaugeData* gd;
const CurrentWeekend* cw;
const LapCrossing* lc;
const DriverEntry* de;
const Standings* se;
const SessionInfo* si;

// Sets up the telemtry session, allowing the program access to the telemetry memory space of NR2003.
NR2003_API bool Setup()
{
	// assume failure
	bool returnValue = false;

	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);

	if (AppBegin("NR2003"))
	{
		(void)AppRequestDataItems(sizeof(desired) / sizeof(desired[0]),	desired);
		(void)AppRequestDataAtPhysicsRate(sampleAtPhysicsRate);
		(void)AppEnableSampling(true);
	}
	// since we called it we need to end it...
	/*
	if (AppCheckIfSimActiveQ()) 
	{
		AppEnd();
	}
	*/	

	//std::thread t1(DataGatherBackGround);
	//t1.join();
	
	returnValue = true;

	return returnValue;
}

NR2003_API bool EndTelemetry() 
{	
	if (AppCheckIfSimActiveQ())
	{
		AppEnd();
	}
	return true;
}

NR2003_API bool WaitForSimToRun()
{
	// assume failure
	bool returnValue = true;


	while (!AppCheckIfSimActiveQ())
	{
		Sleep(1000);
	}


	return returnValue;
}

NR2003_API void DataGatherBackGround() {

}

NR2003_API void RequestData()
{
	//const int timeOutMs = 100;
	const int timeOutMs = 0;
	eSimDataType newStateData;
	bool newSample;
	if ((newSample = AppWaitForNewSample(&newStateData, timeOutMs)) == true ||
		newStateData != kNoStateInfo || 
		AppCheckIfSimActiveQ())
	{
		// See if any new telemetry from the sim.
		if (newSample || newStateData != kNoStateInfo)
		{
			// Yes, it has either written a piece of state data,
			// or has finished writing a new sample.  Which one?
			// Either newStateData won't equal kNoStateInfo
			// (meaning a piece of state data was read), or
			// AppWaitForNewSample will return true (indicating
			// a complete sample is now available), but never
			// both at once.
			if (newStateData != kNoStateInfo)
			{
				// It was state data.  Process it.
				//printStateData(newStateData);
				//printStatus(newStateData);				
				if ((gd = (const GaugeData*)AppGetSimData(kGaugeData)) != NULL)
				{
					// success!
				}
				if ((cw = (const CurrentWeekend*)AppGetSimData(kCurrentWeekend)) != NULL)
				{
					// Success!

				}
				
				if ((lc = (const LapCrossing*)AppGetSimData(kLapCrossing)) != NULL){}				
				if ((de = (const DriverEntry*)AppGetSimData(kDriverEntry)) != NULL)
				{
					// success!
				}
				if ((se = (const Standings*)AppGetSimData(kStandings)) != NULL) {}
				if ((si = (const SessionInfo*)AppGetSimData(kSessionInfo)) != NULL) {}
				
			}
			else if (newSample)
			{
				//printStatus();
				//printSample();
				if ((gd = (const GaugeData*)AppGetSimData(kGaugeData)) != NULL)
				{
					// success!
				}
				if ((cw = (const CurrentWeekend*)AppGetSimData(kCurrentWeekend)) != NULL)
				{
					// Success!

				}
				if ((lc = (const LapCrossing*)AppGetSimData(kLapCrossing)) != NULL)
				{

					// success
				}
				if ((de = (const DriverEntry*)AppGetSimData(kDriverEntry)) != NULL)
				{
					// success!
				}
				if ((se = (const Standings*)AppGetSimData(kStandings)) != NULL) {}

				if ((si = (const SessionInfo*)AppGetSimData(kSessionInfo)) != NULL) {}


				// While it is not strictly necessary that you call
				// this function, it is a very good idea to do so
				// after processing each sample.
				// The various individual pieces of telemetry that
				// make up this sample are only guaranteed to be
				// self-consistent until the next time you call
				// AppWaitForNewSample.  Once called, it is entirely
				// possible that some parts of the sample will be
				// from the prior sampling, and some parts from the
				// current sampling.  If you need the sample data
				// across calls to AppWaitForNewSample, you should
				// copy it to memory that you control.
				//AppClearSample();
			}

			// If, for some reason, our program went away for a while
			// and wasn't reading telemetry from the sim, the buffer
			// into which the sim writes the telemetry might have
			// filled up.  Whenever there is insufficient space
			// available for the sim to write telemetry into the buffer,
			// it simply discards the telemetry, and sets the flag
			// that is checked here.
			// You should carefully consider where you place this test.
			if (AppCheckIfSimDataOverrunQ())
			{
				// Note that this means that some telemetry was lost,
				// If it's important to your application that the
				// sample data be continuous (for example, if you're
				// controlling a motion platform), you would want to
				// deal with that possibility here.
				// If it's important to your application that it deal
				// with samples in real-time (back to the motion platform
				// idea), you should also note that since the telemetry
				// buffer is large enough to hold many samples, and since
				// it has overflowed, there are likely to be many samples
				// in the buffer that have been there for a while, and
				// should probably be considered 'stale'.
				//printf("*OVERRUN*\n");

				// The flag will remain set until we (the app) explicitly
				// clear it.  That is, the sim won't reset the overrun
				// flag once it sees that there is space available in
				// the buffer.
				AppClearSimDataOverrun();
			}
		}
		
		
	}

	return void();
}

NR2003_API uintptr_t GetGaugeData()
{
	uintptr_t retVal = 0;
	retVal = (uintptr_t)gd;
	return retVal;
}

NR2003_API uintptr_t GetCurrentWeekend()
{
	uintptr_t retVal = 0;	
	retVal = (uintptr_t)cw;
	return retVal;
}

NR2003_API uintptr_t GetLapCrossing()
{
	uintptr_t retVal = 0;	
	retVal = (uintptr_t)lc;
	return retVal;
}

NR2003_API uintptr_t GetDriverEntry()
{
	uintptr_t retVal = 0;
	retVal = (uintptr_t)de;
	return retVal;
}

NR2003_API uintptr_t GetStandings()
{
	uintptr_t retVal = 0;
	retVal = (uintptr_t)se;
	return retVal;
}

NR2003_API uintptr_t GetSessionInfo()
{
	uintptr_t retVal = 0;
	retVal = (uintptr_t)si;
	return retVal;
}


/*
 ***************************************************************************
 * Routines to generate the program's output.
 ***************************************************************************
 */
static const char* sessionType(int t)
{
	switch (t)
	{
	case kSessionTypeInvalid:		return "invalid";
	case kSessionTypeTesting:		return "testing";
	case kSessionTypePractice:		return "practice";
	case kSessionTypeQualifyLone:	return "lone qualifying";
	case kSessionTypeQualifyOpen:	return "open qualifying";
	case kSessionTypeRace:			return "race";
	}
	return "?";
}

static const char* sessionState(int s)
{
	switch (s)
	{
	case kSessionStateInvalid:		return "invalid";
	case kSessionStateGetInCar:	return "get in car";
	case kSessionStateWarmup:		return "warmup";
	case kSessionStateParadeLaps:	return "parade laps";
	case kSessionStateRacing:		return "racing";
	case kSessionStateCheckered:	return "checkered";
	case kSessionStateCoolDown:	return "cool down";
	}
	return "?";
}

static const char* sessionFlag(int f)
{
	switch (f)
	{
	case kFlagGreen:		return "green";
	case kFlagYellow:		return "yellow";
	case kFlagRed:			return "red";
	}
	return "?";
}


static float TEMP(float c)
{
	if (englishUnits)
		return 9.0f * c / 5.0f + 32.0f;		// degrees C to degrees F
	else
		return c;
}

static float PRESSURE(float p)
{
	if (englishUnits)
		return p * 0.1450377f;			// kPa to PSI
	else
		return p;
}

static float FORCE(float f)
{
	if (englishUnits)
		return f * 0.2248f;				// N to lb
	else
		return f;
}

static float DIST1(float d)
{
	if (englishUnits)
		return d * 3.281f;				// M to ft
	else
		return d;
}

static float DIST2(float d)
{
	if (englishUnits)
		return d * 39.372f;				// M to in
	else
		return d;
}

static float VEL(float v)
{
	if (englishUnits)
		return v * 2.237f;				// M/s to MPH
	else
		return v;
}

static float ANGLE(float a)
{
	if (englishUnits)
		return a * 180.0f / 3.14159f;	// rad to deg
	else
		return a;
}

static float TORQUE(float t)
{
	if (englishUnits)
		return FORCE(DIST1(t));			// N*m to Lb*ft
	else
		return t;
}

static float POWER(float t)
{
	if (englishUnits)
		return t / 745.7f;				// Watts to hp
	else
		return t;
}