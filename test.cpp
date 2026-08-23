#include <iostream>
#include "Library.hpp"

int main(){
    MalClient mal(Config::getDefaults());
    mal.getAnimebyId(1);

}