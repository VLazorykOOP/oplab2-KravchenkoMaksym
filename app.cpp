#include <iostream>
#include <thread>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <cmath>
#include <mutex>
#include <locale>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Константи
const int NUM_STUDENTS = 5;
const int NUM_FEMALE_STUDENTS = 5;
const double SPEED = 1.0; // Швидкість
const int CHANGE_DIRECTION_INTERVAL = 2; // Інтервал зміни напрямку для студентів
const double RADIUS = 10.0; // Радіус кола для студенток

std::mutex mtx;

void moveStudent(int id, int simulation_time) {
    double x = 0.0, y = 0.0;
    srand(time(0) + id);
    auto start_time = std::chrono::steady_clock::now();
    while (true) {
        // Випадкова зміна напрямку
        double angle = (rand() % 360) * M_PI / 180.0;
        x += SPEED * cos(angle);
        y += SPEED * sin(angle);

        std::this_thread::sleep_for(std::chrono::seconds(CHANGE_DIRECTION_INTERVAL));

        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "Студент " << id << " знаходиться в (" << x << ", " << y << ")\n";
        std::cout << "----------------------------------------------\n";

        auto elapsed = std::chrono::steady_clock::now() - start_time;
        if (std::chrono::duration_cast<std::chrono::seconds>(elapsed).count() >= simulation_time) {
            break;
        }
    }
}

void moveFemaleStudent(int id, int simulation_time) {
    double angle = 0.0;
    auto start_time = std::chrono::steady_clock::now();
    while (true) {
        angle += SPEED / RADIUS;
        if (angle >= 2 * M_PI) {
            angle -= 2 * M_PI;
        }

        double x = RADIUS * cos(angle);
        double y = RADIUS * sin(angle);

        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "Студентка " << id << " знаходиться в (" << x << ", " << y << ")\n";
        std::cout << "==============================================\n";

        auto elapsed = std::chrono::steady_clock::now() - start_time;
        if (std::chrono::duration_cast<std::chrono::seconds>(elapsed).count() >= simulation_time) {
            break;
        }
    }
}

int main() {
    // Установлення локалі для коректного відображення символів
    setlocale(LC_CTYPE, "");

    int simulation_time;
    std::cout << "Введiть час моделювання в секундах: ";
    std::cin >> simulation_time;

    std::vector<std::thread> threads;

    // Запуск потоків для студентів
    for (int i = 0; i < NUM_STUDENTS; ++i) {
        threads.push_back(std::thread(moveStudent, i, simulation_time));
    }

    // Запуск потоків для студенток
    for (int i = 0; i < NUM_FEMALE_STUDENTS; ++i) {
        threads.push_back(std::thread(moveFemaleStudent, i, simulation_time));
    }

    // Очікування завершення всіх потоків
    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}
