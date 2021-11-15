// NR2003TesterApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <windows.h>
#include <iostream>
#include "NR2003.h"
#include "papytelemapp.h"
using namespace PapyTelem;

using namespace std;

static float laptime = 0.0f;

void ClearScreen()
{
    HANDLE                     hStdOut;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD                      count;
    DWORD                      cellCount;
    COORD                      homeCoords = { 0, 0 };

    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE) return;

    /* Get the number of cells in the current buffer */
    if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
    cellCount = csbi.dwSize.X * csbi.dwSize.Y;

    /* Fill the entire buffer with spaces */
    if (!FillConsoleOutputCharacter(
        hStdOut,
        (TCHAR)' ',
        cellCount,
        homeCoords,
        &count
    )) return;

    /* Fill the entire buffer with the current colors and attributes */
    if (!FillConsoleOutputAttribute(
        hStdOut,
        csbi.wAttributes,
        cellCount,
        homeCoords,
        &count
    )) return;

    /* Move the cursor home */
    SetConsoleCursorPosition(hStdOut, homeCoords);
}


int main()
{ 
    bool testFailed = false;
    const GaugeData* gdx;
    const LapCrossing* lcx;

    while (testFailed != true) {
        cout << "Testing the NR2003 DLL!" << endl << endl;

        

        // unit test for Setup()
        cout << "Test 1: Method Setup(): ";
        bool setupTest = Setup();
        cout << setupTest << endl;


        if (setupTest != true)
        {
            testFailed = true;
            break;
        }
            
        // unit test for WaitForSimToRun()
        cout << "Test 2: Method WaitForSimToRun(): ";
        bool waitForSimToRunTest = WaitForSimToRun();
        cout << waitForSimToRunTest << endl;        

        // get data from DLL
        bool testingDone = false;
        while (!testingDone) 
        {
            ClearScreen();
            // unit test for RequestData()
            cout << "Test 3: Method RequestData(): ";
            RequestData();
            cout << " OK" << endl;

            // unit test for GetGaugeData()
            cout << "Test 4: Method GetGaugeData(): ";
            uintptr_t gd = GetGaugeData();            
            gdx = (const GaugeData*)(void*)gd;

            cout << "gd = " << gd << endl;
            if (gdx != NULL) {
                cout << "RM = " << gdx->rpm << endl;
            }
            

            cout << " OK" << endl;

            // unit test for GetCurrentWeekend()
            cout << "Test 5: Method GetCurrentWeekend(): ";
            uintptr_t cw = GetCurrentWeekend();
            cout << "cw = " << cw;
            cout << " OK" << endl;

            // unit test for GetLapCrossing()
            cout << "Test 6: Method GetLapCrossing(): ";                       
            lcx = (const LapCrossing*)(void*)GetLapCrossing();
            if (lcx != NULL) {
                laptime = lcx->crossedAt;
            }
            cout << "lcx = " << lcx << endl;
            cout << "laptime = " << laptime << endl;

            cout << " OK" << endl;

            // unit test for GetDriverEntry()
            cout << "Test 7: Method GetDriverEntry(): ";
            uintptr_t de = GetDriverEntry();
            cout << "de = " << de;
            cout << " OK" << endl;
            Sleep(100);
        }

        
        if (testFailed)
        {
            cout << "TEST FAILED" << endl;
        }
        else {
            cout << "ALL TESTS SUCCESSFUL!" << endl;
        }
        break;
    }
    
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
