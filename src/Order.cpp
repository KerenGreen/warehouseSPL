#include "../include/Order.h"
#include <iostream>
Order::Order(int id, int customerId, int distance) :
   id(id), customerId(customerId), distance(distance),
     status(OrderStatus::PENDING), collectorId(NO_VOLUNTEER), driverId(NO_VOLUNTEER)
{
}

int Order::getId() const
{
    return id;
}

int Order::getCustomerId() const
{
    return customerId;
}

int Order::getDistance() const
{
    return distance;
}

void Order::setStatus(OrderStatus status)
{
    this->status = status;
}

void Order::setCollectorId(int collectorId)
{
    this->collectorId = collectorId;
}

void Order::setDriverId(int driverId)
{
    driverId = driverId;
}

int Order::getCollectorId() const
{
    return collectorId;
}

int Order::getDriverId() const
{
    return driverId;
}

OrderStatus Order::getStatus() const
{
    return status;
}

const string Order::toString() const
{
    return string();
}

string Order::statusToString() const
{
    string statusSTring = "";
    if(status == OrderStatus::PENDING)
        statusSTring = "PENDING";
    else if(status == OrderStatus::COLLECTING)
        statusSTring = "COLLECTING";
    else if(status == OrderStatus::DELIVERING)
        statusSTring = "DELIVERING";
    else statusSTring = "COMPLETED";
    return statusSTring;
}

Order * Order::clone() const{
    return new Order(*this);
}