#include "ReadProj.h";

void Button::Render(int i) {
	SetConsoleTextAttribute(h, i);
	std::cout << this->Name << std::endl;
}

void Button::Refresh() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
	std::cout << "\x1B[2J\x1B[H";
}

Button::Button(std::string Name) {
	this->Name = Name;
	this->h = GetStdHandle(STD_OUTPUT_HANDLE);
}

Button::Button() {
	this->Name = "";
	this->h = GetStdHandle(STD_OUTPUT_HANDLE);
}