//
// Created by davim on 26/01/2022.
//

#ifndef _CADENCE_CPP_CORE_H
#define _CADENCE_CPP_CORE_H

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <list>

namespace core {
    class Bomb {
    private:
        double x;
        double y;
        double r;
        int count_connected_bombs;
        std::map<Bomb *, int> connected_bombs;

        bool insert_bomb(Bomb *bomb);

        double get_min_dist_between_out_bomb_and_connected_bombs(Bomb *out_bomb, Bomb *own_bomb, double &min_distance);

    public:
        Bomb();

        Bomb(double x, double y, double r);

        ~Bomb();

        void get_xyr(double &x, double &y, double &r);

        bool it_is_connected(Bomb *bomb, bool insert = true);

        double get_min_dist_between_group_of_bombs(Bomb *out_bomb, double &min_distance);

        friend std::ofstream &operator<<(std::ofstream &ofs, Bomb b);

        friend std::ifstream &operator>>(std::ifstream &ifs, Bomb &b);

        friend std::ostream &operator<<(std::ostream &os, Bomb &b);
    };

    class Bridge {
    private:
        double width;
        double length;
        double bombs_n;
        bool destroyed;
    public:
        Bridge();

        ~Bridge();

        int get_number_bombs();

        friend std::ofstream &operator<<(std::ofstream &ofs, Bridge b);

        friend std::ifstream &operator>>(std::ifstream &ifs, Bridge &b);

        friend std::ostream &operator<<(std::ostream &os, Bridge &b);

        double get_length();

        double get_width();

        void set_destroyed(bool b);

        bool it_is_destroyed();
    };

    class BridgeAndBombsManipulator {
    private:
        Bridge bridge;
        std::map<Bomb *, int> bombs;
        double min_distance;
    public:
        BridgeAndBombsManipulator(std::ifstream &ifs);

        ~BridgeAndBombsManipulator();

        void sort_graph_and_evaluate(std::ofstream &ofs);

    };

    class Manipulator {
    private:
        int n_bridges;
        std::vector<BridgeAndBombsManipulator *> bridges;
    public:
        Manipulator(std::string file);

        ~Manipulator();

        void evaluate();

    };

}


#endif //_CADENCE_CPP_CORE_H
