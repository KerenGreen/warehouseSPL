#include "../include/Warehouse.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "WareHouse.h"



WareHouse::WareHouse(const string& configFilePath):
isOpen(false), actionsLog(), volunteers(), pendingOrders(),
 inProcessOrders(), completedOrders(), customers(), customerCounter(0), volunteerCounter(0), orderCounter(0)
{
    string line;
    std::ifstream file(configFilePath);
    while (getline (file, line)) {
        if(line != ""){
            std::istringstream iss(line);
            std::vector<string> words;
            string word;
         
            while (iss >> word) {
                words.push_back(word);
            }
           
           
            if(words[0]=="customer")
            {
                AddCustomer* addCustomer = new AddCustomer(words[1], words[2], std::stoi(words[3]), std::stoi(words[4]));
                addCustomer->act(*this);
                delete addCustomer;
            }
            if(words[0]=="volunteer")
            {
                if(words[2] == "collector")
                {
                    CollectorVolunteer *collectorVolunteer = new CollectorVolunteer(newVolunteerId(), words[1], std::stoi(words[3]));
                    volunteers.push_back(collectorVolunteer);
                }
                else if(words[2] == "limited_collector")
                {
                    LimitedCollectorVolunteer *limitedCollectorVolunteer = new LimitedCollectorVolunteer(newVolunteerId(), words[1], std::stoi(words[3]), std::stoi(words[4]));
                    volunteers.push_back(limitedCollectorVolunteer);
                }
                else if(words[2] == "driver")
                {
                    DriverVolunteer *driverVolunteer = new DriverVolunteer(newVolunteerId(), words[1],std::stoi(words[3]),std::stoi(words[4]));
                    volunteers.push_back(driverVolunteer);
                }
                else if(words[2] == "limited_driver")
                {
                    LimitedDriverVolunteer *limitedDriverVolunteer = new LimitedDriverVolunteer(newVolunteerId(), words[1],std::stoi(words[3]),std::stoi(words[4]),std::stoi(words[5]));
                    volunteers.push_back(limitedDriverVolunteer);
                }
                else
                {
                    throw std::invalid_argument("WareHouse constructor: volunteer type is not valid");
                }
               
            }
           
        }
    }

    file.close();
}


void WareHouse::start() {
    open();
    string userInput; 
    do {
        std::getline(std::cin,userInput);
        std::stringstream ss(userInput);
        vector<string> split;

        string s;
        while(getline(ss,s, ' ')) {
            split.push_back(s);
        }

        if (split[0] == "step") {
            SimulateStep* newStep = new SimulateStep(stoi(split[1]));
            newStep->act(*this);
            addAction(newStep);
            continue;
        }

        if (split[0] == "order") {
            AddOrder* newOrder = new AddOrder(stoi(split[1]));
            newOrder->act(*this);
            addAction(newOrder);
            continue;
        }

        if (split[0] == "customer") {
            AddCustomer* newCustomer = new AddCustomer(split[1], split[2], stoi(split[3]), stoi(split[4]));
            newCustomer->act(*this);
            addAction(newCustomer);
            continue;
        }

        if (split[0] == "orderStatus") {
            PrintOrderStatus* orderStatus = new PrintOrderStatus(stoi(split[1]));
            orderStatus->act(*this);
            addAction(orderStatus);
            continue;
        }

        if (split[0] == "customerStatus") {
            PrintCustomerStatus* customerStatus = new PrintCustomerStatus(stoi(split[1]));
            customerStatus->act(*this);
            addAction(customerStatus);
            continue;
        }

        if (split[0] == "volunteerStatus") {
            PrintVolunteerStatus* volunteerStatus = new PrintVolunteerStatus(stoi(split[1]));
            volunteerStatus->act(*this);
            addAction(volunteerStatus);
            continue;
        }

        if (split[0] == "log") {
            PrintActionsLog* actionLog = new PrintActionsLog();
            actionLog->act(*this);
            addAction(actionLog);
            continue;
        }

        if (split[0] == "close") {
            Close* close = new Close();
            close->act(*this);
            addAction(close);
            return; 
        }

        if (split[0] == "backup") {
            BackupWareHouse* back = new BackupWareHouse();
            back->act(*this);
            addAction(back);
            continue;
        }

        if (split[0] == "restore") {
            RestoreWareHouse* restore = new RestoreWareHouse();
            restore->act(*this);
            addAction(restore);
            continue;
        }

    } while(userInput != "close");
}

void WareHouse::open() {
    isOpen = true;
    std::cout << "Warehouse is open!" << std::endl;
}

void WareHouse::addCustomer(Customer *customer)
{
    customers.push_back(customer);
}

void WareHouse::close() {
    isOpen = false;
}

////Destructor
WareHouse::~WareHouse() {
    for (BaseAction* action : actionsLog) {
        delete action;
    }

    for (Volunteer* volunteer : volunteers) {
        delete volunteer;
    }

    for (Order* order : pendingOrders) {
        delete order;
    }

    for (Order* order : inProcessOrders) {
        delete order;
    }

    for (Order* order : completedOrders) {
        delete order;
    }

    for (Customer* customer : customers) {
        delete customer;
    }
}



WareHouse::WareHouse(const WareHouse &other):
isOpen(other.isOpen),actionsLog(),volunteers(),pendingOrders(),inProcessOrders(),
completedOrders(),customers(),customerCounter(other.customerCounter),volunteerCounter(other.volunteerCounter),orderCounter(other.orderCounter)
{
    for (Order *o:other.pendingOrders) {
        pendingOrders.push_back(o->clone());
    }

    for (Order *o:other.inProcessOrders) {
        inProcessOrders.push_back(o->clone());
    }

    for (Order *o:other.completedOrders) {
        completedOrders.push_back(o->clone());
    }

    for (Volunteer *v:other.volunteers) {
        volunteers.push_back(v->clone());
    }

    for (Customer *c:other.customers) {
        customers.push_back(c->clone());
    }

    for (BaseAction *a:other.actionsLog) {
        actionsLog.push_back(a->clone());
    }
}

///COPY ASSIGNMENT///
WareHouse& WareHouse::operator=(const WareHouse& other){
    if(this != &other ){

        isOpen =other.isOpen;
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        orderCounter =other.orderCounter;
       
        ///Clear the existing content of vectors and release any resources they hold
        for (BaseAction* action : actionsLog) {
            delete action;
        }
        for (Volunteer* volunteer : volunteers) {
            delete volunteer;
        }
        for (Order* order : pendingOrders) {
            delete order;
        }
        for (Order* order : inProcessOrders) {
            delete order;
        }
        for (Order* order : completedOrders) {
            delete order;
        }
        for (Customer* customer : customers) {
            delete customer;
        }

        //clear our vectors from old pointers
        actionsLog.clear();
        volunteers.clear();
        pendingOrders.clear();
        inProcessOrders.clear();
        completedOrders.clear();
        customers.clear();
   
        //copy new data
        for(BaseAction * action : other.actionsLog){
            actionsLog.push_back(action->clone());
        }

        for(Volunteer * v : other.volunteers){
            volunteers.push_back(v->clone());
        }

        for(Order * o  : other.pendingOrders){
            pendingOrders.push_back(o->clone());
        }

        for(Order * o : other.inProcessOrders){
            inProcessOrders.push_back(o->clone());
        }
       
        for(Order  * o  : other.completedOrders){
            completedOrders.push_back(o->clone());
        }
       
        for(Customer * c : other.customers){
            customers.push_back(c->clone());
        }
    }
    return *this;
}

//move constructor
WareHouse::WareHouse(WareHouse &&other):
isOpen(other.isOpen),actionsLog(),volunteers(),pendingOrders(),inProcessOrders(),
completedOrders(),customers(),customerCounter(other.customerCounter),volunteerCounter(other.volunteerCounter),orderCounter(other.orderCounter)
{
    for (Order *o:other.pendingOrders) {
        pendingOrders.push_back(o);
    }

    for (Order *o:other.inProcessOrders) {
        inProcessOrders.push_back(o);
    }

    for (Order *o:other.completedOrders) {
        completedOrders.push_back(o);
    }

    for (Volunteer *v:other.volunteers) {
        volunteers.push_back(v);
    }

    for (Customer *c:other.customers) {
        customers.push_back(c);
    }

    for (BaseAction *a:other.actionsLog) {
        actionsLog.push_back(a);
    }

    other.pendingOrders.clear();
    other.inProcessOrders.clear();
    other.completedOrders.clear();
    other.volunteers.clear();
    other.customers.clear();
    other.actionsLog.clear();
}
//move assignment operator
WareHouse& WareHouse::operator=(WareHouse&& other){
    if(this != &other ){
        //Clear the existing content of vectors and release any resources they hold
        for (BaseAction* action : actionsLog) {
            delete action;
        }
        for (Volunteer* volunteer : volunteers) {
            delete volunteer;
        }
        for (Order* order : pendingOrders) {
            delete order;
        }
        for (Order* order : inProcessOrders) {
            delete order;
        }
        for (Order* order : completedOrders) {
            delete order;
        }
        for (Customer* customer : customers) {
            delete customer;
        }

        //clear our vectors from old pointers
        actionsLog.clear();
        volunteers.clear();
        pendingOrders.clear();
        inProcessOrders.clear();
        completedOrders.clear();
        customers.clear();

        //steal from other:

        isOpen =other.isOpen;
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        orderCounter =other.orderCounter;
        customers =other.customers;
        completedOrders =other.completedOrders;
        inProcessOrders =other.inProcessOrders;
        pendingOrders =other.pendingOrders;
        volunteers =other.volunteers;
        actionsLog =other.actionsLog;

        for(BaseAction* action : other.actionsLog){
            BaseAction* p = action;
            actionsLog.push_back(p);
            action = nullptr;
        }
        for(Volunteer* v : other.volunteers){
            Volunteer* p = v;
            volunteers.push_back(p);
            v = nullptr;
        }
        for(Order* o  : other.pendingOrders){
            Order* p = o;
            pendingOrders.push_back(p);
            o = nullptr;
        }
        for(Order* o : other.inProcessOrders){
            Order* p = o;
            inProcessOrders.push_back(p);
            o = nullptr;
        }
        for(Order* o  : other.completedOrders){
            Order* p = o;
            completedOrders.push_back(p);
            o = nullptr;
        }
        for(Customer * c : other.customers){
            Customer* p = c;
            customers.push_back(p);
            c = nullptr;
        }
    }
    return *this;
}



//void WareHouse::addCustomer(const std::string &customerName, const std::string &customerType, int distance, int maxOrders)
//{

//int customerId = customerCounter;

 //   if (customerType == "Civilian") {
   //     customers.push_back(new CivilianCustomer(customerId, customerName, distance, maxOrders));
    //} else if (customerType == "Soldier") {
      //  customers.push_back(new SoldierCustomer(customerId, customerName, distance, maxOrders));
    //}

    //customerCounter++;
//}

void WareHouse::addOrder(Order *order)
{
    pendingOrders.push_back(order);
}

void WareHouse::addAction(BaseAction *action)
{
    actionsLog.push_back(action);
}

void WareHouse::addVolunteer(Volunteer *volunteer)
{
    volunteers.push_back(volunteer);
}


Customer &WareHouse::getCustomer(int customerId) const {
   for(Customer *c: customers) {
        if (c->getId() == customerId)
        {
           return *c;
        }
    }
    ///CivilianCustomer noCustomer = CivilianCustomer(-1,"",0,0);
    //return noCustomer;
    // Throw an exception if customer is not found
    //throw std::out_of_range("Customer not found");
    throw std::invalid_argument("Customer not found");

}

Volunteer &WareHouse::getVolunteer(int volunteerId) const {
      for(Volunteer *v: volunteers) {
           if (v->getId() == volunteerId)
           {
             return *v;
           }
   
        }
     //CollectorVolunteer noVolunteer = CollectorVolunteer(-1, "", 0);
     //return noVolunteer;
    throw std::invalid_argument("Volunteer not found");


}


Order &WareHouse::getOrder(int orderId) const {
    for (Order *o:pendingOrders) {
        if (o->getId() == orderId) {
            return *o;
        }
    }

    for (Order *o:inProcessOrders) {
        if (o->getId() == orderId) {
            return *o;
        }
    }

    for (Order *o:completedOrders) {
        if (o->getId() == orderId) {
            return *o;
        }
    }
        throw std::out_of_range("Order not found");

}

const vector<BaseAction *> &WareHouse::getActions() const
{
    return actionsLog;
}

vector<Order*> &WareHouse::getPendingOrders()
{
    return pendingOrders;
}

vector<Order*> &WareHouse::getInProcessOrders()
{
    return inProcessOrders;
}



vector<Order *> &WareHouse::getCompleted()
{
    return completedOrders;
}

vector<Volunteer*> &WareHouse::getVolunteers() 
{
    return volunteers;
}

bool WareHouse::isOrderExist(int orderId) const
{
    for(Order *order : pendingOrders)
    {
        if(order->getId()== orderId)
        {
            return true;
        }
    }
    for(Order *order : inProcessOrders)
    {
        if(order->getId()== orderId)
        {
            return true;
        }
    }
    for(Order *order : completedOrders)
    {
        if(order->getId()== orderId)
        {
            return true;
        }
    }
    return false;
}


bool WareHouse::isCustomerExist(int customerId) const
{
    return (customerId < customerCounter);
}

bool WareHouse::isVolunteerExist(int volunteerId) const
{
    for (Volunteer *v : volunteers)
    {
         if (v->getId() > volunteerId) {
            return false;
        }     
        if (v->getId() == volunteerId) {
            return true;
        }
    }
    return false;
}

int WareHouse::newOrderId()
{
    int id = orderCounter;
    orderCounter++;
    return id;
}

int WareHouse::newVolunteerId()
{
    int id = volunteerCounter;
    volunteerCounter++;
    return id;
}

int WareHouse::newCustomerId()
{
    int id = customerCounter;
    customerCounter++;
    return id;
}
