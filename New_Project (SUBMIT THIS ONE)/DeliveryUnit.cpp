// DeliveryUnit.cpp
// ESE 224 Fall 2026 Final Project (Part II) starter code
// Team members: TODO
//
// Much of this is your Part I Drone code, moved into the base class (Section 3.2).

#include "DeliveryUnit.h"
#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

DeliveryUnit::DeliveryUnit()
{
    // TODO: the Part I defaults (Done :))
    ID = -1;
    battery = 100.0;
    maxPayload = 0.0;
    position[0] = 0;
    position[1] = 0;
    status = "IDLE";
    deliveriesCompleted = 0;
}

DeliveryUnit::~DeliveryUnit() {}

// ---------- Accessors ----------
string DeliveryUnit::getName() const { return name; }
int DeliveryUnit::getID() const { return ID; }
string DeliveryUnit::getModel() const { return model; }
double DeliveryUnit::getBattery() const { return battery; }
double DeliveryUnit::getMaxPayload() const { return maxPayload; }
string DeliveryUnit::getStatus() const { return status; }
int DeliveryUnit::getDeliveriesCompleted() const { return deliveriesCompleted; }

int DeliveryUnit::getPosition(int index) const
{
    // TODO: as in Part I (Done :))
    if (index == 0 || index == 1) {
        return position[index];
    }
    return -1;
}

// ---------- Mutators (validate as in Part I; setMaxPayload: 0 < p <= MAX_PAYLOAD_LIMIT) ----------
void DeliveryUnit::setName(const string& n) {
    name = n; 
}
bool DeliveryUnit::setID(int id) { 
    if (id > 0)
    {
        ID = id;
        return true;
    }
    cout << "Error: ID must be positive." << endl;
    
    return false; 
}

bool DeliveryUnit::setBattery(double b) {
    if (b >= 0 && b <= 100)
    {
        battery = b;
        return true;
    }
    cout << "Battery outside of range." << endl;
    return false;
    }
bool DeliveryUnit::setMaxPayload(double p) { 
    if (p > 0)
    {
        maxPayload = p;
        return true;
    }
    cout << "Max payload must be greater than 0." << endl;
     
    return false; 
}
bool DeliveryUnit::setPosition(int index, int value) { 
     if (index != 0 && index != 1) {
		cout << "Error: position index must be 0 (x) or 1 (y) (got " << index << ")" << endl;
		return false;
	}
    if (value < 0) {
        cout << "Error: position value must be non-negative (got " << value << ")." << endl;
        return false;
    }
    position[index] = value;
    return true; 
}
bool DeliveryUnit::setStatus(const string& s) { 
     
    if (s == "IDLE" || s == "CHARGING" || s == "MAINTENANCE")
    {
        status = s;
        return true;
    }
    else{
        cout << "Status must be IDLE, CHARGING, or MAINTENANCE" << endl;
        return false;
    }
}

// ---------- Shared behaviour ----------
double DeliveryUnit::distanceTo(int x, int y) const
{
    // TODO (Done :)): distance formula sqrt((x2-x1)^2 + (y2-y1)^2)
    double dx = x - position[0];
    double dy = y - position[1];
    return sqrt(dx * dx + dy * dy);
}

bool DeliveryUnit::canDeliver(int x, int y, double weight) const
{
    // TODO: as in Part I, but call the virtual batteryNeeded so each unit type uses its own formula (Done :))
    if (status != "IDLE" || !(weight > 0.0 && weight <= maxPayload)) {
        return false;
    }

    return battery - batteryNeeded(x, y, weight) >= SAFETY_RESERVE;
}

void DeliveryUnit::completeDelivery(int x, int y, double weight)
{
    // TODO: as in Part I (Done :))
    battery -= batteryNeeded(x, y, weight);
    position[0] = x;
    position[1] = y;
    status = "DELIVERING";
    deliveriesCompleted++;
}

void DeliveryUnit::undoDelivery(double oldBattery, int oldX, int oldY, const string& oldStatus)
{
    // TODO: restore battery, position, and status; deliveriesCompleted goes down by 1 (never below 0) (Done :))
    battery = oldBattery;
    position[0] = oldX;
    position[1] = oldY;
    status = oldStatus;
    deliveriesCompleted = max(0, deliveriesCompleted - 1);
}

void DeliveryUnit::display() const
{
    // TODO: print [type()] and every member with a label (Done :))
    cout << "[" << type() << "] " << name << " (ID: " << ID << ")" << endl;
    cout << "  Model: " << model << endl;
    cout << "  Battery: " << battery << endl;
    cout << "  Max Payload: " << maxPayload << endl;
    cout << "  Position: (" << position[0] << ", " << position[1] << ")" << endl;
    cout << "  Status: " << status << endl;
    cout << "  Deliveries Completed: " << deliveriesCompleted << endl;
}