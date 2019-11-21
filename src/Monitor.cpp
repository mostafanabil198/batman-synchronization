#include "Monitor.h"

Monitor::Monitor()
{
    for(int i = 0; i < 4; i++){
        dirQueue[i] = PTHREAD_COND_INITIALIZER;
        dirFirst[i] = PTHREAD_COND_INITIALIZER;
        waitingRight[i] = false;
        dirQueueCounter[i] = 0;
    }
    mutex = PTHREAD_MUTEX_INITIALIZER;
    crossMutex = PTHREAD_MUTEX_INITIALIZER;
}

void Monitor::arrive(Bat b) {
    pthread_mutex_lock(&mutex);
    cout << "BAT " << b.id << " from " << b.dirStr() << " arrives at crossing " << endl;
    dirQueueCounter[b.dir]++;
    // check if there's a thread already in this direction.
    if(dirQueueCounter[b.dir] > 1) {
        cout << "BAT " << b.id << " from " << b.dirStr() << " waiting in the direction queue " << endl;
        pthread_cond_wait(&dirQueue[b.dir], &mutex);
    }
    pthread_mutex_unlock(&mutex);
}
void Monitor::cross(Bat b) {
    pthread_mutex_lock(&mutex);
    // check if there's a thread in the right direction as it
    // has a priority to run first.
    if(dirQueueCounter[b.right()] > 0) {
        cout << "BAT " << b.id << " from " << b.dirStr() << " in cross - give up to right" << endl;
        // this direction will be waiting for the right one to finish.
        waitingRight[b.dir] = true;
        // wait on dirFirst of the right direction of it.
        pthread_cond_wait(&dirFirst[b.right()], &mutex);
        waitingRight[b.dir] = false;
        cout << "BAT " << b.id << " from " << b.dirStr() << " in cross - return from right" << endl;
    }
    pthread_mutex_lock(&crossMutex);
    cout << "BAT " << b.id << " from " << b.dirStr() << " is crossing" << endl;
    pthread_mutex_unlock(&mutex);
    sleep(1);
    pthread_mutex_unlock(&crossMutex);
}
void Monitor::leave(Bat b) {
    pthread_mutex_lock(&mutex);
    cout << "BAT " << b.id << " from " << b.dirStr() << " is leaving" << endl;
    dirQueueCounter[b.dir]--;
    // if there's any thread waiting for this dir to signal, then signal that it finished.
    if(waitingRight[b.left()]) {
        pthread_cond_signal(&dirFirst[b.dir]);
    }
    pthread_cond_signal(&dirQueue[b.dir]);
    pthread_mutex_unlock(&mutex);
}
void Monitor::check() {
    pthread_mutex_lock(&mutex);
    // if all directions waiting for the right then we have a deadlock.
    // so we signal one *the one in the north* to force it move to break the deadlock.
    if(waitingRight[0] && waitingRight[1] && waitingRight[2] && waitingRight[3]) {
        cout << "DEADLOCK: BAT jam detected, signalling North to go" << endl;
        pthread_cond_signal(&dirFirst[0]);
    }
    pthread_mutex_unlock(&mutex);
}

// destroy all the conditions and the mutex
void Monitor::destroy() {
    for(int i = 0; i < 4; i++){
        pthread_cond_destroy(&dirQueue[i]);
        pthread_cond_destroy(&dirFirst[i]);
    }
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&crossMutex);
}
