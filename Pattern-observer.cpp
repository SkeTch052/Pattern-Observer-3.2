#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

class Observer {
public:
    virtual ~Observer() = default;
    virtual void onWarning(const std::string& message) {}
    virtual void onError(const std::string& message) {}
    virtual void onFatalError(const std::string& message) {}
};

class Observable {
public:
    void addObserver(Observer* observer) {
        observers.push_back(observer);
    }

    void removeObserver(Observer* observer) {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

    void warning(const std::string& message) const {
        for (auto observer : observers) {
            observer->onWarning(message);
        }
    }

    void error(const std::string& message) const {
        for (auto observer : observers) {
            observer->onError(message);
        }
    }

    void fatalError(const std::string& message) const {
        for (auto observer : observers) {
            observer->onFatalError(message);
        }
    }

private:
    std::vector<Observer*> observers;
};

class LogCommandToConsole : public Observer {
public:
    void onWarning(const std::string& message) override {
        std::cout << "WARNING: " << message << std::endl;
    }

    void onFatalError(const std::string& message) override {
        std::cout << "FATAL ERROR: " << message << std::endl;
    }
};

class LogCommandToFile : public Observer {
public:
    LogCommandToFile(const std::string& filePath) : filePath(filePath) {}

    void onError(const std::string& message) override {
        std::ofstream file(filePath, std::ios_base::app);
        if (file) {
            file << "ERROR: " << message << std::endl;
        }
    }

    void onFatalError(const std::string& message) override {
        std::ofstream file(filePath, std::ios_base::app);
        if (file) {
            file << "FATAL ERROR: " << message << std::endl;
        }
    }

private:
    std::string filePath;
};

int main() {
    Observable observable;
    LogCommandToConsole logToConsole;
    LogCommandToFile logToFile("log.txt");

    observable.addObserver(&logToConsole);
    observable.addObserver(&logToFile);

    //observable.warning("Warning message");
    //observable.error("Error message");
    observable.fatalError("Fatal error message");

    return 0;
}