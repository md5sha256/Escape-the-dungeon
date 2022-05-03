#include "playerinfo.hpp"
#include "start.cpp"
#include <algorithm>
#include <iostream>
#include <string>

using namespace std;


int main(){
	player_info p;
	start(p);
	p.print_all_status();
}
