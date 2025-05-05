#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include "../include/CivilianCustomer.h"
#include "../include/SoldierCustomer.h"
#include "../include/Volunteer.h"

using std::string;
using std::vector;

class WareHouse;

enum class ActionStatus{
    COMPLETED, ERROR
};

enum class CustomerType{
    Soldier, Civilian
};


class BaseAction{
    public:
        BaseAction();
        virtual ~BaseAction() = default; // Dynamic (late binding) virtual destructor
        ActionStatus getStatus() const;
        virtual void act(WareHouse& wareHouse)=0;
        virtual string toString() const=0;
        virtual BaseAction* clone() const=0;
        string getStatusString() const ;
        string actionStatusToString(ActionStatus status) const;

    protected:
        void complete();
        void error(string errorMsg);
        string getErrorMsg() const;

    private:
        string errorMsg;
        ActionStatus status;
};

class SimulateStep : public BaseAction {

    public:
        SimulateStep(int numOfSteps);
        void act(WareHouse &wareHouse) override;
        std::string toString() const override;
        SimulateStep *clone() const override;
        Volunteer* findAvailableCollector(vector<Volunteer*>& volunteers, Order &order);
        Volunteer* findAvailableDriver(vector<Volunteer*>& volunteers, Order &order);


    private:
        const int numOfSteps;
};

class AddOrder : public BaseAction {
    public:
        AddOrder(int id);
        void act(WareHouse &wareHouse) override;
        string toString() const override;
        AddOrder *clone() const override;
    private:
        const int customerId;
};


class AddCustomer : public BaseAction {
    public:
        AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders);
        CustomerType whichCustomerType(const string &customerType);
        void act(WareHouse &wareHouse) override;
        AddCustomer *clone() const override;
        string toString() const override;
        string typeCustomerToString() const;

    private:
        const string customerName;
        const CustomerType customerType;
        const int distance;
        const int maxOrders;
};



class PrintOrderStatus : public BaseAction {
    public:
        PrintOrderStatus(int id);
        void act(WareHouse &wareHouse) override;
        PrintOrderStatus *clone() const override;
        string toString() const override;
    private:
        const int orderId;
};

class PrintCustomerStatus: public BaseAction {
    public:
        PrintCustomerStatus(int customerId);
        void act(WareHouse &wareHouse) override;
        PrintCustomerStatus *clone() const override;
        string toString() const override;
    private:
        const int customerId;
};


class PrintVolunteerStatus : public BaseAction {
    public:
        PrintVolunteerStatus(int id);
        void act(WareHouse &wareHouse) override;
        PrintVolunteerStatus *clone() const override;
        string toString() const override;
    private:
        const int volunteerId;
};


class PrintActionsLog : public BaseAction {
    public:
        PrintActionsLog();
        void act(WareHouse &wareHouse) override;
        PrintActionsLog *clone() const override;
        string toString() const override;
    private:
};

class Close : public BaseAction {
    public:
        Close();
        void act(WareHouse &wareHouse) override;
        Close *clone() const override;
        string toString() const override;
    private:
};

class BackupWareHouse : public BaseAction {
    public:
        BackupWareHouse();
        void act(WareHouse &wareHouse) override;
        BackupWareHouse *clone() const override;
        string toString() const override;
    private:
};


class RestoreWareHouse : public BaseAction {
    public:
        RestoreWareHouse();
        void act(WareHouse &wareHouse) override;
        RestoreWareHouse *clone() const override;
        string toString() const override;
    private:
};