//
// Created by davim on 26/01/2022.
//
#include "core.h"
#include <string>
#include <cmath>

namespace core {

    double calculate_distance_between_center_bombs(Bomb *b1, Bomb *b2) {
        double x_1, y_1, r_1;
        b1->get_xyr(x_1, y_1, r_1);
        double x_2, y_2, r_2;
        b2->get_xyr(x_2, y_2, r_2);
        return std::sqrt((x_1 - x_2) * (x_1 - x_2) + (y_1 - y_2) * (y_1 - y_2));
    }

    double calculate_min_dist_between_bombs(Bomb *b1, Bomb *b2) {
        double x_n, y_n, r_1, r_2, dist;
        b1->get_xyr(x_n, y_n, r_1);
        b2->get_xyr(x_n, y_n, r_2);
        dist = calculate_distance_between_center_bombs(b1, b2) - r_1 - r_2;
        dist = dist < 0. ? 0. : dist;
        return dist;
    }

    double calculate_distance_between_center_bomb_and_vertical(Bomb *b1, double x = 0) {
        double x_1, y_1, r_1;
        b1->get_xyr(x_1, y_1, r_1);
        double x_2 = x, y_2 = y_1;
        return std::sqrt((x_1 - x_2) * (x_1 - x_2) + (y_1 - y_2) * (y_1 - y_2));
    }

    double calulate_min_dist_between_bomb_and_vertical(Bomb *b1, double x = 0) {
        double x_1, y_1, r_1, dist;
        b1->get_xyr(x_1, y_1, r_1);
        dist = calculate_distance_between_center_bomb_and_vertical(b1, x) - r_1;
        dist = dist < 0. ? 0. : dist;
        return dist;
    }

    bool BuildTheGraph(Bomb *pivo, std::map<Bomb *, int> &bombs, double x, double &min_dist_to_side) {
        std::map<Bomb *, int>::iterator it_bombs = bombs.begin();
        Bomb *thr_bomb = NULL;
        while (it_bombs != bombs.cend()) {
            thr_bomb = (Bomb *) it_bombs->first;
            if (pivo->it_is_connected(thr_bomb)) {
                bombs.erase(thr_bomb);
                double local_dist = calulate_min_dist_between_bomb_and_vertical(thr_bomb, x);
                min_dist_to_side = local_dist < min_dist_to_side ? local_dist : min_dist_to_side;
                BuildTheGraph(thr_bomb, bombs, x, min_dist_to_side);
                it_bombs = bombs.begin();
            } else {
                it_bombs++;
            }
        }
        return true;
    }

    bool BuildTheGraphs(std::list<Bomb *> pivos, std::map<Bomb *, int> bombs, double x, double &min_dist_to_side) {
        Bomb *pivo;
        std::list<Bomb *>::iterator it_pivos = pivos.begin();
        while (it_pivos != pivos.cend()) {
            pivo = *it_pivos;
            bombs.erase(pivo);
            double local_dist = calulate_min_dist_between_bomb_and_vertical(pivo, x);
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

    double
    Bomb::get_min_dist_between_out_bomb_and_connected_bombs(Bomb *out_bomb, Bomb *own_bomb, double &min_distance) {
        std::map<Bomb *, int>::iterator it = own_bomb->connected_bombs.begin();
        double local_dist;
        local_dist = calculate_min_dist_between_bombs(out_bomb, own_bomb);
        min_distance = min_distance < local_dist ? min_distance : local_dist;
        while (it != own_bomb->connected_bombs.cend()) {
            Bomb *thr_bomb = (Bomb *) it->first;
            thr_bomb->get_min_dist_between_out_bomb_and_connected_bombs(out_bomb, thr_bomb, min_distance);
            local_dist = calculate_min_dist_between_bombs(out_bomb, own_bomb);
            min_distance = min_distance < local_dist ? min_distance : local_dist;
            it++;
        }
        return min_distance;
    }

    double Bomb::get_min_dist_between_group_of_bombs(Bomb *out_bomb, double &min_distance) {
        std::map<Bomb *, int>::iterator it = out_bomb->connected_bombs.begin();
        this->get_min_dist_between_out_bomb_and_connected_bombs(out_bomb, this, min_distance);
        while (it != out_bomb->connected_bombs.cend()) {
            Bomb *thr_bomb = it->first;
            this->get_min_dist_between_out_bomb_and_connected_bombs(out_bomb, this, min_distance);
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
            return true;
        }
        return false;
    }

    bool Bomb::it_is_connected(Bomb *bomb, bool insert) {
        if (bomb == this) return false;
        double x_1, y_1, r_1;
        bomb->get_xyr(x_1, y_1, r_1);
        double dist = calculate_distance_between_center_bombs(this, bomb);
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
            this->bombs.insert(std::pair<Bomb *, int>(bomb, 0));
            n += 1;
        }
    }

    BridgeAndBombsManipulator::~BridgeAndBombsManipulator() {
        std::map<Bomb *, int>::iterator it_bombs = this->bombs.begin();
        while (it_bombs != this->bombs.cend()) {
            Bomb *bomb = (Bomb *) it_bombs->first;
            delete bomb;
            bomb = NULL;
            it_bombs++;
        }
    }

    void BridgeAndBombsManipulator::init_pivos(std::map<Bomb *, int> &bombs_copy, std::list<Bomb *> &pivos_left,
                                               std::list<Bomb *> &pivos_right) {
        Bomb *thr_bomb = NULL;
        std::map<Bomb *, int>::iterator it_bombs = bombs_copy.begin();
        while (it_bombs != bombs_copy.cend()) {
            thr_bomb = (Bomb *) it_bombs->first;
            if ((calulate_min_dist_between_bomb_and_vertical(thr_bomb, this->bridge.get_width()) == 0.) &
                (calulate_min_dist_between_bomb_and_vertical(thr_bomb, 0.) == 0.)) {
                this->bridge.set_destroyed(true);
            } else if (calulate_min_dist_between_bomb_and_vertical(thr_bomb, 0.) == 0.) {
                pivos_left.push_back((Bomb *) it_bombs->first);
            } else if (calulate_min_dist_between_bomb_and_vertical(thr_bomb, this->bridge.get_width()) == 0.) {
                pivos_right.push_back((Bomb *) it_bombs->first);
            }
            it_bombs++;
        }
    }

    void BridgeAndBombsManipulator::init_graphs(std::map<Bomb *, int> &bombs_copy, std::list<Bomb *> &pivos_left,
                                                std::list<Bomb *> &pivos_right, double &min_distance_to_right,
                                                double &min_distance_to_left) {
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
    }

    void BridgeAndBombsManipulator::calc_dist_between_graphs_left_and_right(std::list<Bomb *> &pivos_left,
                                                                            std::list<Bomb *> &pivos_right) {
        Bomb *left, *right;
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
                it_right = pivos_right.begin();
                it_left++;
            }
        }
    }

    void BridgeAndBombsManipulator::test_min_dists(double &min_distance_to_left, double &min_distance_to_right) {
        this->min_distance = this->min_distance < min_distance_to_left ? this->min_distance : min_distance_to_left;
        this->min_distance = this->min_distance < min_distance_to_right ? this->min_distance : min_distance_to_right;
        (this->min_distance == 0.) ? this->bridge.set_destroyed(true) : void();
    }

    void BridgeAndBombsManipulator::plot_final_result(std::ofstream &ofs) {
        if (this->bridge.it_is_destroyed()) {
            std::cout << "Bridge already split" << std::endl;
            ofs << "Bridge already split" << std::endl;
        } else {
            int min = std::ceil(this->min_distance / 2.);
            std::string out = std::to_string(min);
            std::cout << out << std::endl;
            ofs << out << std::endl;
        }
    }

    void BridgeAndBombsManipulator::sort_graph_and_evaluate(std::ofstream &ofs) {
        std::map<Bomb *, int> bombs_copy = this->bombs;
        std::list<Bomb *> pivos_left, pivos_right;
        pivos_left.clear();
        pivos_right.clear();
        double min_distance_to_left = this->bridge.get_width();
        double min_distance_to_right = this->bridge.get_width();
        this->min_distance = this->bridge.get_width();
        init_pivos(bombs_copy, pivos_left, pivos_right);
        init_graphs(bombs_copy, pivos_left, pivos_right, min_distance_to_left, min_distance_to_right);
        calc_dist_between_graphs_left_and_right(pivos_right, pivos_left);
        test_min_dists(min_distance_to_left, min_distance_to_right);
        plot_final_result(ofs);
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
        while (it != this->bridges.cend()) {
            BridgeAndBombsManipulator *manipulator = (BridgeAndBombsManipulator *) *it;
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