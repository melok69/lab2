#include <iostream>
#include <vector>
#include <memory>
#include <string>

// Контейнеризируемый класс
class LuggageStorage {
private:
    std::string storageName;
    int capacity; // Вместимость в кг

public:
    LuggageStorage(const std::string& name, int capacity)
        : storageName(name), capacity(capacity) {
        if (capacity < 500 || capacity > 2000) {
            throw std::invalid_argument("Capacity must be between 500 and 2000 kg!");
        }
    }

    void displayInfo() const {
        std::cout << "Luggage Storage: " << storageName
            << ", Capacity: " << capacity << " kg" << std::endl;
    }

    std::string getName() const { return storageName; }
    int getCapacity() const { return capacity; }
};

// Класс-контейнер с шаблоном Singleton
class TrainStation {
private:
    std::vector<std::shared_ptr<LuggageStorage>> storages;

    // Закрытый конструктор
    TrainStation() {}

public:
    TrainStation(const TrainStation&) = delete;
    TrainStation& operator=(const TrainStation&) = delete;

    // Получение единственного экземпляра
    static TrainStation& getInstance() {
        static TrainStation instance;
        return instance;
    }

    // Добавление багажного отделения
    void addLuggageStorage(const std::string& name, int capacity) {
        try {
            std::shared_ptr<LuggageStorage> newStorage = std::make_shared<LuggageStorage>(name, capacity);
            storages.push_back(newStorage);
            std::cout << "Luggage storage added successfully." << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    // Отображение всех багажных отделений
    void displayStorages() const {
        if (storages.empty()) {
            std::cout << "No luggage storages in the train station." << std::endl;
        }
        else {
            std::cout << "Train Station Luggage Storages: " << std::endl;
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
