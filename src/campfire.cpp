#include "campfire.hpp"

void campfire(Player &p){
	std::string input;
	std::cout<<p.getName()<<" found a place to rest"<<std::endl<<"He/she started a campfire";
	std::cout<<"Note: You obtained 2 additional skill points, you may want to allocate them."<<std::endl;
    p.addSkillPoints(2);
    allocateSkillPoints(p);
}
