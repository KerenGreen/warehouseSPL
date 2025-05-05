#pragma once

#include "Customer.h"

class SoldierCustomer : public Customer {
public:
    SoldierCustomer(int id, std::string name, int locationDistance, int maxOrders);
    //SoldierCustomer::~SoldierCustomer() {}
    SoldierCustomer *clone() const override;
};
