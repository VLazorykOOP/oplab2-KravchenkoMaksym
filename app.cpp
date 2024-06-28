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

// ���������
const int NUM_STUDENTS = 5;
const int NUM_FEMALE_STUDENTS = 5;
const double SPEED = 1.0; // ��������
const int CHANGE_DIRECTION_INTERVAL = 2; // �������� ���� �������� ��� ��������
const double RADIUS = 10.0; // ����� ���� ��� ���������

std::mutex mtx;

void moveStudent(int id, int simulation_time) {
    double x = 0.0, y = 0.0;
    srand(time(0) + id);
    auto start_time = std::chrono::steady_clock::now();
    while (true) {
        // ��������� ���� ��������
        double angle = (rand() % 360) * M_PI / 180.0;
        x += SPEED * cos(angle);
        y += SPEED * sin(angle);

        std::this_thread::sleep_for(std::chrono::seconds(CHANGE_DIRECTION_INTERVAL));

        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "������� " << id << " ����������� � (" << x << ", " << y << ")\n";
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
        std::cout << "��������� " << id << " ����������� � (" << x << ", " << y << ")\n";
        std::cout << "==============================================\n";

        auto elapsed = std::chrono::steady_clock::now() - start_time;
        if (std::chrono::duration_cast<std::chrono::seconds>(elapsed).count() >= simulation_time) {
            break;
        }
    }
}

int main() {
    // ������������ ����� ��� ���������� ����������� �������
    setlocale(LC_CTYPE, "");

    int simulation_time;
    std::cout << "����i�� ��� ����������� � ��������: ";
    std::cin >> simulation_time;

    std::vector<std::thread> threads;

    // ������ ������ ��� ��������
    for (int i = 0; i < NUM_STUDENTS; ++i) {
        threads.push_back(std::thread(moveStudent, i, simulation_time));
    }

    // ������ ������ ��� ���������
    for (int i = 0; i < NUM_FEMALE_STUDENTS; ++i) {
        threads.push_back(std::thread(moveFemaleStudent, i, simulation_time));
    }

    // ���������� ���������� ��� ������
    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}
