#include <iostream>
#include "actor.h"
class tree : public actor
{
    public:
        void act()
        {
            std::cout << "Acting!" << std::endl;
        }

        void move()
        {
            std::cout << "Moving!" << std::endl;
        }
};
