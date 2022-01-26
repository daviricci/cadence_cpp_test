//
// Created by davim on 26/01/2022.
//

#ifndef _CADENCE_CPP_CORE_H
#define _CADENCE_CPP_CORE_H
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
namespace core {
    class Bomb{
        private:
            double x;
            double y;
            double r;
            int count_connected_bombs;
            std::map<Bomb*, int> connected_bombs;
            bool insert_bomb(Bomb *bomb);
        public:
            Bomb();
            Bomb(double x, double y, double r);
            ~Bomb();
            void get_data(double &x, double &y, double &r);
            bool it_is_connected(Bomb *bomb, bool insert = true);
            friend std::ofstream & operator<<(std::ofstream &ofs, Bomb b);
            friend std::ifstream & operator>>(std::ifstream &ifs,Bomb &b);
            friend std::ostream & operator<<(std::ostream &os,Bomb &b);
    };
    class Bridge{
        private:
            double width;
            double length;
            double bombs_n;
        public:
            Bridge();
            friend std::ofstream & operator<<(std::ofstream &ofs, Bridge b);
            friend std::ifstream & operator>>(std::ifstream &ifs,Bridge &b);
            friend std::ostream & operator<<(std::ostream &os,Bridge &b);
    };
    double calculate_distance_between_bomb_and_vertical(Bomb *b1, double x=0);
    double calculate_distance_between_bombs(Bomb *b1, Bomb *b2);
}

#endif //_CADENCE_CPP_CORE_H
