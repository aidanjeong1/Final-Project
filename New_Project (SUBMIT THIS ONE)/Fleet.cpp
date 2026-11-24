// Fleet.cpp
// ESE 224 Fall 2026 Final Project (Part II) starter code
// Team members: TODO
//
// The fleet is a singly linked list kept in ascending ID order (Section 5) plus the dispatch center (Section 7).
// No vector or other STL containers: use your own list, Queue, Stack, and DroneTree.

#include "Fleet.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
using namespace std;

Fleet::Fleet() : head(nullptr), count(0) {}

Fleet::~Fleet()
{
    // TODO: delete every node AND every unit (the Fleet owns the units)
}

// ---------- Linked list ----------
bool Fleet::addUnit(DeliveryUnit* u)
{
    // TODO: sorted insert by ID and insert into 'index'; on a duplicate ID print a message, delete u, return false
    const int id = u->getID();
    UnitNode* prev = nullptr;
    UnitNode* walk = head;
    while(walk != nullptr && walk->unit->getID() < id){
        prev = walk;
        walk = walk->next;
    }
    if(findQuiet(u->getID())){
        cout << "Unit ID " << id << " already in list." << endl;
        delete u;
        return false;
    }
    UnitNode* newNode = new UnitNode{u, walk};
    if(prev == nullptr){
        head = newNode;
    }
    else{
        prev->next = newNode;
    }
    index.insert(u);
    count++;
    return true;
}

bool Fleet::removeUnit(int id)
{
    UnitNode* prev = nullptr;
    UnitNode* walk = head;

    // Stop at the first ID that could match.
    while (walk != nullptr && walk->unit->getID() < id) {
        prev = walk;
        walk = walk->next;
    }
    if (walk == nullptr || walk->unit->getID() != id) {
        cout << "ID " << id << " not found." << endl;
        return false;
    }
    if (prev == nullptr) {
        head = walk->next;
    } else {
        prev->next = walk->next;
    }
    index.remove(id);
    delete walk->unit;
    delete walk;
    count--;
    return true;
}

DeliveryUnit* Fleet::findQuiet(int id) const
{
    UnitNode* walk = head;
    while (walk != nullptr && walk->unit->getID() <= id) {
        if (walk->unit->getID() == id) {
            return walk->unit;
        }
        walk = walk->next;
    }
    return nullptr;
}

DeliveryUnit* Fleet::findByID(int id) const
{
    // TODO: one comparison per node examined; stop at the first larger ID; print the count
    int count(0);
    UnitNode* walk = head;
    while (walk != nullptr && walk->unit->getID() <= id) {
        if (walk->unit->getID() == id) {
            return walk->unit;
        }
        count++;
        walk = walk->next;
    }
    cout << "Count: " << count << endl;
    return nullptr;
}

DeliveryUnit* Fleet::searchTree(int id) const
{
    // TODO: index.search(...) and print the count
    int comparisons(0);
    if(index.search(id, comparisons)){
        cout << "Comparisons: " << comparisons << endl;
        return index.search(id, comparisons);
    }
    cout << "Search failed" << endl;
    return nullptr;
}

int Fleet::getSize() const { return count; }

void Fleet::displayAll() const
{
    for (const UnitNode* walk = head; walk != nullptr; walk = walk->next) {
        walk->unit->display();
    }
}

void Fleet::displayTree() const
{
    // TODO: in-order, pre-order, post-order, and height
    cout << "In-order: ";
    index.inOrder();

    cout << "Pre-order: ";
    index.preOrder();

    cout << "Post-order: ";
    index.postOrder();

    cout << "Height: " << index.height() << '\n';
}

void Fleet::countByStatus() const {
    int regular = 0;
    int heavy = 0;
    int idle = 0;
    int delivering = 0;
    int charging = 0;
    int maintenance = 0;
    double totalBattery = 0.0;

    for (const UnitNode* walk = head; walk != nullptr; walk = walk->next) {
        const DeliveryUnit* unit = walk->unit;

        if (unit->type() == "Heavy") {
            heavy++;
        } else {
            regular++;
        }

        const string status = unit->getStatus();
        if (status == "IDLE") {
            idle++;
        } else if (status == "DELIVERING") {
            delivering++;
        } else if (status == "CHARGING") {
            charging++;
        } else if (status == "MAINTENANCE") {
            maintenance++;
        }

        totalBattery += unit->getBattery();
    }

    const double averageBattery = (count == 0) ? 0.0 : totalBattery / count;

    cout << "Regular drones: " << regular << '\n'
         << "Heavy drones: " << heavy << '\n'
         << "IDLE: " << idle << '\n'
         << "DELIVERING: " << delivering << '\n'
         << "CHARGING: " << charging << '\n'
         << "MAINTENANCE: " << maintenance << '\n'
         << "Average battery: " << averageBattery << "%\n"
         << "Pending orders: " << pending.size() << '\n'
         << "Retry orders: " << retry.size() << '\n'
         << "Dispatch history: " << history.size() << '\n';
}

void Fleet::writeFleetToFile(const string& filename) const {
    ofstream out(filename);
    if (!out) {
        cout << "Could not open " << filename << " for writing.\n";
        return;
    }

    out << fixed << setprecision(1);

    for (const UnitNode* walk = head; walk != nullptr; walk = walk->next) {
        const DeliveryUnit* unit = walk->unit;
        const char typeCode = (unit->type() == "Heavy") ? 'H' : 'D';

        out << typeCode << ' '
            << unit->getName() << ' '
            << unit->getID() << ' '
            << unit->getModel() << ' '
            << unit->getBattery() << ' '
            << unit->getMaxPayload() << ' '
            << unit->getPosition(0) << ' '
            << unit->getPosition(1) << ' '
            << unit->getStatus() << ' '
            << unit->getDeliveriesCompleted() << '\n';
    }
}

// ---------- Dispatch center ----------
DeliveryUnit* Fleet::findNearestAvailable(int x, int y, double weight) const
{
    // TODO: closest unit that canDeliver; a tie goes to the smaller ID
    DeliveryUnit* nearest = nullptr;
    double nearestDistance = 0.0;

    for (const UnitNode* walk = head; walk != nullptr; walk = walk->next) {
        DeliveryUnit* unit = walk->unit;

        if (!unit->canDeliver(x, y, weight)) {
            continue;
        }

        const double distance = unit->distanceTo(x, y);
        if (nearest == nullptr || distance < nearestDistance) {
            nearest = unit;
            nearestDistance = distance;
        }
    }

    return nearest;
}

void Fleet::appendLog(const string& logFile, const string& line) const
{
    // Prints a line and appends it to the log file
    cout << line << endl;
    ofstream out(logFile, ios::app);
    out << line << endl;
}

int Fleet::loadOrders(const string& orderFile, const string& logFile)
{
    // TODO: enqueue every order into 'pending' in file order; start a new, empty log file
    ifstream in(orderFile);
    if (!in) {
        cout << "Could not open " << orderFile << '\n';
        return 0;
    }

    ofstream log(logFile, ios::trunc);
    if (!log) {
        cout << "Could not open " << logFile << endl;
        return 0;
    }

    int loaded = 0;
    Order order{};

    while (in >> order.orderID >> order.x >> order.y >> order.weight) {
        pending.enqueue(order);
        loaded++;
    }

    return loaded;
}

void Fleet::processPending(const string& logFile)
{
    // TODO: dequeue until empty; dispatch (push a DispatchRecord first) or move to 'retry'; log each line and a summary
    int assigned = 0;
    int failed = 0;
    double totalDistance = 0.0;
    Order order{};

    while (pending.dequeue(order)) {
        DeliveryUnit* unit = findNearestAvailable(order.x, order.y, order.weight);
        ostringstream line;
        line << "Order " << order.orderID << ": ";

        if (unit == nullptr) {
            retry.enqueue(order);
            failed++;
            line << "no unit available, moved to retry queue";
        } 
        else {
            const double distance = unit->distanceTo(order.x, order.y);
            const DispatchRecord record{
                order,
                unit->getID(),
                unit->getBattery(),
                unit->getPosition(0),
                unit->getPosition(1),
                unit->getStatus()
            };

            history.push(record);
            unit->completeDelivery(order.x, order.y, order.weight);
            assigned++;
            totalDistance += distance;

            line << unit->type() << ' ' << unit->getID()
                 << " (" << unit->getName() << ')'
                 << fixed << setprecision(1)
                 << " distance " << distance
                 << " battery " << record.oldBattery
                 << " -> " << unit->getBattery();
        }

        appendLog(logFile, line.str());
    }

    ostringstream summary;
    summary << "Processed " << assigned + failed
            << " orders: " << assigned << " assigned, "
            << failed << " moved to retry queue. Distance flown: "
            << fixed << setprecision(1) << totalDistance;

    appendLog(logFile, summary.str());
}

bool Fleet::undoLastDispatch(const string& logFile)
{
    // TODO: pop the last record, undoDelivery on that unit, move the order to 'retry'
    DispatchRecord record{};
    if (!history.pop(record)) {
        cout << "Nothing to undo." << endl;
        return false;
    }

    DeliveryUnit* unit = findQuiet(record.unitID);
    ostringstream line;
    line << "Undo order " << record.order.orderID << ": ";

    if (unit == nullptr) {
        line << "unit " << record.unitID << " was removed";
    } else {
        unit->undoDelivery(record.oldBattery, record.oldX,
                           record.oldY, record.oldStatus);

        line << unit->type() << ' ' << unit->getID()
             << " (" << unit->getName() << ") restored to battery "
             << fixed << setprecision(1) << record.oldBattery
             << " at (" << record.oldX << ", " << record.oldY << ") "
             << record.oldStatus;
    }

    retry.enqueue(record.order);
    line << ", order moved to retry queue";
    appendLog(logFile, line.str());
    return true;
}

int Fleet::rechargeAll(const string& logFile)
{
    // TODO: every CHARGING unit -> battery 100, IDLE
    int recharged = 0;

    for (UnitNode* walk = head; walk != nullptr; walk = walk->next) {
        DeliveryUnit* unit = walk->unit;
        if (unit->getStatus() == "CHARGING") {
            unit->setBattery(100.0);
            unit->setStatus("IDLE");
            recharged++;
        }
    }

    appendLog(logFile, "Recharged " + to_string(recharged) + " units to 100%.");
    return recharged;

}

void Fleet::retryFailed(const string& logFile)
{
    // TODO: move every order from 'retry' to 'pending' (keep the order), then processPending
    const int retrying = retry.size();
    Order order{};

    while (retry.dequeue(order)) {
        pending.enqueue(order);
    }

    appendLog(logFile, "Retrying " + to_string(retrying) + " orders.");
    processPending(logFile);
}

int Fleet::pendingCount() const { return pending.size(); }
int Fleet::retryCount() const { return retry.size(); }
int Fleet::historyCount() const { return history.size(); }