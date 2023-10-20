// ReadProj.cpp : Defines the entry point for the application.
//
#include <conio.h>
#include <cmath>
#include <iostream>
#include <ostream>
#include <stdio.h>
#include <string>
#include <windows.h>
#include <vector>
#include <matplot/matplot.h>
#include "ReadProj.h";

using namespace std;
using namespace matplot;


int Select(Button Buttons[], int arrSize, bool ShowTGraph = false) {
    int Count = 0;
    
    for (int i = 0; i < arrSize; i++)
    {
        if (Count == i)
            Buttons[i].Render(18);
        else
            Buttons[i].Render(3);
    }

    while (1) {
        switch (getch()) {
        case 72:
            Button::Refresh();
            Count--;
            Count = std::clamp(Count, 0, 3);

            if(ShowTGraph)
            std::cout << "6627 Terminal Graph" << endl << endl;

            for (int i = 0; i < arrSize; i++)
            {
                if (Count == i)
                    Buttons[i].Render(18);
                else
                    Buttons[i].Render(3);
            }
            break;
        case 80:
            Button::Refresh();
            Count++;
            Count = std::clamp(Count, 0, 3);

            if(ShowTGraph)
            std::cout << "6627 Terminal Graph" << endl << endl;

            for (int i = 0; i < arrSize; i++)
            {
                if (Count == i)
                    Buttons[i].Render(18);
                else
                    Buttons[i].Render(3);
            }
            break;
        case 13:
            return Count;
            break;
        }
    }
}


void New() {
    string COMPort = "";
    Button::Refresh();
    std::cout << "Please Enter your COM port #" << endl;
    std::cin >> COMPort;
    Button Buttons[2];
    Buttons[0] =  Button("2 Values");
    Buttons[1] =  Button("3 Values");

    COMPort = "\\\\.\\COM" + COMPort;

    char* char_array = new char[8];
    strcpy(char_array, COMPort.c_str());

    Button::Refresh();
    int Selection = Select(Buttons, 2);

    HANDLE usbPort = CreateFile(char_array,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (usbPort == INVALID_HANDLE_VALUE) {
        std::cerr << "Error: Unable to open " << COMPort << std::endl;
        return;
    }

    // Set the baud rate and other communication settings
    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    GetCommState(usbPort, &dcbSerialParams);
    dcbSerialParams.BaudRate = CBR_115200;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    SetCommState(usbPort, &dcbSerialParams);

    // Set the timeouts
    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    SetCommTimeouts(usbPort, &timeouts);

    // Read a line of text from the USB port
    std::string line, temp;
    const char* delimiterNum = "|";
    const char* delimiterTime = "#";
    const char* end = "~";
    const char* negative = "-";

    bool Run = true;

    std::vector<double> X = { 0 };
    std::vector<double> Y = { 0 };

    show();
    while (Run) {
        char buffer[512];
        DWORD bytesRead;
        ReadFile(usbPort, buffer, 512, &bytesRead, NULL);
        // Print the line of text to stdout
        bool firstFound = false;
        bool ReadValue = true;
        std::string Value = "";
        std::string Time = "";

        for (int i = 0; i < 512; i++) {
            if (buffer[i] == *end) {
                Run = false;
                break;
            }

            if (firstFound == false && buffer[i] != *delimiterNum) continue;

            if (buffer[i] == *delimiterNum) {
                ReadValue = true;
                if (!firstFound || Time == "" || Value == "") {
                    firstFound = true;
                    Value = "";
                    Time = "";
                    continue;
                }

                int ValueNum = std::stof(Value);
                int TimeNum = std::stoi(Time);

                Value = "";
                Time = "";

                //if (Y.size() >= 4 && std::sqrt(std::abs(ValueNum)) > std::sqrt(std::abs(Y.back())) * 50) continue;
                if (X.size() >= 4 && std::sqrt(std::abs(TimeNum)) > std::sqrt(std::abs(X.back())) * 5) continue;

                X.push_back(TimeNum);
                Y.push_back(ValueNum);

                continue;
            }

            if (buffer[i] == *delimiterTime) {
                ReadValue = false;
                Time = "";
                continue;
            }

            if (!isdigit(buffer[i]) && buffer[i] != *negative) continue;

            if (ReadValue) {
                Value += buffer[i];
            }
            else {
                Time += buffer[i];
            }

        }
        if (firstFound && X.size() > 5) {
            
            if (Selection == 0) scatter(X, Y);
            else scatter(X, Y, std::vector<double>{}, X);
        }
    }

    CloseHandle(usbPort);
}
void Help() {
    Button::Refresh();
    std::cout << "About: " << endl << endl;
    std::cout << "Project made by Neil K to pull data from vex v5 Brain." << std::endl;
    std::cout << "Send data over COMPort via the Vex Pros API using '|'" << std::endl;
    std::cout << "as the delimiter for y values and '#' as the delimiter for x values" << endl;
    std::string val;
    std::cin >> val;
    return;
}

static void Menu() {

    Button::Refresh();

    std::cout << "6627 Terminal Graph" << endl << endl;

    Button Buttons[4];

    Buttons[0] = Button("New");
    Buttons[1] = Button("Load (Non functional)");
    Buttons[2] = Button("Help");
    Buttons[3] = Button("About");

    string COMPort = "";

    int cval = Select(Buttons, 4, true);
    std::string val;
    switch (cval)
    {
    case 0:
        New();
        break;
    case 1:
        break;
    case 2:
        Button::Refresh();
        std::cout << "Help: " << endl << endl;
        std::cout << "Send data over COMPort via the Vex Pros API using '|'" << std::endl;
        std::cout << "as the delimiter for y values and '#' as the delimiter for x values" << endl;
        std::cout << "enter '.' to leave" << endl;
        std::cin >> val;
        if (val == ".")
            Menu();
        break;
    case 3:
        Button::Refresh();
        std::cout << "About: " << endl << endl;
        std::cout << "Project made by Neil K to pull data from vex v5 Brain." << std::endl;
        std::cout << "enter '.' to leave" << endl;
        std::cin >> val;
        if(val == ".")
            Menu();
        break;
    }

}


int main() {
    //scatter(X, Y);
    //show();
    Menu();
    // Close the USB port

    while (true);
    return 0;
}