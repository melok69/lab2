#include <iostream>
#include <vector>
#include <memory>
#include <string>

using namespace std;

class LuggageStorage {
private:
    string storageName;
    int capacity;

public:
    LuggageStorage(const string& name, int capacity)
        : storageName(name), capacity(capacity) {
        if (capacity < 500 || capacity > 2000) {
            throw invalid_argument("Capacity must be between 500 and 2000 kg!");
        }
    }

    void displayInfo() const {
        cout << "Luggage Storage: " << storageName
            << ", Capacity: " << capacity << " kg" << endl;
    }

    string getName() const { return storageName; }
    int getCapacity() const { return capacity; }
};

class TrainStation {
private:
    vector<shared_ptr<LuggageStorage>> storages;

    TrainStation() {}

public:
    TrainStation(const TrainStation&) = delete;
    TrainStation& operator=(const TrainStation&) = delete;

    static TrainStation& getInstance() {
        static TrainStation instance;
        return instance;
    }

    void addLuggageStorage(const string& name, int capacity) {
        try {
            shared_ptr<LuggageStorage> newStorage = make_shared<LuggageStorage>(name, capacity);
            storages.push_back(newStorage);
            cout << "Luggage storage added successfully." << endl;
        }
        catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }

    void displayStorages() const {
        if (storages.empty()) {
            cout << "No luggage storages in the train station." << endl;
        }
        else {
            cout << "Train Station Luggage Storages: " << endl;
            for (const auto& storage : storages) {
                storage->displayInfo();
            }
        }
    }
};

int main() {
    TrainStation& station = TrainStation::getInstance();

    station.addLuggageStorage("Storage A", 1000);
    station.addLuggageStorage("Storage B", 1500);

    station.displayStorages();

    return 0;
}
