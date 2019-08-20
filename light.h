#ifndef light_H
#define light_H
#include "trafficItem.h"
#include <string>
#include <vector>

using namespace std;

class light : public trafficItem
{
    public:
        light(string startingState, int minStateDuration, int maxStateDuration) : trafficItem(startingState, minStateDuration, maxStateDuration) {}
        
        void TurnGreen()
        {
            this->SetState("green");
        }
        
        void TurnYellow()
        {
            this->SetState("yellow");
        }
        
        void TurnRed()
        {
            this->SetState("red");
        }
};

#endif