//
// Created by davim on 26/01/2022.
//

#include "core.h"
#include <string>
namespace core {
    Bomb::Bomb() {x=0;y=0;r=0;}
    Bomb::Bomb(double x, double y, double r):x(x), y(y), r(r) {}
    Bomb::~Bomb() {}
    std::ofstream &operator<<(std::ofstream &ofs, Bomb b) {
        ofs<<b.x<<' '<<b.y<<' '<<b.r<<std::endl;
        return ofs;
    }

    std::ifstream &operator>>(std::ifstream &ifs, Bomb &b) {
        std::string data;
        int i=0;
        int j=0;
        int k=0;
        double *config[3] = {&b.x, &b.y, &b.r};
        std::getline(ifs, data);
        do{
            if (data.c_str()[i]==' ' || data.c_str()[i] =='\0'){
                *config[j] = std::strtof(data.substr(k,i).c_str(),nullptr);
                k=i+1;
                j++;
            }
            i++;
        }while ( data.c_str()[i]!='\0');
        *config[j] = std::strtof(data.substr(k,i).c_str(),nullptr);
        return ifs;
    }

    std::ostream &operator<<(std::ostream &os, Bomb &b) {
        os<<b.x<<' '<<b.y<<' '<<b.r<<std::endl;
        return os;
    }

}