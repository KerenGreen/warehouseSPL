#pragma once

#include "Customer.h"

class CivilianCustomer : public Customer {
public:
    CivilianCustomer(int id, std::string name, int locationDistance, int maxOrders);
    CivilianCustomer *clone() const override;
};
