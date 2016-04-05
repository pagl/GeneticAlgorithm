#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <string.h>
#include <iostream>

using namespace std;

//----------GENERATOR-LOSOWYCH-INSTANCJI------------

int randTimeForOperation() {
    return (int) (rand() % 80) + 20;
}

int randWhichMachine() {
    return (int) (rand() % 2) + 0;
}

int randTimeForPause() {
    int timeForPause = (int) (rand() % 80) + 20;
    return timeForPause;
}

int randWhenPauseStart(int sumOfTasksTime) {
    return (int)(rand() % (int) (0.45 * sumOfTasksTime) ) + 0;
}

void prepareTasksList(int ** taskList, int numberOfTasks) {
    for (int i = 0 ; i < 2 ; i++) {
        for (int j = 0 ; j < numberOfTasks ; j++) {
            taskList[i][j] = randTimeForOperation();
        }
    }
}

void randInstances(int numberOfFiles) {
    fstream plik;
    char fileName[15];
    srand(time(NULL));

    for (int N = 1 ; N <= numberOfFiles ; N++) {
        cout << "#" << N*100 << "...";
        int numberOfTasks = 100 * N;
        int numberOfPauses = (int) (0.115 * numberOfTasks);
        int sumOfTasksTime = 0;
        int ** pauseList = new int * [2];

        int ** taskList = new int * [2];
        taskList[0] = new int [numberOfTasks];
        taskList[1] = new int [numberOfTasks];

        prepareTasksList(taskList, numberOfTasks);

        itoa(numberOfTasks, fileName, 10);
        strncat(fileName, "RANDOM.txt", 10);
        plik.open(fileName, ios::out);

        plik << numberOfTasks << "\n";

        for (int i = 0 ; i < numberOfTasks ; i++) {
            plik << taskList[0][i] << ";" << taskList[1][i] << "\n";
            sumOfTasksTime += taskList[0][i] + taskList[1][i];
        }

        pauseList[0] = new int [50 * sumOfTasksTime];
        pauseList[1] = new int [50 * sumOfTasksTime];

        for (int i = 0 ; i < 2 ; i++) {
            for (int j = 0 ; j < 50 * sumOfTasksTime ; j++) {
                pauseList[i][j] = 0;
            }
        }

        for (int i = 0 ; i < numberOfPauses ; i++) {
            int machineNumber;
            int pauseStart;
            int timeForPause;
            while(true) {
                bool canFinish = true;
                machineNumber = randWhichMachine();
                pauseStart = randWhenPauseStart(sumOfTasksTime);
                timeForPause = randTimeForPause();
                for (int i = pauseStart ; i <= pauseStart + timeForPause ; i++) {
                    if(pauseList[machineNumber][i] != 0) {
                        canFinish = false;
                        break;
                    }
                }

                if(canFinish) {
                    for (int i = pauseStart ; i <= pauseStart + timeForPause ; i++) {
                        pauseList[machineNumber][i] = 1;
                    }
                    break;
                }

            }
            plik << i << ";" << machineNumber << ";" << timeForPause << ";" << pauseStart << "\n";
        }

        delete [] taskList[0];
        delete [] taskList[1];
        delete [] taskList;

        delete [] pauseList[0];
        delete [] pauseList[1];
        delete [] pauseList;

        plik.close();
        cout << "generated!\n";
    }
    cout << "Random Instances Generator has finished...\n";
}
//-------------------------------------------------


int main() {
    randInstances(1);
    return 0;
}
