#include <iostream>
#include <chrono>
#include <conio.h>  // for _kbhit and _getch on Windows
#include <string>   // for std::string
#include <sstream>  // for std::stringstream
#include <thread>   // for sleep_for

const std::string MARQUEE_STRING = "Hello Marquee!";
const int SCREEN_WIDTH = 80;
const int SCREEN_HEIGHT = 24;
const int REFRESH_DELAY = 100;

// Global variables declaration
int x = 0, y = 4;  
int x_step = 1, y_step = 1;  
bool isRunning = true;
std::string currentCommand;
std::stringstream outputBuffer;
int commandNo = 0;

void renderCommandPrompt();
void handleCommand();

void setCursorPosition(int x, int y) {
    std::cout << "\033[" << y << ";" << x << "H";
    std::cout.flush();
}

void clearLine(int y) {
    setCursorPosition(0, y);  
    std::cout << std::string(SCREEN_WIDTH, ' ') << std::flush;  
}

void moveMarquee() {
    // Clear the previous position
    clearLine(y);

    // Update the position
    x += x_step;
    y += y_step;

    // Bounce off the left and right edges
    if (x <= 0 || x >= SCREEN_WIDTH - MARQUEE_STRING.length()) {
        x_step = -x_step;
    }

    // Bounce off the top and bottom edges
    if (y <= 4 || y >= SCREEN_HEIGHT - 2) {
        y_step = -y_step;
    }
}

void renderMarquee() {
    //new position
    setCursorPosition(x, y);
    std::cout << MARQUEE_STRING << std::flush;
}

void renderStaticText() {
    setCursorPosition(0, 0);
    std::cout << "*****************************************" << std::endl;
    std::cout << "* Displaying a marquee console!         *" << std::endl;
    std::cout << "*****************************************" << std::endl;
    renderCommandPrompt();
}

void renderCommandPrompt() {
    // Clear the input line and redraw the command prompt
    clearLine(SCREEN_HEIGHT - 1);
    setCursorPosition(0, SCREEN_HEIGHT - 1);
    std::cout << "Enter a command for the MARQUEE_CONSOLE: " << currentCommand << std::flush;
}

void processInput() {
    if (_kbhit()) {  // If a key has been pressed
        char ch = _getch();
        if (ch == '\b' && !currentCommand.empty()) {  // Backspace handling
            currentCommand.pop_back();
        }
        else if (ch != '\n' && ch != '\r') {  // Any other character
            currentCommand.push_back(ch);
        }
        else {  // Enter key pressed
            handleCommand();
            currentCommand.clear();
            commandNo++;
        }
        renderCommandPrompt();
    }
}

void clearCommandList() {
    for (int i = 0; i < commandNo; ++i) {
        clearLine(SCREEN_HEIGHT + i);
    }
    commandNo = 0;
}

// Function to handle commands
void handleCommand() {
    if (currentCommand == "exit") {
        isRunning = false;
        setCursorPosition(0, SCREEN_HEIGHT);
    }
    else if (currentCommand == "clear") {
        clearCommandList();
        outputBuffer.str("");   // Clear the buffer
        outputBuffer.clear();   // Reset stringstream flags
        renderStaticText();     // Redraw the static text and prompt
        commandNo--;
    } else {
        outputBuffer << "Command processed in MARQUEE_CONSOLE: " << currentCommand << std::endl;
        setCursorPosition(0, SCREEN_HEIGHT + commandNo);
        std::cout << outputBuffer.str();
        outputBuffer.str("");   // Clear the buffer after printing
        outputBuffer.clear();   // Reset stringstream flags
    }
}


int main() {
    renderStaticText();
    while (isRunning) {
        processInput();
        moveMarquee();
        renderMarquee();
        std::this_thread::sleep_for(std::chrono::milliseconds(REFRESH_DELAY)); 
    }
    return 0;
}