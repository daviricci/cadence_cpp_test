//
// Created by davim on 26/01/2022.
//

#ifndef _CADENCE_CPP_CORE_H
#define _CADENCE_CPP_CORE_H
#include <iostream>
#include <fstream>

namespace core {
    class Bomb{
        private:
            double x;
            double y;
            double r;
        public:
            Bomb();
            Bomb(double x, double y, double r);
            ~Bomb();
            friend std::ofstream & operator << (std::ofstream &ofs, Bomb b);
            friend std::ifstream & operator>>(std::ifstream &ifs,Bomb &b);
            friend std::ostream & operator<<(std::ostream &os,Bomb &b);
    };
}

#endif //_CADENCE_CPP_CORE_H
