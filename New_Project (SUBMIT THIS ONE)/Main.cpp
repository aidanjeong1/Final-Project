// main.cpp
// ESE 224 Fall 2026 Final Project (Part II) starter code
// Team members: TODO
//
// This skeleton compiles and shows the menu. Fill in each TODO (Section 8).
// You can reuse your Part I login and input-checking code.

#include "DeliveryUnit.h"
#include "Drone.h"
#include "HeavyDrone.h"
#include "Fleet.h"
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
using namespace std;

const string LOG_FILE = "delivery_log.txt";

// TODO: ask for a username and password until the pair is in users.txt; greet the user
bool login(){
    ifstream fin("users.txt");
    if(!(fin)){
        cout << "Unable to open file" << endl;
        return false;
    }
    string inputUser, fileUser;
    string inputPassword, filePassword;
    while(true){
        cout << "Input Username: ";
        if(!(cin >> inputUser)){
            return false;
        }
        cout << "Input Password: ";
        if(!(cin >> inputPassword)){
            return false;
        }
        fin.clear();
        fin.seekg(0);
        while(fin >> fileUser >> filePassword){
            if (inputUser == fileUser && inputPassword == filePassword) {
                cout << "Welcome, " << inputUser << "!" << endl;
                return true;
            }
        }

        cout << "Invalid, Try Again." << endl;
    }
}

// TODO: read every line of drones.txt until end of file. The first word is D (Drone) or H (HeavyDrone):
//       create the unit with new, add it to the fleet, and print how many were loaded.
void loadFleet(Fleet& fleet, const string& filename)
{
    ifstream fin(filename);
    if (!fin) {
        cout << "Unable to open file" << endl;
        return;
    }
    int loaded(0);
    char typeCode;
    string name;
    int ID;
    string model;
    double battery;
    double maxPayload;
    int x;
    int y;
    string status;
    int deliveriesCompleted;
    while (fin >> typeCode >> name >> ID >> model >> battery >> maxPayload >> x >> y >> status) {
        DeliveryUnit* newDrone = nullptr;
        if(typeCode == 'H'){
            newDrone = new HeavyDrone(name, ID, model, battery, maxPayload, x, y, status);
        }
        else if(typeCode == 'D'){
            newDrone = new Drone(name, ID, model, battery, maxPayload, x, y, status);
        }
        else {
            cout << "Unknown unit type: " << typeCode << '\n';
            continue;
        }
        if(fleet.addUnit(newDrone)){
            loaded++;
        };
    }
    cout << "Loaded " << fleet.getSize() << " drones." << endl;
}

void showMenu()
{
    cout << endl
         << "===== SkyDrop Dispatch Center =====" << endl
         << " 1. Display fleet (ID order)" << endl
         << " 2. Display ID tree (traversals and height)" << endl
         << " 3. Search by ID (linked list and tree)" << endl
         << " 4. Add a unit" << endl
         << " 5. Remove a unit" << endl
         << " 6. Find nearest available unit" << endl
         << " 7. Load orders into the pending queue" << endl
         << " 8. Process pending orders" << endl
         << " 9. Undo last dispatch" << endl
         << "10. Recharge all charging units" << endl
         << "11. Retry failed orders" << endl
         << "12. Fleet status summary" << endl
         << "13. Write fleet to file" << endl
         << " 0. Quit" << endl;
}

int main()
{
    if (!login()){
        return 0;
    }
    Fleet fleet;
    loadFleet(fleet, "drones.txt");

    int choice = -1;
    while (choice != 0) {
        showMenu();
        cout << "Choose an option: ";
        // TODO: recover from a letter (cin.clear and cin.ignore) instead of crashing or looping forever

        if (!(cin >> choice)) {
            if (cin.eof() || cin.bad()) {
                return 0;
            }

            cout << "Invalid input. Try again.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice){
        case 1:
            fleet.displayAll();
            break;

        case 2:
            fleet.displayTree();
            break;

        case 3: {
            int id;
            cout << "Enter unit ID: ";
            if (cin >> id) {
                DeliveryUnit* listResult = fleet.findByID(id);
                fleet.searchTree(id);

                if (listResult != nullptr) {
                    listResult->display();
                } else {
                    cout << "Unit not found.\n";
                }
            }
            break;
        }

        case 4: {
            char typeCode;
            string name, model, status;
            int id, x, y;
            double battery, maxPayload;

            cout << "Enter type (D/H), name, ID, model, battery, "
                    "max payload, x, y, and status:\n";

            if (!(cin >> typeCode >> name >> id >> model >> battery >> maxPayload >> x >> y >> status)) {
                break;
            }

            DeliveryUnit* unit = nullptr;
            if (typeCode == 'D') {
                unit = new Drone(name, id, model, battery, maxPayload, x, y, status);
            } 
            else if (typeCode == 'H') {
                unit = new HeavyDrone(name, id, model, battery, maxPayload, x, y, status);
            }
            else {
                cout << "Invalid unit type. Enter D or H.\n";
                break;
            }

            if (fleet.addUnit(unit)) {
                cout << "Unit added.\n";
            }
            break;
        }

        case 5: {
            int id;
            cout << "Enter unit ID to remove: ";
            if (cin >> id && fleet.removeUnit(id)) {
                cout << "Unit removed.\n";
            }
            break;
        }

        case 6: {
            int x, y;
            double weight;
            cout << "Enter destination x, y, and payload weight: ";

            if (cin >> x >> y >> weight) {
                DeliveryUnit* unit =
                    fleet.findNearestAvailable(x, y, weight);

                if (unit != nullptr) {
                    unit->display();
                }
                else {
                    cout << "No unit available.\n";
                }
            }
            break;
        }

        case 7: {
            int loaded = fleet.loadOrders("orders.txt", LOG_FILE);
            cout << "Loaded " << loaded << " orders.\n";
            break;
        }

        case 8:
            fleet.processPending(LOG_FILE);
            break;

        case 9:
            fleet.undoLastDispatch(LOG_FILE);
            break;

        case 10:
            fleet.rechargeAll(LOG_FILE);
            break;

        case 11:
            fleet.retryFailed(LOG_FILE);
            break;

        case 12:
            fleet.countByStatus();
            break;

        case 13:
            fleet.writeFleetToFile("fleet_output.txt");
            break;

        case 0:
            cout << "Goodbye!\n";
            return 0;

        default:
            cout << "Invalid option.\n";
            break;  
        }
    }
    return 0;   // the Fleet destructor frees everything here
}