#ifndef BAT_H
#define BAT_H
#include <iostream>
#include <bits/stdc++.h>
using namespace std;



class Bat
{
    public:
        Bat();
        int id;
        int dir;
        int left();
        int right();
        string dirStr();

    protected:

    private:
};

#endif // BAT_H
