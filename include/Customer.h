#pragma once

#include <string>
#include <vector>


//class Order;

class Customer {
public:
    Customer(int id, const std::string &name, int locationDistance, int maxOrders);
    virtual ~Customer() = default;  // Dynamic (late binding) virtual destructor
    const std::string &getName() const;
    int getId() const;
    int getCustomerDistance() const;
    int getMaxOrders() const;
    int getNumOrders() const;
    bool canMakeOrder() const;
    const std::vector<int> &getOrdersIds() const;
    int addOrder(int orderId);
    virtual Customer *clone() const = 0; // Return a copy of the customer

private:
    const int id;
    const std::string name;
    const int locationDistance;
    const int maxOrders;
    std::vector<int> ordersId;
};
