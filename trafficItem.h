#ifndef trafficItem_H
#define trafficItem_H
#include <string>
#include <vector>

using namespace std;

class trafficItem
{
    public:
        trafficItem(string startingState, int minStateDuration, int maxStateDuration)
        {
            this->state = startingState;
            this->minStateDuration = minStateDuration;
            this->maxStateDuration = maxStateDuration;
            this->stateDuration = 0;
        }
        
        string GetState()
        {
            return this->state;
        }
        
        void SetState(string state)
        {
            this->state = state;
            this->stateDuration = 0;
        }
        
        int GetStateDuration()
        {
            return this->stateDuration;
        }
        
        void IncrementStateDuration()
        {
            stateDuration++;
        }
        
        int GetMaxStateDuration()
        {
            return this->maxStateDuration;
        }
        
        int GetMinStateDuration()
        {
            return this->minStateDuration;
        }
    
    private:
        string state;
        int stateDuration;
        int minStateDuration;
        int maxStateDuration;
};

#endif