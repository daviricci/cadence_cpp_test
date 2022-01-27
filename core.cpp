//
// Created by davim on 26/01/2022.
//
#include "core.h"
#include <string>
#include <cmath>

namespace core {

    double calculate_distance_between_bombs(Bomb *b1, Bomb *b2) {
        double x_1, y_1, r_1;
        b1->get_xyr(x_1, y_1, r_1);
        double x_2, y_2, r_2;
        b2->get_xyr(x_2, y_2, r_2);
        return std::sqrt((x_1 - x_2) * (x_1 - x_2) + (y_1 - y_2) * (y_1 - y_2));
    }

    double calculate_distance_between_bomb_and_vertical(Bomb *b1, double x) {
        double x_1, y_1, r_1;
        b1->get_xyr(x_1, y_1, r_1);
        double x_2 = x, y_2 = y_1;
        return std::sqrt((x_1 - x_2) * (x_1 - x_2) + (y_1 - y_2) * (y_1 - y_2));
    }

    bool BuildTheGraph(Bomb *pivo, std::list<Bomb *> &bombs, double x, double &min_dist_to_side) {
        std::list<Bomb *>::iterator it_bombs = bombs.begin();
        Bomb *thr_bomb = NULL;
        while (it_bombs != bombs.cend()) {
            thr_bomb = (Bomb *) *it_bombs;
            if (pivo->it_is_connected(thr_bomb)) {
                bombs.remove(thr_bomb);
                double x_b, y_b, r;
                thr_bomb->get_xyr(x_b, y_b, r);
                double local_dist = calculate_distance_between_bomb_and_vertical(thr_bomb, x) - r;
                local_dist = local_dist < 0 ? 0 : local_dist;
                min_dist_to_side = local_dist < min_dist_to_side ? local_dist : min_dist_to_side;
                BuildTheGraph(thr_bomb, bombs, x, min_dist_to_side);
                it_bombs = bombs.begin();
            } else {
                it_bombs++;
            }
        }
        return true;
    }

    bool BuildTheGraphs(std::list<Bomb *> pivos, std::list<Bomb *> bombs, double x, double &min_dist_to_side) {
        Bomb *pivo;
        std::list<Bomb *>::iterator it_pivos = pivos.begin();
        while (it_pivos != pivos.cend()) {
            pivo = *it_pivos;
            double x_b, y_b, r;
            pivo->get_xyr(x_b, y_b, r);
            double local_dist = calculate_distance_between_bomb_and_vertical(pivo, x) - r;
            local_dist = local_dist < 0 ? 0 : local_dist;
            min_dist_to_side = local_dist < min_dist_to_side ? local_dist : min_dist_to_side;
            BuildTheGraph(pivo, bombs, x, min_dist_to_side);
            it_pivos++;
        }
        return true;
    }

    Bomb::Bomb() {
        x = 0;
        y = 0;
        r = 0;
        count_connected_bombs = 0;
    }

    Bomb::Bomb(double x, double y, double r) : x(x), y(y), r(r) { count_connected_bombs = 0; }

    Bomb::~Bomb() {
        this->connected_bombs.clear();
    }

    void Bomb::get_xyr(double &x, double &y, double &r) {
        x = this->x;
        y = this->y;
        r = this->r;
    }

    double Bomb::get_min_dist_between_out_bomb_and_connected_bombs(Bomb *out_bomb, double &min_distance) {
        std::map<Bomb *, int>::iterator it = this->connected_bombs.begin();
        double x, y, r1, r2, min_local;
        while (it != this->connected_bombs.cend()) {
            Bomb *thr_bomb =(Bomb*) it->first;
            out_bomb->get_xyr(x, y, r1);
            thr_bomb->get_xyr(x, y, r2);
            min_local = calculate_distance_between_bombs(out_bomb, thr_bomb) - r1 - r2;
            min_local = min_local < 0. ? 0. : min_local;
            min_distance = min_local < min_distance ? min_local : min_distance;
            thr_bomb->get_min_dist_between_out_bomb_and_connected_bombs(out_bomb, min_distance);
            it++;
        }
        return min_distance;
    }

    double Bomb::get_min_dist_between_group_of_bombs(Bomb *out_bomb, double &min_distance) {
        double x, y, r1, r2, min_local;
        out_bomb->get_xyr(x, y, r1);
        this->get_xyr(x, y, r2);
        min_local = calculate_distance_between_bombs(out_bomb, this) - r1 - r2;
        min_local = min_local < 0. ? 0. : min_local;
        min_distance = min_local < min_distance ? min_local : min_distance;
        std::map<Bomb *, int>::iterator it = out_bomb->connected_bombs.begin();
        while (it != out_bomb->connected_bombs.cend()) {
            Bomb *thr_bomb = it->first;
            this->get_min_dist_between_out_bomb_and_connected_bombs(thr_bomb, min_distance);
            this->get_min_dist_between_group_of_bombs(thr_bomb, min_distance);
            it++;
        }
        return min_distance;
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
        if (bomb == this) return false;
        double x_1, y_1, r_1;
        bomb->get_xyr(x_1, y_1, r_1);
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


    Bridge::Bridge() { this->destroyed = false; }

    Bridge::~Bridge() {}

    int Bridge::get_number_bombs() {
        return this->bombs_n;
    }

    std::ofstream &operator<<(std::ofstream &ofs, Bridge b) {
        ofs << b.width << ' ' << b.length << ' ' << b.bombs_n << std::endl;
        return ofs;
    }

    std::ifstream &operator>>(std::ifstream &ifs, Bridge &b) {
        std::string data;
        int i = 0;
        int j = 0;
        int k = 0;
        double *config[3] = {&b.width, &b.length, &b.bombs_n};
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
        os << b.width << ' ' << b.length << ' ' << b.bombs_n << std::endl;
        return os;
    }

    double Bridge::get_length() {
        return this->length;
    }

    double Bridge::get_width() {
        return this->width;
    }

    void Bridge::set_destroyed(bool b) {
        this->destroyed = b;
    }

    bool Bridge::it_is_destroyed() {
        return this->destroyed;
    }


    BridgeAndBombsManipulator::BridgeAndBombsManipulator(std::ifstream &ifs) {
        std::string data;
        this->min_distance = std::numeric_limits<double>::infinity();
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
        std::list<Bomb *>::iterator it_bombs = this->bombs.begin();
        while (it_bombs != this->bombs.cend()) {
            Bomb *bomb = (Bomb*)*it_bombs;
            delete bomb;
            bomb = NULL;
            it_bombs ++;
        }
    }

    void BridgeAndBombsManipulator::sort_graph_and_evaluate(std::ofstream &ofs) {
        std::list<Bomb *> bombs_copy = this->bombs;
        std::list<Bomb *>::iterator it_bombs = bombs_copy.begin();
        std::list<Bomb *> pivos_left, pivos_right;
        pivos_left.clear();
        pivos_right.clear();
        Bomb *thr_bomb = NULL;
        Bomb *left, *right;
        double min_distance_to_left = this->bridge.get_width();
        double min_distance_to_right = this->bridge.get_width();
        this->min_distance = this->bridge.get_width();
        double x, y, r;
        while (it_bombs != bombs_copy.cend()) {
            thr_bomb = (Bomb *) *it_bombs;
            thr_bomb->get_xyr(x, y, r);
            if ((calculate_distance_between_bomb_and_vertical(thr_bomb, this->bridge.get_width()) <= r) &
                (calculate_distance_between_bomb_and_vertical(thr_bomb, 0) <= r)) {
                this->bridge.set_destroyed(true);
            } else if (calculate_distance_between_bomb_and_vertical(thr_bomb, 0) <= r) {
                pivos_left.push_back((Bomb *) *it_bombs);
            } else if (calculate_distance_between_bomb_and_vertical(thr_bomb, this->bridge.get_width()) <= r) {
                pivos_right.push_back((Bomb *) *it_bombs);
            }
            it_bombs++;
        }
        if ((!pivos_left.empty()) && (!this->bridge.it_is_destroyed())) {
            BuildTheGraphs(pivos_left, bombs_copy, this->bridge.get_width(), min_distance_to_right);
            if (min_distance_to_right == 0.) {
                this->bridge.set_destroyed(true);
                this->min_distance = 0;
            }
        }
        if ((!pivos_right.empty()) && (!this->bridge.it_is_destroyed())) {
            BuildTheGraphs(pivos_right, bombs_copy, 0., min_distance_to_left);
            if (min_distance_to_left == 0.) {
                this->bridge.set_destroyed(true);
                this->min_distance = 0;
            }
        }
        if (!this->bridge.it_is_destroyed() && (!pivos_left.empty()) && (!pivos_right.empty())) {
            std::list<Bomb *>::iterator it_left = pivos_left.begin();
            std::list<Bomb *>::iterator it_right = pivos_right.begin();
            while (it_left != pivos_left.cend()) {
                left = (Bomb *) *it_left;
                while (it_right != pivos_right.cend()) {
                    right = (Bomb *) *it_right;
                    left->get_min_dist_between_group_of_bombs(right, this->min_distance);
                    it_right++;
                }
                it_left++;
            }
            this->min_distance = this->min_distance < min_distance_to_left ? this->min_distance : min_distance_to_left;
            this->min_distance = this->min_distance < min_distance_to_right ? this->min_distance : min_distance_to_right;
        }

        if (this->bridge.it_is_destroyed()) {
            std::cout << "Bridge already split" << std::endl;
            ofs << "Bridge already split" << std::endl;
        } else {
            int min = std::ceil(this->min_distance / 2.);
            std::string out = std::to_string(min);
            std::cout<<out<< std::endl;
            ofs << out<< std::endl;
        }
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

    Manipulator::~Manipulator() {
        std::vector<BridgeAndBombsManipulator *>::iterator it = this->bridges.begin();
        while(it!=this->bridges.cend()){
            BridgeAndBombsManipulator *manipulator = (BridgeAndBombsManipulator*) *it;
            delete manipulator;
            manipulator = NULL;
            it++;
        }
    }

    void Manipulator::evaluate() {
        std::vector<BridgeAndBombsManipulator *>::iterator it = this->bridges.begin();
        BridgeAndBombsManipulator *manipulator;
        std::ofstream ofs("output.txt");
        while (it != this->bridges.cend()) {
            manipulator = (BridgeAndBombsManipulator *) *it;
            manipulator->sort_graph_and_evaluate(ofs);
            it++;
        }
    }


}