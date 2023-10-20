// ReadProj.h : Include file for standard system include files,
// or project specific include files.
#include <string>
#include <stdio.h>
#include <iostream>
#include <windows.h>

class Button {
	std::string Name;
	HANDLE h;

public:

	void Render(int i);

	static void Refresh();

	Button(std::string);
	Button();
};

// TODO: Reference additional headers your program requires here.
