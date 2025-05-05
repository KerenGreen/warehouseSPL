#include "../include/SoldierCustomer.h"

SoldierCustomer::SoldierCustomer(int id, std::string name, int locationDistance, int maxOrders)
    : Customer(id, name, locationDistance, maxOrders) {}

SoldierCustomer *SoldierCustomer::clone() const { //This declares a member function named clone within the SoldierCustomer class.
    return new SoldierCustomer(*this); //This line creates a new SoldierCustomer object on the heap using the new keyword. 
}
