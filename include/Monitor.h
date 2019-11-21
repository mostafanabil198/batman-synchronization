#ifndef MONITOR_H
#define MONITOR_H
#include <pthread.h>
#include <Bat.h>
#include <iostream>
#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;

class Monitor
{
    public:
        Monitor();
        pthread_mutex_t mutex;
        pthread_mutex_t crossMutex;
        pthread_cond_t dirQueue[4];
        pthread_cond_t dirFirst[4];
        int dirQueueCounter[4];
        bool waitingRight[4];
        void arrive(Bat b);
        void cross(Bat b);
        void leave(Bat b);
        void check();
        void destroy();

    protected:

    private:
};

#endif // MONITOR_H
