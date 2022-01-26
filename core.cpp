//
// Created by davim on 26/01/2022.
//
#include "core.h"
#include <string>
#include <cmath>

namespace core {

    double calculate_distance_between_bombs(Bomb *b1, Bomb *b2) {
        double x_1, y_1, r_1;
        b1->get_data(x_1, y_1, r_1);
        double x_2, y_2, r_2;
        b2->get_data(x_2, y_2, r_2);
        return std::sqrt((x_1 - x_2) * (x_1 - x_2) + (y_1 - y_2) * (y_1 - y_2));
    }

    double calculate_distance_between_bomb_and_vertical(Bomb *b1, double x) {
        double x_1, y_1, r_1;
        b1->get_data(x_1, y_1, r_1);
        double x_2 = x, y_2 = y_1;
        return std::sqrt((x_1 - x_2) * (x_1 - x_2) + (y_1 - y_2) * (y_1 - y_2));
    }

    Bomb::Bomb() {
        x = 0;
        y = 0;
        r = 0;
        count_connected_bombs = 0;
    }

    Bomb::Bomb(double x, double y, double r) : x(x), y(y), r(r) { count_connected_bombs = 0; }

    Bomb::~Bomb() {}

    void Bomb::get_data(double &x, double &y, double &r) {
        x = this->x;
        y = this->y;
        r = this->r;
    }

    bool Bomb::insert_bomb(Bomb *bomb) {
        std::map<Bomb *, int>::iterator it = this->connected_bombs.find(bomb);
        if (it == this->connected_bombs.cend()) {
            this->count_connected_bombs += 1;
            this->connected_bombs.insert(std::pair<Bomb *, int>(bomb, this->count_connected_bombs));
            bomb->it_is_connected(this);
            return true;
        }
        return false;
    }

    bool Bomb::it_is_connected(Bomb *bomb, bool insert) {
        double x_1, y_1, r_1;
        bomb->get_data(x_1, y_1, r_1);
        double dist = calculate_distance_between_bombs(this, bomb);
        if (dist <= r_1 + this->r) {
            if (insert) {
                insert_bomb(bomb);
            }
            return true;
        }
        return false;
    }

    std::ofstream &operator<<(std::ofstream &ofs, Bomb b) {
        ofs << b.x << ' ' << b.y << ' ' << b.r << std::endl;
        return ofs;
    }

    std::ifstream &operator>>(std::ifstream &ifs, Bomb &b) {
        std::string data;
        int i = 0;
        int j = 0;
        int k = 0;
        double *config[3] = {&b.x, &b.y, &b.r};
        std::getline(ifs, data);
        do {
            if (data.c_str()[i] == ' ' || data.c_str()[i] == '\0') {
                *config[j] = std::strtof(data.substr(k, i).c_str(), nullptr);
                k = i + 1;
                j++;
            }
            i++;
        } while (data.c_str()[i] != '\0');
        *config[j] = std::strtof(data.substr(k, i).c_str(), nullptr);
        return ifs;
    }

    std::ostream &operator<<(std::ostream &os, Bomb &b) {
        os << b.x << ' ' << b.y << ' ' << b.r << std::endl;
        return os;
    }


    Bridge::Bridge() {}

    Bridge::~Bridge() {}

    int Bridge::get_number_bombs() {
        return this->bombs_n;
    }

    std::ofstream &operator<<(std::ofstream &ofs, Bridge b) {
        ofs << b.length << ' ' << b.width << ' ' << b.bombs_n << std::endl;
        return ofs;
    }

    std::ifstream &operator>>(std::ifstream &ifs, Bridge &b) {
        std::string data;
        int i = 0;
        int j = 0;
        int k = 0;
        double *config[3] = {&b.length, &b.width, &b.bombs_n};
        std::getline(ifs, data);
        do {
            if (data.c_str()[i] == ' ' || data.c_str()[i] == '\0') {
                *config[j] = std::strtof(data.substr(k, i).c_str(), nullptr);
                k = i + 1;
                j++;
            }
            i++;
        } while (data.c_str()[i] != '\0');
        *config[j] = std::strtof(data.substr(k, i).c_str(), nullptr);
        return ifs;
    }

    std::ostream &operator<<(std::ostream &os, Bridge &b) {
        os << b.length << ' ' << b.width << ' ' << b.bombs_n << std::endl;
        return os;
    }


    BridgeAndBombsManipulator::BridgeAndBombsManipulator(std::ifstream &ifs) {
        std::string data;
        ifs >> this->bridge;
        int n = 0;
        while (n < this->bridge.get_number_bombs()) {
            Bomb *bomb = new Bomb();
            ifs >> *bomb;
            this->bombs.push_back(bomb);
            n += 1;
        }
    }

    BridgeAndBombsManipulator::~BridgeAndBombsManipulator() {

    }

    Manipulator::Manipulator(std::string file) {
        std::ifstream ifs(file);
        std::string data;
        std::getline(ifs, data);
        this->n_bridges = (int) std::strtof(data.c_str(), nullptr);
        int n = 0;
        while (n < this->n_bridges) {
            this->bridges.push_back(new BridgeAndBombsManipulator(ifs));
            n++;
        }
    }

    Manipulator::~Manipulator() {}


}