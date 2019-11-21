#include <iostream>
#include<bits/stdc++.h>
#include <Bat.h>
#include <Monitor.h>

using namespace std;

void *moveProducts(void* bat);
void *checker (void* args);
Monitor monitor;

int main()
{
    cout << "Enter bats order using the directions 'n', 's', 'e', 'w'" << endl;
    string bats;
    cin >> bats;
    // threads for each direction input.
    pthread_t batThreads[bats.length()];
    // thread to check for deadlock.
    pthread_t chekerThread;
    if (pthread_create(&chekerThread, NULL, checker, NULL) > 0) {
        cout << "Failed to create thread" << endl;
    }
    // take each entered direction and create a bat for it
    // then run bat's functions in a thread for each.
    for(int i = 0; i < bats.length(); i++){
        Bat *bat = new Bat();
        switch(bats.at(i)){
            case 'n':
                bat->id = i;
                bat->dir = 0;
                break;
            case 'w':
                bat->id = i;
                bat->dir = 1;
                break;
            case 's':
                bat->id = i;
                bat->dir = 2;
                break;
            case 'e':
                bat->id = i;
                bat->dir = 3;
                break;
            default :
                cout << "ERROR! skip wrong direction." << endl;
                continue;
                break;
        }
        int status = pthread_create(&batThreads[i], NULL, moveProducts, (void*)bat);
         if (status > 0) {
            cout << "Failed to create thread" << endl;
        }
    }
    // wait untill all threads finish before exit the program.
    for (int i = 0; i < bats.length(); i++) {
        pthread_join(batThreads[i], NULL);
    }
    return 0;
}

// thread function to run the three methods of a bat.
void *moveProducts(void* bat) {
    Bat *b = (Bat*) bat;
    Bat b1;
    b1.dir = b->dir;
    b1.id = b->id + 1;
    monitor.arrive(b1);
    monitor.cross(b1);
    monitor.leave(b1);
}

// thread function to check if there's a deadlock.
void *checker (void* args) {
    while (true) {
        monitor.check();
        sleep(1);
    }
}
