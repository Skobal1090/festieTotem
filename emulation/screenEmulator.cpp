#include <iostream>
#include <windows.h>
#include <unistd.h>
using namespace std;



void printToScreen(){
    int f = 0;
    int seconds = 0;
    char buffer[8][32] = {};
    //gets size of array
    int rows = sizeof buffer / sizeof buffer[0];
    int cols = sizeof buffer[0] / sizeof(char);
    for (int x = 0; x < rows; x ++){
        for (int y = 0; y < cols; y ++){
            buffer[x][y] = ' ';
        }
    }

    while(true){
        //Ensures everything is printed to cout before clearing screen
        cout << flush;
        //Invokes system "CLS" command to wipe screen 
        system("CLS");

        //Prints array
        for (int i = 0; i < rows; i ++){
            for (int j = 0; j < cols; j ++){
                cout << buffer[i][j];
            } cout << endl;
        }

        cout << "\nSeconds Passed: " << seconds << endl;
        cout << "\nFrames Passed: " << f << endl;

        //Sets next buffer to be printed (temp)
        for (int x = 0; x < rows; x ++){
            for (int y = 0; y < cols; y ++){
                (f % cols == y) ? buffer[x][y] = '*': buffer[x][y] = ' ';
        }
    }

        //usleep takes microseconds as an argument, we chose a refresh rate of 5hz
        usleep(50000);
        if (f % 20 == 0) seconds++;
        f++;
    }
}

int main() {
    printToScreen();
    return 0;
}