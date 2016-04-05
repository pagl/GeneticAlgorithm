#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <string.h>
#include <cstdlib>
#include <iostream>
#include <windows.h>


using namespace std;


/*###########__STRUCTURES__#############*/

//----------TASK-STRUCTURE----------
struct Task{
    int start_time;
    int time_length;
    int real_number;
    bool isPause;
    Task *next;

    Task();
    void add(int start_time, int time_length, int real_number, bool isPause, Task * &head);
};

Task::Task() {}

void Task::add(int start_time, int time_length, int real_number, bool isPause, Task * &head) {
    Task *new_task = new Task;
    Task *temp;

    new_task->start_time = start_time;
    new_task->time_length = time_length;
    new_task->real_number = real_number;
    new_task->isPause = isPause;
    new_task->next = NULL;

    temp = head;
    if(temp) {
        while(temp->next) temp = temp->next;
        temp->next = new_task;
    } else head = new_task;
}


//----------SOLUTION-STRUCTURE----------
struct Solution {
    Task *machine_1_sequence;
    Task *machine_2_sequence;
    int fitness_value;
    Solution *next;

    Solution();
    void add(Solution * &head);
};

Solution::Solution() {}

void Solution::add(Solution * &head) {
    Solution *new_solution = new Solution;
    Solution *temp;

    new_solution->machine_1_sequence = NULL;
    new_solution->machine_2_sequence = NULL;
    new_solution->fitness_value = 0;


    new_solution->next = NULL;

    temp = head;
    if(temp) {
        while(temp->next) temp = temp->next;
        temp->next = new_solution;
    } else head = new_solution;
}


//----------POPULATION-STRUCTURE----------
struct Population {
    Solution *solution;

    Population();
    void add_solution(Solution * &old_solution);
    int population_size();
};

Population::Population() {
    solution = NULL;
}

void Population::add_solution(Solution * &old_solution) {
    Task *machine1_task = old_solution->machine_1_sequence;
    Task *machine2_task = old_solution->machine_2_sequence;

    solution->add(solution);
    Solution *new_solution = solution;

    while(new_solution->next) new_solution = new_solution->next;

    while(machine1_task) {
        new_solution->machine_1_sequence->add(machine1_task->start_time, machine1_task->time_length, machine1_task->real_number, false, new_solution->machine_1_sequence);
        machine1_task = machine1_task->next;
    }

    while(machine2_task) {
        new_solution->machine_2_sequence->add(machine2_task->start_time, machine2_task->time_length, machine2_task->real_number, false, new_solution->machine_2_sequence);
        machine2_task = machine2_task->next;
    }
    new_solution->fitness_value = old_solution->fitness_value;
}

int Population::population_size() {
    Solution *temp_solution = solution;
    int population_size = 0;

    while(temp_solution) {
        population_size++;
        temp_solution = temp_solution->next;
    }

    return population_size;
}


//---------GLOBAL-VARIABLES----------------
Task *machine1_operations;
Task *machine2_operations;

Task *machine1_pauses;
Task *machine2_pauses;


/*###########__ALGORITHM-FUNCTIONS__#############*/

/**
    Funkcja ta zamienia miejscami dwa zadania.
    Jest ona uøywana w procesie mutacji.

    @param sequence wskaünik na poczπtek uszeregowania (pierwsze zadanie w szeregu)
    @param task1 wskaünik na pierwsze zadanie do zamiany
    @param task2 wskaünik na drugie zadanie do zamiany
*/
void swapTasks(Task *sequence, Task *task1, Task *task2) {

    Task *temp_sequence = sequence;
    Task *task1_prev, *task2_prev;
    Task *temp_task;

//--ZAMIANA-GDY-ZADANIE-PIERWSZE-JEST-NA-PIERWSZYM-MIEJSCU-USZEREGOWANIA
    if(task1->real_number == temp_sequence->real_number) {

         if(task1->next == task2) {

            task1->next = task2->next;
            task2->next = task1;
            sequence = task2;

            int tempStartTime = task1->start_time;
            task1->start_time = task2->start_time;
            task2->start_time = tempStartTime;
            return;
        }

        while(temp_sequence) {
            if(temp_sequence->next->real_number == task2->real_number) {
                task2_prev = temp_sequence;
                    break;
            }
            temp_sequence = temp_sequence->next;
        }

        temp_task = task1->next;
        task1->next = task2->next;
        task2->next = temp_task;

        task2_prev->next = task1;
        sequence = task2;

        int tempStartTime = task1->start_time;
        task1->start_time = task2->start_time;
        task2->start_time = tempStartTime;
        return;
    }

//--ZAMIANA-GDY-ZADANIE-DRUGIE-JEST-NA-PIERWSZYM-MIEJSCU-USZEREGOWANIA
    if(task2->real_number == temp_sequence->real_number) {

        if(task2->next == task1) {

            task2->next = task1->next;
            task1->next = task2;
            sequence = task1;

            int tempStartTime = task1->start_time;
            task1->start_time = task2->start_time;
            task2->start_time = tempStartTime;
            return;
        }

        while(temp_sequence) {
                if(temp_sequence->next->real_number == task1->real_number) {
                    task1_prev = temp_sequence;
                    break;
                }
            temp_sequence = temp_sequence->next;
        }

        temp_task = task1->next;
        task1->next = task2->next;
        task2->next = temp_task;

        task1_prev->next = task2;
        sequence = task1;

        int tempStartTime = task1->start_time;
        task1->start_time = task2->start_time;
        task2->start_time = tempStartTime;
        return;
    }

//--ZAMIANA-GDY-ZADANIA-NASTEPUJA-JEDEN-PO-DRUGIM
    if(task1->next == task2) {
        while(temp_sequence) {
            if (temp_sequence->next->real_number == task1->real_number) {
                task1_prev = temp_sequence;
                break;
            }
            temp_sequence = temp_sequence->next;
        }
        task1_prev->next = task2;

        temp_task = task2->next;
        task2->next = task1;
        task1->next = temp_task;

        int tempStartTime = task1->start_time;
        task1->start_time = task2->start_time;
        task2->start_time = tempStartTime;
        return;
    }

    if(task2->next == task1) {
        while(temp_sequence) {
            if (temp_sequence->next->real_number == task2->real_number) {
                task2_prev = temp_sequence;
                break;
            }
            temp_sequence = temp_sequence->next;
        }
        task2_prev->next = task1;

        temp_task = task1->next;
        task1->next = task2;
        task2->next = temp_task;

        int tempStartTime = task1->start_time;
        task1->start_time = task2->start_time;
        task2->start_time = tempStartTime;
        return;
    }

//--ZAMIANA-W-KAZDYM-INNYM-PRZYPADKU
    while(temp_sequence) {

        if(!temp_sequence->next) break;
        if(temp_sequence->next->real_number == task1->real_number) task1_prev = temp_sequence;
        if(temp_sequence->next->real_number == task2->real_number) task2_prev = temp_sequence;

        temp_sequence = temp_sequence->next;
    }

    task1_prev->next = task2;
    task2_prev->next = task1;

    temp_task = task1->next;
    task1->next = task2->next;
    task2->next = temp_task;

    int tempStartTime = task1->start_time;
    task1->start_time = task2->start_time;
    task2->start_time = tempStartTime;
}


/**
    Funkcja ta zwraca wskaünik na zadanie znajdujπce siÍ w rozwiπzaniu.

    @param sequence wskaünik na uszeregowanie (pierwsze zadanie w szeregu)
    @param task2 wskaünik na poszukiwane zadanie

    @return wskaünik na poszukiwane zadanie dla danego uszeregowania
*/
Task *takeFromSequence(Task *sequence, Task *task2){

    Task *temp;
    Task *takenTask;
    temp = sequence;

    if(task2 == sequence){
        if(sequence){
            takenTask = sequence;
            sequence = sequence->next;
        }

    } else {

        while(temp->next != task2){
            temp = temp->next;
        }

        takenTask = temp->next;
        temp->next = task2->next;

    }
    return takenTask;
}


/**
    Funkcja sprawdza czy zadanie koliduje z przerwπ konserwujπcπ.

    @param task wskaünik na zadanie, ktÛre bÍdzie porÛwnywane
    @param pauses wskaünik na listÍ przerw

    @return true or false, w zaleønoúci od tego czy koliduje zadanie z pauzπ czy nie
*/
bool isCollideWithPause(Task *task, Task *pauses){

    Task *pause = pauses;

    while(pause){
        for(int i = task->start_time ; i <= task->start_time + task->time_length ; i++) {
            if(i > pause->start_time && i < pause->start_time + pause->time_length) {
                return true;
            }
        }
        pause = pause->next;
    }

    return false;
}


/**
    Funkcja zwracajπca moøliwy czas startu zadania.
    Uøywana jest tylko, gdy badamy czas startu operacji drugiej zadania.

    @param solution wskaünik na dane uszeregowanie (pierwsze zadanie w uszeregowaniu).
    @param task wskaünik na zadanie, dla ktÛrego znajdujemy najbliøszy moøliwy czas startu.

    @return moøliwy czas startu podanego zadania.
*/
int returnPossibleStartTime(Solution *solution, Task *task){

    Task *next_task = solution->machine_1_sequence;

    while (next_task->real_number != task->real_number) {
        next_task = next_task->next;
        if(!next_task) return 0;

    }
    return next_task->start_time + next_task->time_length;

}


/**
    Funkcja ta wstawia nowe zadanie w najbliøsze moøliwe miejsce uwzglÍdniajπÊ wszystkie
    kryteria problemu szeregowania zadaÒ.

    @param solution wskaünik na dane uszeregowanie do ktÛrego ma zostaÊ dodane nowe zadanie.
    @param task wskaünik na zadanie, ktÛre ma zostaÊ dodane.
    @param machine_number numer maszyny zgodnej z danym uszeregowaniem.
*/
void insertToSequence(Solution *solution, Task *task, int machine_number) {

    if(machine_number == 1) {

        int task_start_time = 0;
        int task_finish_time = task_start_time + task->time_length;
        bool checkEverythingAgain = true;

        while(checkEverythingAgain) {
            checkEverythingAgain = false;

            bool checkTasksAgain = true;
            while(checkTasksAgain) {
                Task *solution_task = solution->machine_1_sequence;
                while(solution_task) {
                    for(int i = task_start_time ; i <= task_finish_time ; i++) {
                        if(i > solution_task->start_time && i < solution_task->start_time + solution_task->time_length) {
                            task_start_time = solution_task->start_time + solution_task->time_length;
                            task_finish_time = task_start_time + task->time_length;
                            checkTasksAgain = true;
                            checkEverythingAgain = true;
                            break;
                        }
                    }
                    checkTasksAgain = false;
                    solution_task = solution_task->next;
                }
            }

            bool checkPausesAgain = true;
            while(checkPausesAgain) {
                Task *pauseM1 = machine1_pauses;
                while(pauseM1) {
                    for(int i = task_start_time ; i <= task_finish_time ; i++) {
                        if(i > pauseM1->start_time && i < pauseM1->start_time + pauseM1->time_length) {
                            task_start_time = pauseM1->start_time + pauseM1->time_length;
                            task_finish_time = task_start_time + task->time_length;
                            checkPausesAgain = true;
                            checkEverythingAgain = true;
                            break;
                        }
                    }
                    checkPausesAgain = false;
                    pauseM1 = pauseM1->next;
                }
            }
        }

        Task *solution_task = solution->machine_1_sequence;
        Task *new_task = new Task;
        new_task->start_time = task_start_time;
        new_task->real_number = task->real_number;
        new_task->isPause = false;
        new_task->time_length = task->time_length;
        new_task->next=NULL;

        if(task_start_time < solution_task->start_time) {

            new_task->next = solution_task;
            solution->machine_1_sequence = new_task;

        } else {
            while(solution_task) {
                if(!solution_task->next) {
                    solution_task->next = new_task;
                    break;
                }

                if(solution_task->next->start_time > task_start_time) {

                    Task *temp_task = solution_task->next;
                    solution_task->next = new_task;
                    new_task->next = temp_task;
                    break;
                }
                solution_task = solution_task->next;
            }
        }

    } else if(machine_number == 2) {

        int task_start_time = returnPossibleStartTime(solution, task);
        int task_finish_time = task_start_time + task->time_length;
        bool checkEverythingAgain = true;

        while(checkEverythingAgain) {
            checkEverythingAgain = false;

            bool checkTasksAgain = true;
            while(checkTasksAgain) {
                Task *solution_task = solution->machine_2_sequence;
                while(solution_task) {
                    for(int i = task_start_time ; i <= task_finish_time ; i++) {
                        if(i > solution_task->start_time && i < solution_task->start_time + solution_task->time_length) {
                            task_start_time = solution_task->start_time + solution_task->time_length;
                            task_finish_time = task_start_time + task->time_length;
                            checkTasksAgain = true;
                            checkEverythingAgain = true;
                            break;
                        }
                    }
                    checkTasksAgain = false;
                    solution_task = solution_task->next;
                }
            }

            bool checkPausesAgain = true;
            while(checkPausesAgain) {
                Task *pauseM2 = machine2_pauses;
                while(pauseM2) {
                    for(int i = task_start_time ; i <= task_finish_time ; i++) {
                        if(i > pauseM2->start_time && i < pauseM2->start_time + pauseM2->time_length) {
                            task_start_time = pauseM2->start_time + pauseM2->time_length;
                            task_finish_time = task_start_time + task->time_length;
                            checkPausesAgain = true;
                            checkEverythingAgain = true;
                            break;
                        }
                    }
                    checkPausesAgain = false;
                    pauseM2 = pauseM2->next;
                }
            }
        }

        Task *solution_task = solution->machine_2_sequence;
        Task *new_task = new Task;
        new_task->start_time = task_start_time;
        new_task->real_number = task->real_number;
        new_task->isPause = false;
        new_task->time_length = task->time_length;
        new_task->next=NULL;


        if(task_start_time < solution_task->start_time) {

           new_task->next = solution_task;
            solution->machine_2_sequence = new_task;

        } else {
            while(solution_task) {
                if(!solution_task->next) {
                    solution_task->next = new_task;
                    break;
                }

                if(solution_task->next->start_time > task_start_time) {

                    Task *temp_task = solution_task->next;
                    solution_task->next = new_task;
                    new_task->next = temp_task;
                    break;
                }
                solution_task = solution_task->next;
            }
        }
    }

}


/**
    Funkcja odpowiedzialna za naprawÍ ewentualnych b≥ÍdÛw w osobniku
    powsta≥ych w wyniku operacji mutacji i krzyøowania.

    @param solution wskaünik na rozwiπzanie, ktÛre ma zostaÊ sprawdzone i naprawione.
    @param numberOfTasks liczba zadaÒ dla danego uszeregowania.
*/
void repair(Solution *solution, int numberOfTasks){

    Task *task1;
    Task *temp_task;
    Task *task2;
    bool tasks[numberOfTasks];

//--USUWANIE-POWIELEN-NA-MASZYNIE-1
    for(int i = 0 ; i < numberOfTasks ; i++) tasks[i] = false;

    task1 = solution->machine_1_sequence;
    while(task1) {
        if(tasks[task1->real_number]){

            temp_task = task1->next;
            delete takeFromSequence(solution->machine_1_sequence, task1);
            task1 = temp_task;

        } else {
            tasks[task1->real_number] = true;
            task1 = task1->next;
        }
    }

//--DOPISYWANIE-BRAKUJACYCH-ZADAN-DO-MASZYNY-1
    for(int i = 0 ; i < numberOfTasks ; i++){

        if(!tasks[i]){

            task1 = machine1_operations;

            while(task1->real_number != i){
                task1 = task1->next;
            }

            insertToSequence(solution, task1, 1);
        }
    }

//--USUWANIE-POWIELEN-NA-MASZYNIE-2
    for(int i = 0 ; i < numberOfTasks ; i++) tasks[i] = false;

    task1 = solution->machine_2_sequence;
    while(task1) {
        if(tasks[task1->real_number]){
            temp_task = task1->next;
            delete takeFromSequence(solution->machine_2_sequence, task1);
            task1 = temp_task;
        } else {
            tasks[task1->real_number] = true;
            task1 = task1->next;
        }
    }

//--DOPISYWANIE-BRAKUJACYCH-ZADAN-NA-MASZYNIE-2
    for(int i = 0 ; i < numberOfTasks ; i++){

        if(!tasks[i]) {
            task1 = machine2_operations;

            while(task1->real_number != i){
                task1 = task1->next;
            }

            insertToSequence(solution, task1, 2);
        }
    }

//--PRZESTAWIANIE-NIEPASUJACYCH-ZADAN-NA-MASZYNIE-1
    task1 = solution->machine_1_sequence;

    while(task1->next){

        if( (task1->start_time + task1->time_length > task1->next->start_time) || isCollideWithPause(task1, machine1_pauses) ){
            task2 = task1->next;
            temp_task = takeFromSequence(solution->machine_1_sequence, task1);
            Task *global_task=machine1_operations;

            while(global_task->real_number != temp_task->real_number) global_task = global_task->next;

            insertToSequence(solution, global_task, 1);
            //delete temp_task;

            task1 = task2;

        } else task1=task1->next;

    }

//--PRZESTAWIANIE-NIEPASUJACYCH-ZADAN-NA-MASZYNIE-2
    task1 = solution->machine_2_sequence;
    while(task1->next){

        if( (task1->start_time + task1->time_length > task1->next->start_time) || isCollideWithPause(task1, machine2_pauses) || returnPossibleStartTime(solution, task1) > task1->start_time){
            task2 = task1->next;
            temp_task = takeFromSequence(solution->machine_2_sequence, task1);

            Task *global_task=machine2_operations;

            while(global_task->real_number != temp_task->real_number) global_task = global_task->next;

            insertToSequence(solution, global_task, 2);
            //delete temp_task;

            task1 = task2;
        } else task1 = task1->next;
    }

}


/**
    Operator algorytmu genetycznego - mutacja. Odpowiedzialny za zamianÍ ze sobπ dwÛch
    zadaÒ w rozwiπzaniu (ang. bit swap mutation).

    @param old_solution wskaünik na stare rozwiπzanie
    @param numberOfTasks liczba zadaÒ w danej instancji problemu.
    @param population wskaünik na ca≥π nowπ populacjÍ, do ktÛrej zostanie dodane nowe rozwiπzanie.
*/
void mutate(Solution *old_solution, int numberOfTasks, Population *population) {

    population->add_solution(old_solution);
    Solution *new_solution = population->solution;
    while(new_solution->next) new_solution = new_solution->next;

    Task *z1_op1 = new_solution->machine_1_sequence;                //---WSKAZNIK-NA-ZADANIE-1-NA-MASZYNIE-1
    Task *z2_op1 = new_solution->machine_1_sequence;                //---WSKAZNIK-NA-ZADANIE-2-NA-MASZYNIE-1
    Task *z1_op2 = new_solution->machine_2_sequence;                //---WSKAZNIK-NA-ZADANIE-1-NA-MASZYNIE-2
    Task *z2_op2 = new_solution->machine_2_sequence;                //---WSKAZNIK-NA-ZADANIE-2-NA-MASZYNIE-2

    int z1_rand = rand() % (numberOfTasks) ;                           //---LOSOWANIE-NUMERU-ZADANIA-1-NA-MASZYNIE-1
    for(int i = 0 ; i < z1_rand ; i++) z1_op1 = z1_op1->next;             //---PRZESUNIECIE-WSKAZNIKA-DO-TEGO-ZADANIA

    int z2_rand = rand() % (numberOfTasks);                               //---LOSOWANIE-NUMERU-ZADANIA-2-NA-MASZYNIE-1
    while (z1_rand == z2_rand) z2_rand = rand() % (numberOfTasks);         //---SPRAWDZANIE-CZY-NIE-WYLOSOWANO-TEGO-SAMEGO-ZADANIA

    for(int i = 0 ; i < z2_rand ; i++) z2_op1 = z2_op1->next;             //---PRZESUNIECIE-WSKAZNIKA-DO-TEGO-ZADANIA

//--WYSZUKANIE-TYCH-ZADAN-NA-MASZYNIE-DRUGIEJ-I-USTAWIENIE-WSKAZNIKA
    while(z1_op2->real_number != z1_op1->real_number) z1_op2 = z1_op2->next;
    while(z2_op2->real_number != z2_op1->real_number) z2_op2 = z2_op2->next;

    swapTasks(new_solution->machine_1_sequence, z1_op1, z2_op1);            //---ZAMIANA-ZADAN-NA-MASZYNIE-1
    swapTasks(new_solution->machine_2_sequence, z1_op2, z2_op2);            //---ZAMIANA-ZADAN-NA-MASZYNIE-2

    repair(new_solution, numberOfTasks);               //---NAPRAWIENIE-EWENTUALNIE-POWSTALYCH-BLEDOW-USZEREGOWANIA
}


/**
    Operator algorytmu genetycznego - krzyøowanie. Krzyøowanie jest wykonywane
    w dwÛch punktach, co oznacza, øe losowany jest przedzia≥, ktÛry bÍdzie punktem
    podzia≥u dwÛch starych rozwiπzaÒ, w dwa nowe.

    @param old_solution1 wskaünik na pierwsze stare rozwiπzanie (rodzic 1).
    @param old_population wskaünik na starπ populacjÍ rozwiπzaÒ.
    @param numberOfTasks liczba zadaÒ dla danego problemu.
    @param population wskaünik na nowπ populacjÍ do ktÛrej zostanπ dodane nowe rozwiπzania.
*/
void crossover(Solution *old_solution1, Population *old_population, int numberOfTasks, Population *population) {

//--SZUKANIE-DRUGIEGO-,-INNEGO-ROZWIAZANIA-ZE-STAREJ-POPULACJI
    Solution *old_solution2;
    do {
        old_solution2 = old_population->solution;
        int rand_second_solution = rand() % (old_population->population_size() - 1);
        for(int i = 0 ; i < rand_second_solution ; i++) old_solution2 = old_solution2->next;
    } while (old_solution1->fitness_value == old_solution2->fitness_value);

//--DODANIE-PIERWSZEGO-STAREGO-ROZWIAZANIA-DO-NOWEJ-POPULLACJI-I-ZNALEZIENIE-GO
    population->add_solution(old_solution1);
    Solution *new_solution1 = population->solution;
    while(new_solution1->next) new_solution1 = new_solution1->next;

//--DODANIE-DRUGIEGO-STAREGO-ROZWIAZANIA-DO-NOWEJ-POPULACJI-I-ZNALEZIENIE-GO
    population->add_solution(old_solution2);
    Solution *new_solution2 = population->solution;
    while(new_solution2->next) new_solution2 = new_solution2->next;

/*KRZYZOWANIE-NA-MASZYNIE-1*/
    Task *start_task1 = new_solution1->machine_1_sequence;
    Task *over_task1 = new_solution1->machine_1_sequence;
    Task *start_task2 = new_solution2->machine_1_sequence;
    Task *over_task2 = new_solution2->machine_1_sequence;
    Task *before_start_task1 = new_solution1->machine_1_sequence;
    Task *before_start_task2 = new_solution2->machine_1_sequence;
    Task *after_over_task1;
    Task *after_over_task2;

    int task_start_time;
    int rand_start_range;
    int rand_over_range;


    do {
        rand_start_range = rand() % (numberOfTasks - 2) + 1;
        rand_over_range = rand() % (numberOfTasks - 2) + 1;
    } while(rand_start_range == rand_over_range || rand_start_range == rand_over_range + 1 || rand_start_range == rand_over_range - 1);

    if(rand_start_range > rand_over_range) {
        int temp = rand_start_range;
        rand_start_range = rand_over_range;
        rand_over_range = temp;
    }

//--PRZESUNIECIE-WSKAZNIKA-NA-POCZATEK-ZAKRESU
    for(int i = 0 ; i < rand_start_range ; i++){
        start_task1 = start_task1->next;
        start_task2 = start_task2->next;
    }

//--PRZESUNIECIE-WSKAZNIKA-NA-KONIEC-ZAKRESU
    for(int i = 0 ; i < rand_over_range ; i++){
        over_task1 = over_task1->next;
        over_task2 = over_task2->next;
    }

    while(before_start_task1->next != start_task1) before_start_task1 = before_start_task1->next;
    while(before_start_task2->next != start_task2) before_start_task2 = before_start_task2->next;

    after_over_task1 = over_task1->next;
    after_over_task2 = over_task2->next;

    task_start_time = start_task1->start_time;
    start_task1->start_time = start_task2->start_time;
    start_task2->start_time = task_start_time;

    before_start_task1->next = start_task2;
    before_start_task2->next = start_task1;
    over_task1->next = after_over_task2;
    over_task2->next = after_over_task1;

//--AKTUALIZACJA-CZASOW-ROZPOCZECIA
    before_start_task1 = start_task1;
    while(before_start_task1 != over_task1) {
        before_start_task1->next->start_time = before_start_task1->start_time + before_start_task1->time_length;
        before_start_task1 = before_start_task1->next;
    }

    before_start_task2 = start_task2;
    while(before_start_task2 != over_task2) {
        before_start_task2->next->start_time = before_start_task2->start_time + before_start_task2->time_length;
        before_start_task2 = before_start_task2->next;
    }

/*KRZYZOWANIE-NA-MASZYNIE-2*/
    start_task1 = new_solution1->machine_2_sequence;
    start_task2 = new_solution2->machine_2_sequence;
    before_start_task1 = new_solution1->machine_2_sequence;
    before_start_task2 = new_solution2->machine_2_sequence;
    over_task1=new_solution1->machine_2_sequence;
    over_task2=new_solution2->machine_2_sequence;
//--PRZESUNIECIE-WSKAZNIKA-NA-POCZATEK-ZAKRESU
    for(int i = 0 ; i < rand_start_range ; i++){
        start_task1 = start_task1->next;
        start_task2 = start_task2->next;
    }

//--PRZESUNIECIE-WSKAZNIKA-NA-KONIEC-ZAKRESU
    for(int i = 0 ; i < rand_over_range ; i++){
        over_task1 = over_task1->next;
        over_task2 = over_task2->next;
    }

    while(before_start_task1->next != start_task1) before_start_task1 = before_start_task1->next;
    while(before_start_task2->next != start_task2) before_start_task2=before_start_task2->next;

    after_over_task1 = over_task1->next;
    after_over_task2 = over_task2->next;

    task_start_time = start_task1->start_time;
    start_task1->start_time = start_task2->start_time;
    start_task2->start_time = task_start_time;

    over_task1->next = after_over_task2;
    over_task2->next = after_over_task1;
    before_start_task1->next = start_task2;
    before_start_task2->next = start_task1;

//--AKTUALIZACJA-CZASOW-ROZPOCZECIA
    before_start_task1 = start_task1;
    while(before_start_task1 != over_task1){
        before_start_task1->next->start_time = before_start_task1->start_time + before_start_task1->time_length;
        before_start_task1 = before_start_task1->next;
    }

    before_start_task2=start_task2;
    while(before_start_task2 != over_task2){
        before_start_task2->next->start_time = before_start_task2->start_time + before_start_task2->time_length;
        before_start_task2 = before_start_task2->next;
    }

    repair(new_solution1, numberOfTasks);
    repair(new_solution2, numberOfTasks);
}


//---------------------SELECTION-OPERATOR---------------------------


/**
    Funkcja wyliczajπca wartoúÊ funkcji celu, dla kaødego rozwiπzania
    w populacji.

    @param population wskaünik na populacjÍ, dla ktÛrej ma zostaÊ wyliczona wartoúÊ funkcji celu.
*/
void calculate_fitness(Population *population) {
    Solution *solution = population->solution;                                      //---WSKAZNIK-NA-PIERWSZE-ROZWIAZANIE-W-POPULACJI
    int maxFitnessValue = 0;                                                        //---ZMIENNA-PRZECHOWUJACA-WARTOSC-NAJWIEKSZEJ-FUNKCJI

//--DLA-KAZDEGO-ROZWIAZANIA-WYLICZANA-JEST-WARTOSC-FUNKCJI-CELU-(IM-WIEKSZA-WARTOSC-TYM-LEPSZE-USZEREGOWANIE)
    while(solution) {
        Task *machine1_task = solution->machine_1_sequence;                         //---WSKAZNIK-NA-ZADANIE-DLA-DANEGO-ROZWIAZANIA-DLA-MASZYNY-1
        Task *machine2_task = solution->machine_2_sequence;                         //---WSKAZNIK-NA-ZADANIE-DLA-DANEGO-ROZWIAZANIA-DLA-MASZYNY-2
        int fitnessValue = 0;                               //---WARTOSC-FUNKCJI-CELU

//------SUMOWANIE-CZASOW-ZAKONCZENIA-OPERACJI-DLA-MASZYNY-1
        while(machine1_task) {
            fitnessValue += machine1_task->start_time + machine1_task->time_length;
            machine1_task = machine1_task->next;
        }

//------SUMOWANIE-CZASOW-ZAKONCZENIA-OPERACJI-DLA-MASZYNY-2
        while(machine2_task) {
            fitnessValue += machine2_task->start_time + machine2_task->time_length;
            machine2_task = machine2_task->next;
        }

//------SPRAWDZANIE-CZY-AKTUALNA-WARTOSC-FUNKCJI-CELU-JEST-NAJWIEKSZA
        if(fitnessValue > maxFitnessValue) {
            maxFitnessValue = fitnessValue + 1;
        }

        solution->fitness_value = fitnessValue;     //---ZAPISANIE-WARTOSCI-FUNKCJI-CELU-(TU-JESZCZE-WIEKSZA-WARTOSC-JEST-GORSZYM-USZEREGOWANIEM)
        solution = solution->next;
    }
    solution = population->solution;

//--ZAMIANA-WARTOSCI-FUNKCJI-CELU-(IM-WIEKSZA-TERAZ-JEST-LEPSZYM-USZEREGOWANIEM)
    while(solution) {
        solution->fitness_value = maxFitnessValue - solution->fitness_value;
        solution = solution->next;
    }
}


/**
    Funkcja odpowiedzialna za przeprowadzenie selekcji metodπ turniejowπ.

    @param population wskaünik na populacjÍ dla ktÛrej ma zostaÊ przeprowadzona selekcje turniejowa.
    @param new_population wskaünik na nowπ populacjÍ do ktÛrej bÍdπ dodawane wybrane rozwiπzania.
    @param new_population_size rozmiar nowej powsta≥ej populacji.
*/
void tournament_selection(Population *population, Population *new_population, int new_population_size) {
    Solution *solution1;
    Solution *solution2;
    int population_size = population->population_size();
    int solution1_number;
    int solution2_number;

    int popArray[population_size];
    for(int i = 0 ; i < population_size ; i++) {
        popArray[i] = 0;
    }

    for(int i = 0 ; i < new_population_size ; i++) {

        solution1 = population->solution;
        solution2 = population->solution;

        while(true) {
            solution1_number = rand() % (population_size - 1);
            if(popArray[solution1_number] == 0) {
                popArray[solution1_number] = 1;
                break;
            }
        }

        while(true) {
            solution2_number = rand() % (population_size - 1);
            if(popArray[solution2_number] == 0) {
                popArray[solution2_number] = 1;
                break;
            }
        }

        for(int i = 0 ; i < solution1_number ; i++) solution1 = solution1->next;
        for(int i = 0 ; i < solution2_number ; i++) solution2 = solution2->next;

        if(solution1->fitness_value > solution2->fitness_value) {
            popArray[solution2_number] = 0;
            new_population->add_solution(solution1);
        } else {
            popArray[solution1_number] = 0;
            new_population->add_solution(solution2);
        }
    }
}


/**
    Operator algorytmu genetycznego - selekcja.

    @param population wskaünik na populacjÍ dla ktÛrej ma zostaÊ przeprowadzona selekcja.
    @param new_population wskaünik na nowπ populacjÍ do ktÛrej zostanπ dodane wybrane rozwiπzania.
    @param new_population_size rozmiar nowej populacji.
*/
void selection(Population *population, Population* new_population, int new_population_size) {

    calculate_fitness(population);
    tournament_selection(population, new_population, new_population_size);
}


/**
    Funkcja generujπca startowπ populacjÍ.

    @param numberOfTasks liczba zadaÒ w danej instancji.
    @param sizeOfPopulation rozmiar populacji startowej
    @param population wskaünik na populacjÍ do ktÛrej bÍdπ dodawane wygenerowane rozwiπzania.

    @return liczbÍ zadaÒ, dla danej instancji.
*/
int generate_population(int numberOfTasks, int sizeOfPopulation, Population *population) {
    for(int i = 0 ; i < sizeOfPopulation ; i++) {

        population->solution->add(population->solution);            //---DODANIE-NOWEGO-ROZWIAZANIA-DO-LISTY-ROZWIAZA—
        Solution *solution = population->solution;
        for(int j = 0 ; j < i; j++) solution = solution->next;      //---PRZESUNIECIE-WSKAZNNIKA-DO-AKTUALNIE-GENEROWANEGO-ROZWIAZANIA

        int taskOrder[numberOfTasks], usedTasks[numberOfTasks];
        for(int j = 0 ; j < numberOfTasks ; j++) usedTasks[j] = 0;

//------GENEROWANIE-KOLEJNOåCI-ZADA—-NA-PIERWSZEJ-MASZYNIE
        for(int j = 0 ; j < numberOfTasks ; j++) {
            while(true){
                int taskNumber =(int) (rand()% numberOfTasks);
                if(usedTasks[taskNumber] == 0) {
                    usedTasks[taskNumber] = 1;
                    taskOrder[j] = taskNumber;
                    break;
                }
            }
        }

//------DODAWANIE-TASK”W-DO-AKTUALNIE-GENEROWANEGO-ROZWIAZANIA
        for(int j = 0 ; j < numberOfTasks ; j++) {



/*__________GENEROWANIE-USZEREGOWANIA-DLA-MASZYNY-1*/
            int taskNumber = taskOrder[j];                  //---RZECZYWISTY-NUMER-ZADANIA-KTORE-MA-ZOSTAC-DODANE-DO-ROZWIAZANIA
            int task_start_timeM1 = 0;                      //---MOMENT-CZASU-W-KTORYM-AKTUALNIE-DODAWANE-ZADANIE-MOZE-ROZPOCZAC-WYKONYWANIE
            Task *machine1Task, *sequenceM1;

            machine1Task = machine1_operations;             //---WSKAèNIK-NA-ZADANIE-Z-LISTY-ZADA—-WCZYTANYCH-Z-PLIKU
            sequenceM1 = solution->machine_1_sequence;      //---WSKAèNIK-POMOCNICZY-NA-AKTUALNIE-GENEROWANE-ROZWIAZANIE-DLA-MASZYNY-PIERWSZEJ

//----------PRZESUNIECIE-WSKAZNIKA-NA-POZYCJE-KTOREMU-ODPOWIADA-WCZESNIEJ-WYGENEROWANY-NUMER-ZADANIA
            for(int k = 0; k < taskNumber ; k++) {
                machine1Task = machine1Task->next;
            }

//----------ZNALEZIENIE-NAJBLIZSZEGO-MOZLIWEGO-CZASU-KIEDY-ZADANIE-MOZE-ROZPOCZAC-WYKONYWANIE
            while(sequenceM1) {
                task_start_timeM1 = sequenceM1->start_time + sequenceM1->time_length;
                sequenceM1 = sequenceM1->next;
            }

            Task *pauseM1;                                  //---WSKAZNIK-NA-LISTE-PRZERW-DLA-MASZYNY-1
            bool checkAgain = true;

//----------ZNAJDOWANIE-NOWEGO-MOZLIWEGO-CZASU-ROZPOCZECIA-WYKONYWANIA-ZADANIA-W-ZALEZNOSCI-OD-PRZERW
            while(checkAgain){
                pauseM1 = machine1_pauses;
                while(pauseM1) {
                    for(int k = task_start_timeM1 ; k <= task_start_timeM1 + machine1Task->time_length ; k++) {
                        if(k > pauseM1->start_time && k < pauseM1->start_time + pauseM1->time_length) {
                            task_start_timeM1 = pauseM1->start_time + pauseM1->time_length;
                            checkAgain = true;
                            break;
                        }
                    }
                    checkAgain = false;
                    pauseM1 = pauseM1->next;
                }
            }
            solution->machine_1_sequence->add(task_start_timeM1, machine1Task->time_length, machine1Task->real_number, false, solution->machine_1_sequence);



/*__________GENEROWANIE-USZEREGOWANIA-DLA-MASZYNY-2*/
            Task *machine2Task, *sequenceM2;
            int task_start_timeM2 = 0;                      //---MOMENT-CZASU-W-KTORYM-AKTUALNIE-DODAWANE-ZADANIE-MOZE-ROZPOCZAC-WYKONYWANIE

            machine2Task = machine2_operations;             //---WSKAZNIK-NA-ZADANIE-Z-LISTY-ZADAN-WCZYTANYCH-Z-PLIKU
            sequenceM2 = solution->machine_2_sequence;      //---WSKAZNIK-POMOCNICZY-NA-AKTUALNIE-GENEROWANE-ROZWIAZANIE-DLA-MASZYNY-DRUGIEJ

//---------PRZESUNIECIE-WSKAZNIKA-NA-POZYCJE-KTOREMU-ODPOWIADA-WCZESNIEJ-WYGENEROWANY-NUMER-ZADANIA
            for(int k = 0 ; k < taskNumber ; k++) {
                machine2Task = machine2Task->next;
            }

//---------ZNALEZIENIE-NAJBLIZSZEGO-MOZLIWEGO-CZASU-KIEDY-ZADANIE-MOZE-ROZPOCZAC-WYKONYWANIE
            while(sequenceM2) {
                task_start_timeM2 = sequenceM2->start_time + sequenceM2->time_length;
                sequenceM2 = sequenceM2->next;
            }

//----------SPRAWDZENIE-CZY-ZADANIE-NA-MASZYNIE-2-NIE-ZACZYNA-SIE-SZYBCIEJ-NIZ-KONCZY-SIE-ZADANIE-NA-MASZYNIE-1
            if(task_start_timeM2 <= task_start_timeM1 + machine1Task->time_length) {
                task_start_timeM2 = task_start_timeM1 + machine1Task->time_length;
            }

            Task *pauseM2;                      //---WSKAZNIK-NA-LISTE-PRZERW-DLA-MASZYNY-2
            checkAgain = true;

//----------ZNAJDOWANIE-NOWEGO-MOZLIWEGO-CZASU-ROZPOCZECIA-WYKONYWANIA-ZADANIA-W-ZALEZNOSCI-OD-PRZERW
            while(checkAgain){
                pauseM2 = machine2_pauses;
                while(pauseM2) {
                    for(int k = task_start_timeM2 ; k <= task_start_timeM2 + machine2Task->time_length ; k++) {
                        if(k > pauseM2->start_time && k < pauseM2->start_time + pauseM2->time_length) {
                            task_start_timeM2 = pauseM2->start_time + pauseM2->time_length;
                            checkAgain = true;
                            break;
                        }
                    }
                    checkAgain = false;
                    pauseM2 = pauseM2->next;
                }
            }

            solution->machine_2_sequence->add(task_start_timeM2, machine2Task->time_length, machine2Task->real_number, false, solution->machine_2_sequence);
        }
    }
    return numberOfTasks;
}


/**
    Funkcja wczytujπca instantcje problem z pliku tekstowego.
    A nastÍpnie dodajπca je do globalnej listy zadaÒ.

    @param fileName nazwa pliku, z ktÛrego majπ zostaÊ wczytane dane.

    @return liczba zadaÒ dla danej instancji.
*/
int load_instance(char fileName[20]) {
    char buf[100];                  //---ZMIENNA-POMOCNICZA-PRZECHOWUJACA-DANE-NA-TEMAT-ZADAN-I-PRZERW
    int numberOfTasks;              //---ILOSC-ZDAN-KTORE-WYSTEPUJA-W-DANEJ-INSTANCJI-PROBLEMU
    fstream plik;

    plik.open(fileName, ios::in);

    if(plik.good()) {
        plik.getline(buf, 6);
        numberOfTasks = atoi(buf);  //---WCZYTANIE-ILOSCI-ZADAN-(PIERWSZY-WIERSZ-PLIKU)

//------WCZYTANIE-I-DODANIE-WSZYSTKICH-ZADAN-DO-LISTY-ZADAN
        for(int i = 0 ; i < numberOfTasks ; i++) {
            int machine1_task, machine2_task;

            plik.getline(buf, 10, ';');
            machine1_task = atoi(buf);
            plik.getline(buf, 10);
            machine2_task = atoi(buf);

            machine1_operations->add(0, machine1_task, i, false, machine1_operations);
            machine2_operations->add(0, machine2_task, i, false, machine2_operations);
        }

//------WCZYTANIE-I-DODANIE-PRZERW-DO-LISTY-PRZERW
        while(!plik.eof()) {
            int pauseNumber, machineNumber, timeForPause, whenPauseStart;

            plik.getline(buf, 10, ';');
            pauseNumber = atoi(buf);
            plik.getline(buf, 10, ';');
            machineNumber = atoi(buf);
            plik.getline(buf, 10, ';');
            timeForPause = atoi(buf);
            plik.getline(buf, 10);
            whenPauseStart = atoi(buf);

            if(timeForPause == 0 && machineNumber == 0 && whenPauseStart == 0) break;

            if(machineNumber == 0) machine1_pauses->add(whenPauseStart, timeForPause, pauseNumber, true, machine1_pauses);
            if(machineNumber == 1) machine2_pauses->add(whenPauseStart, timeForPause, pauseNumber, true, machine2_pauses);
        }

    } else cout << "Ups...Something went wrong.";

    plik.close();

    return numberOfTasks;
}


/**
    Funkcja zapisujπca najlepszy wynik powsta≥y z przeprowadzenia algorytmu
    genetycznego do pliku tekstowego.

    @param population wskaünik na populacjÍ, z ktÛrej zostanie wybrane najlepsze rozwiπzanie.
    @param fileName nazwa pliku do jakiej zostanie zapisane rozwiπzanie.
*/
void save_results(Population *population, char fileName[20]) {
    char new_fileName[30] ="solution/";
    strncat(new_fileName, fileName, 20);
    calculate_fitness(population);

    Solution *best_solution = population->solution;
    Solution *solution = population->solution;

    while(solution) {
        if(solution->fitness_value > best_solution->fitness_value) {
            best_solution = solution;
        }
        solution = solution->next;
    }

    Task *machine1_seq = best_solution->machine_1_sequence;
    Task *machine2_seq = best_solution->machine_2_sequence;
    int fitness_value = 0;
    int maximum_time;

    while(machine1_seq) {
        fitness_value += machine1_seq->start_time + machine1_seq->time_length;
        machine1_seq = machine1_seq->next;
    }

    while(machine2_seq) {
        fitness_value += machine2_seq->start_time + machine2_seq->time_length;
        maximum_time = machine2_seq->start_time + machine2_seq->time_length;
        machine2_seq = machine2_seq->next;
    }

    machine1_seq = best_solution->machine_1_sequence;
    machine2_seq = best_solution->machine_2_sequence;

    fstream plik;
    plik.open(new_fileName, ios::out);

    plik << maximum_time << ";" << fitness_value << endl;

//-----------WPISYWANIE-USZEREGOWANIA-NA-MASZYNIE-1
    int actuallTimeMoment = 0;
    int pause_number_M1 = 0;
    int idle_pause_number_M1 = 0;
    int sum_pause_M1 = 0;
    int sum_idle_pause_M1 = 0;

    plik << "M1:";
    while(machine1_seq) {
        bool idle = true;

        if(machine1_seq->start_time == actuallTimeMoment) {
            plik << "o1_z" << machine1_seq->real_number << "," << machine1_seq->start_time << "," << machine1_seq->time_length << ";";
            actuallTimeMoment = machine1_seq->start_time + machine1_seq->time_length;
            machine1_seq = machine1_seq->next;
            idle = false;
        }

        Task *machine1_p = machine1_pauses;
        while(machine1_p) {
            if(machine1_p->start_time == actuallTimeMoment) {
                plik << "maint" << pause_number_M1 << "_M1," << machine1_p->start_time << "," << machine1_p->time_length << ";";
                pause_number_M1++;
                sum_pause_M1 += machine1_p->time_length;
                actuallTimeMoment = machine1_p->start_time + machine1_p->time_length;
                idle = false;
            }
            machine1_p = machine1_p->next;
        }
        if(idle) {
            machine1_p = machine1_pauses;
            int closest_Pause = MAXINTATOM;
            int closest_Task = MAXINTATOM;

            Task *temp_machine1_seq = best_solution->machine_1_sequence;
            while(temp_machine1_seq) {
                if(temp_machine1_seq->start_time >= actuallTimeMoment) {
                    closest_Task = temp_machine1_seq->start_time;
                    break;
                }
                temp_machine1_seq = temp_machine1_seq->next;
            }

            while(machine1_p) {
                if(machine1_p->start_time < closest_Pause && machine1_p->start_time >= actuallTimeMoment) closest_Pause = machine1_p->start_time;
                machine1_p = machine1_p->next;
            }

            if(closest_Task < closest_Pause) {
                plik << "idle" << idle_pause_number_M1 << "_M1," << actuallTimeMoment << "," << closest_Task - actuallTimeMoment << ";";
                idle_pause_number_M1++;
                sum_idle_pause_M1 += closest_Task - actuallTimeMoment;
                actuallTimeMoment = closest_Task;
            } else {
                plik << "idle" << idle_pause_number_M1 << "_M1," << actuallTimeMoment << "," << closest_Pause - actuallTimeMoment << ";";
                idle_pause_number_M1++;
                sum_idle_pause_M1 += closest_Pause - actuallTimeMoment;
                actuallTimeMoment = closest_Pause;
            }
        }
    }
    plik << endl;


//-----------WPISYWANIE-USZEREGOWANIA-NA-MASZYNIE-2
    actuallTimeMoment = 0;
    int pause_number_M2 = 0;
    int idle_pause_number_M2 = 0;
    int sum_pause_M2 = 0;
    int sum_idle_pause_M2 = 0;

    plik << "M2:";
    while(machine2_seq) {
        bool idle = true;

        if(machine2_seq->start_time == actuallTimeMoment) {
            plik << "o2_z" << machine2_seq->real_number << "," << machine2_seq->start_time << "," << machine2_seq->time_length << ";";
            actuallTimeMoment = machine2_seq->start_time + machine2_seq->time_length;
            machine2_seq = machine2_seq->next;
            idle = false;
        }

        Task *machine2_p = machine2_pauses;

        while(machine2_p) {
            if(machine2_p->start_time == actuallTimeMoment) {
                plik << "maint" << pause_number_M2 << "_M2," << machine2_p->start_time << "," << machine2_p->time_length << ";";
                pause_number_M2++;
                sum_pause_M2 += machine2_p->time_length;
                actuallTimeMoment = machine2_p->start_time + machine2_p->time_length;
                idle = false;
            }
            machine2_p = machine2_p->next;
        }
        if(idle) {
            machine2_p = machine2_pauses;
            int closest_Pause = MAXINTATOM;
            int closest_Task = MAXINTATOM;

            Task *temp_machine2_seq = best_solution->machine_2_sequence;
            while(temp_machine2_seq) {
                if(temp_machine2_seq->start_time >= actuallTimeMoment) {
                    closest_Task = temp_machine2_seq->start_time;
                    break;
                }
                temp_machine2_seq = temp_machine2_seq->next;
            }

            while(machine2_p) {
                if(machine2_p->start_time < closest_Pause && machine2_p->start_time >= actuallTimeMoment) closest_Pause = machine2_p->start_time;
                machine2_p = machine2_p->next;
            }

            if(closest_Task < closest_Pause) {
                plik << "idle" << idle_pause_number_M2 << "_M2," << actuallTimeMoment << "," << closest_Task - actuallTimeMoment << ";";
                idle_pause_number_M2++;
                sum_idle_pause_M2 += closest_Task - actuallTimeMoment;
                actuallTimeMoment = closest_Task;
            } else {
                plik << "idle" << idle_pause_number_M2 << "_M2," << actuallTimeMoment << "," << closest_Pause - actuallTimeMoment << ";";
                idle_pause_number_M2++;
                sum_idle_pause_M2 += closest_Task - actuallTimeMoment;
                actuallTimeMoment = closest_Pause;
            }
        }
    }
    plik << endl;
    plik << pause_number_M1 << "," << sum_pause_M1 << endl;
    plik << pause_number_M2 << "," << sum_pause_M2 << endl;
    plik << idle_pause_number_M1 << "," << sum_idle_pause_M1 << endl;
    plik << idle_pause_number_M2 << "," << sum_idle_pause_M2;
    plik.close();

    cout << "\n\nZakonczono prace algorytmu!\n";
}

void clearMemory(Population *population) {
    Solution *solution = population->solution;

    while(solution) {
        Solution *next_solution = solution->next;

        Task *m1_sequence = solution->machine_1_sequence;
        while(m1_sequence) {
            Task *next_task = m1_sequence->next;

            delete m1_sequence;
            m1_sequence = next_task;
        }

        Task *m2_sequence = solution->machine_2_sequence;
        while(m2_sequence) {
            Task *next_task = m2_sequence->next;

            delete m2_sequence;
            m2_sequence = next_task;
        }


        delete solution;
        solution = next_solution;
    }
}


/**
    Funkcja zwracajπca dany moment czasu.

    @return dany moment czasu.
*/
double getTime() {
  long long f, t;
  QueryPerformanceFrequency( (PLARGE_INTEGER) & f );
  QueryPerformanceCounter( (PLARGE_INTEGER) & t);
  return (double)t / (double)f;
}


/**
    G≥Ûwna funkcja programu.
    @author Patryk GliszczyÒski
    @author Krzysztof Prajs
*/
int main() {

    #define START_POPULATION_SIZE  100                  //---ROZMIAR-POPULACJI-STARTOWEJ
    #define MAIN_POPULATION_SIZE  50                    //---ROZMIAR-POPULACJI-DO-KTOREGO-ROZMIARU-OPERATOR-SELEKCJI-BEDZIE-ZMNIEJSZAC (nie powinien byÊ wiÍkszy od populacji startowej)
    #define CROSSOVER_PROPABILITY 70                    //---PRAWDOPODOBIENSTWO-WYSTAPIENIA-KRZYZOWANIA
    #define MUTATION_PROPABILITY 30                     //---PRAWDOPODOBIENSTWO-WYSTAPIENIA-MUTACJI
    #define TIME_STOP_VALUE 1200                        //---WARUNEK-STOPU-PODANY-W-SEKUNDACH

    srand(time(NULL));
    char fileName[30] = "100RANDOM.txt";

    Population *population = new Population;            //---WSKAZNIK-NA-POPULACJE-STARTOWA


    int numberOfTasks = generate_population(load_instance(fileName), START_POPULATION_SIZE, population);            //---GENEROWANIE-STARTOWEJ-POPULACJI
    double startTime = getTime();
    double currentTime;
    int generationNumber = 1;
//------PRZEZ-OKRESLONY-CZAS-WYKONYWANE-SA-OPERACJE-MUTACJI-KRZYZOWANIA-I-SELEKCJI
    do {
        Population *new_population = new Population;                            //---UTWORZENIE-NOWEGO-MIEJSCA-W-PAMIECI-DLA-NOWEJ-POPULACJI
        selection(population, new_population, MAIN_POPULATION_SIZE);            //---SELEKCJA-STAREJ-POPULACJI-W-NOWA-POPULACJE
        clearMemory(population);
        population = new Population;

//----------WYKONWANIE-OPERACJI-MUTACJI-I-KRZYZOWANIA-DLA-KAZDEGO-ZADANIA
        Solution *solution = new_population->solution;

        while(solution) {

            int crossoverPropability = (rand() % 100) + 1;
            if(crossoverPropability <= CROSSOVER_PROPABILITY) crossover(solution, new_population, numberOfTasks, population);

            int mutationPropability = (rand() % 100) + 1;
            if(mutationPropability <= MUTATION_PROPABILITY) mutate(solution, numberOfTasks, population);

            solution = solution->next;
        }
        cout << "current generation: " << generationNumber++ << endl;

        clearMemory(new_population);
        currentTime = getTime();
    } while(currentTime - startTime < TIME_STOP_VALUE);

    save_results(population, fileName);         //---ZAPISANIE-WYNIKU-PRZEPROWADZENIA-ALGORYTMU-DO-PLIKU-TEKSTOWEGO

    return 0;
}
