#include "Bat.h"

Bat::Bat()
{
}

int Bat::left() {
    return ((dir - 1) % 4) == -1 ? 3 : ((dir - 1) % 4);
}

int Bat::right() {
    return ((dir + 1) % 4);
}

string Bat::dirStr() {
    switch(dir){
        case 0: return "North"; break;
        case 1: return "West"; break;
        case 2: return "South"; break;
        case 3: return "East"; break;
        default: return "error"; break;
    }
}

