/*
    12/7/2018
    Written by Daniel Keele
*/

#include <chrono>
#include <iostream>
#include <sstream>
#include <map>
#include "light.h"
#include <unistd.h>

using namespace std;

//Configuration
//run the simulation instantly, or in real time
bool runInstantly = false;

//other configurable parameters
int desiredSimulationTimeInSeconds = 120;
int eastWestMinimumGreenDuration = 30;
int northSouthMaximumGreenDuration = 40;
int sensorTriggerTime = 10;
int sensorMinimumGapTime = 5;
int redToGreenTime = 2;
int yellowToRedTime = 3;

//event system
//the key is the second at which the event occurs
//the value is the event to trigger
//event options are "activateSensor" and "deactivateSensor"
map<int, string> events =
{
    {10, "activateSensor"},
    {31, "deactivateSensor"},
    {70, "activateSensor"},
};

//this function takes the number of lapsed seconds
//and returns the formatted time along with any event at that time
string TimeToString(int secondCount, string* event)
{
    stringstream ss;
    int minutes = secondCount / 60;
    int seconds = secondCount - (minutes * 60);
    
    ss << minutes << ":" << seconds << "------- ";
    
    if (*event != "none")
    {
        ss << *event;
    }
    
    ss << "\n\n";
    
    return ss.str();
}

//the method to be called each second. Enforces rules as described in prompt.
string UpdateController(light* EWlight, light* NSlight, trafficItem* sensor, string* event)
{
    //handle lights
    //red
    if (EWlight->GetState() == "red" && EWlight->GetStateDuration() == redToGreenTime && NSlight->GetState() != "green")
    {
        NSlight->TurnGreen();
    }
    
    if (NSlight->GetState() == "red" && NSlight->GetStateDuration() == redToGreenTime && EWlight->GetState() != "green")
    {
        EWlight->TurnGreen();
    }
    
    //yellow
    if (EWlight->GetState() == "yellow" && EWlight->GetStateDuration() == yellowToRedTime)
    {
        EWlight->TurnRed();
    }
    
    if (NSlight->GetState() == "yellow" && NSlight->GetStateDuration() == yellowToRedTime)
    {
        NSlight->TurnRed();
    }
    
    //handle events
    //sensor activated
    if (*event == "activateSensor" && sensor->GetState() != "active")
    {
        sensor->SetState("active");
    }
    
    //sensor deactivated
    if (*event == "deactivateSensor" && sensor->GetState() != "inactive")
    {
        sensor->SetState("inactive");
    }
    
    //other rules
    //sensor activated for 10+ seconds
    if (sensor->GetState() == "active" && sensor->GetStateDuration() >= sensor->GetMinStateDuration() && EWlight->GetState() == "green" && EWlight->GetStateDuration() >= EWlight->GetMinStateDuration())
    {
        EWlight->TurnYellow();
    }
    
    //sensor deactivated 5+ seconds
    if (sensor->GetState() == "inactive" && sensor->GetStateDuration() >= sensor->GetMaxStateDuration() && EWlight->GetState() == "red" && NSlight->GetState() == "green")
    {
        NSlight->TurnYellow();
    }
    
    //NSlight green for more than 40 seconds
    if (NSlight->GetState() == "green" && NSlight->GetStateDuration() == NSlight->GetMaxStateDuration())
    {
        NSlight->TurnYellow();
    }
    
    //increase timer
    EWlight->IncrementStateDuration();
    NSlight->IncrementStateDuration();
    sensor->IncrementStateDuration();
    
    //return status
    stringstream ss;
    
    ss << "E/W light: " << EWlight->GetState() << "\n";
    ss << "N/S light: " << NSlight->GetState() << "\n";
    ss << "Sensor: " << sensor->GetState() << "\n\n";
    
    return ss.str();
}

int main() 
{
    //variable declaration
    int secondCount = 0;
    light* EWlight = new light("green", eastWestMinimumGreenDuration, 0);
    light* NSlight = new light("red", 0, northSouthMaximumGreenDuration);
    trafficItem* sensor = new trafficItem("inactive", sensorTriggerTime, sensorMinimumGapTime);
    string* event = new string("none");
    
    //this loop simulates a one second time span
    while (secondCount < desiredSimulationTimeInSeconds)
    {
        //responds to real time or instantly as set in the configuration variables
        runInstantly ? : sleep(1);
        
        //checks the event system for any events this second
        for (map<int, string>::iterator it = events.begin(); it != events.end() && *event == "none"; it++)
        {
            if (secondCount == it->first)
            {
                event->assign(it->second);
            }
        }
        
        //print time
        cout << TimeToString(secondCount, event);
        
        //print status
        cout << UpdateController(EWlight, NSlight, sensor, event);
        
        //reset event
        event->assign("none");
        
        //move on
        secondCount++;
    };
    
    cout << "\n\n\nSimulation Complete." << endl;
}