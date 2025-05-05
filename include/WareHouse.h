#pragma once
#include <string>
#include <vector>
#include "Order.h"
#include "Customer.h"
#include "Volunteer.h"
//#include "../include/Volunteer.h"
#include "../include/Action.h"
#include <iostream>

class BaseAction;
class Volunteer;

class WareHouse {

    public:
        WareHouse(const string &configFilePath);
        void start();
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        const vector<BaseAction*> &getActions() const;
        void close();
        void open();
        void addCustomer(Customer* customer);
        //void addCustomer(const std::string &customerName, const std::string &customerType, int distance, int maxOrders);
        void addVolunteer(Volunteer* volunteer);
        vector<Order*> &getPendingOrders();
        vector<Order*> &getInProcessOrders();
        vector<Order*> &getCompleted();
        vector<Volunteer*> &getVolunteers();
        bool isOrderExist(int orderId) const;
        bool isCustomerExist(int customerId) const;
        bool isVolunteerExist(int volunteerId) const;
        int newOrderId();
        int newVolunteerId();
        int newCustomerId();

        ///RULE OF 5///
        virtual ~WareHouse();
        WareHouse(const WareHouse &other);
        WareHouse& operator=(const WareHouse &other);
        WareHouse(WareHouse &&other);
        WareHouse& operator=(WareHouse &&other);

    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs
        int orderCounter; //For assigning unique order IDs

};