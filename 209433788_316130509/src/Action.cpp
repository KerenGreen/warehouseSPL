#include "../include/Action.h"
#include "../include/WareHouse.h"
#include <string>
#include "Action.h"


BaseAction::BaseAction():
   errorMsg(""), status(ActionStatus::ERROR)
{
}

ActionStatus BaseAction::getStatus() const
{
    return ActionStatus();
}
string BaseAction::getStatusString() const 
{
    return status == ActionStatus::COMPLETED ? " COMPLETED" : " ERROR";
}

void BaseAction::complete()
{
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg)
{
    this->errorMsg = errorMsg;
    status = ActionStatus::ERROR;
    std::cout << "Error : " << getErrorMsg() << std::endl;
}

string BaseAction::getErrorMsg() const
{
    return errorMsg;
}

string BaseAction::actionStatusToString(ActionStatus status) const{
    string res = "";
    if(status == ActionStatus::ERROR)
        res = "ERROR";
    else res = "COMPLETED";
    return res;
}

/////////////////////SIMULATE STEP//////////////////
SimulateStep::SimulateStep(int numOfSteps): 
   numOfSteps(numOfSteps)
{
}


void SimulateStep::act(WareHouse &wareHouse)
{
    vector<Order*> &pendingOrd = wareHouse.getPendingOrders();
    vector<Order*> &inProcessOrd = wareHouse.getInProcessOrders();
    vector<Order*> &completedOrd = wareHouse.getCompleted();
    vector<Volunteer*> &volunteer = wareHouse.getVolunteers();
    
    for(int i = 0; i < numOfSteps; i++){ //KEREN changed from <= to < so it won't be out of bound

      for( size_t j = 0; j < pendingOrd.size(); j++)  //Iterate through pending orders
      {
        Order& po = *(pendingOrd[j]);
        
        if (po.getStatus() == OrderStatus::PENDING) { // Check order status and assign to the appropriate volunteer
           
            Volunteer *collector = findAvailableCollector(volunteer, po);  // Find an available Collector
            if (collector != nullptr) {
                collector->acceptOrder(po);
                po.setCollectorId(collector->getId());
                po.setStatus(OrderStatus::COLLECTING);
                inProcessOrd.push_back(&po);
                pendingOrd.erase(pendingOrd.begin() + j);
                j--;
                break;
            }
        } else if (po.getStatus() == OrderStatus::COLLECTING) {
            
            Volunteer *driver = findAvailableDriver(volunteer,po); // Find an available Driver
            if (driver != nullptr) {
                driver->acceptOrder(po);
                po.setDriverId(driver->getId());
                po.setStatus(OrderStatus::DELIVERING);
                pendingOrd.erase(std::next(pendingOrd.begin(),j));
                inProcessOrd.push_back(&po);
                j--;
                continue;
            }
        }
        
      }

     //2
     //for(Volunteer* v : volunteer)
     for (auto it = volunteer.begin(); it!= volunteer.end(); ++it){  
        Volunteer* v = *it;
        v->step();
        if(v->getActiveOrderId() == NO_ORDER) //done
        {
            for(size_t o = 0; o < inProcessOrd.size(); o++)
            {
                Order* order = inProcessOrd[o];
                int volunteerOrd = v->getCompletedOrderId();
                if(order->getId()== volunteerOrd)
                {
                    if(order->getStatus() == OrderStatus::COLLECTING)
                    {
                     inProcessOrd.erase(std::next(inProcessOrd.begin(),o));
                     pendingOrd.push_back(order);
                    }
                    else if(order->getStatus() == OrderStatus::DELIVERING)
                    {
                        inProcessOrd.erase(std::next(inProcessOrd.begin(),o));
                        completedOrd.push_back(order);
                        order->setStatus(OrderStatus::COMPLETED);
                    }
                }
            }
  
        }
        //4
        if(!(v->hasOrdersLeft()) && v->getActiveOrderId() == NO_ORDER)
        {
            volunteer.erase(it);
            delete v;
        }

     }
    

    }
    complete();
}

// Helper function to find an available CollectorVolunteer
Volunteer *SimulateStep::findAvailableCollector(vector<Volunteer *> &volunteers, Order &order)
{
    for (Volunteer *collector : volunteers) {
        if (collector->type() == "collector" || collector->type() == "limitedCollector") {
            if (collector->canTakeOrder(order)) {
                return collector;
            }
        }
    }
    return nullptr;
}

// Helper function to find an available DriverVolunteer
Volunteer *SimulateStep::findAvailableDriver(vector<Volunteer *> &volunteers, Order &order)
{
    for (Volunteer *driver : volunteers) {
        if (driver->type() == "driver" || driver->type() == "limitedDriver") {
            if (driver->canTakeOrder(order)) {
                return driver;
            }
        }
    }
    return nullptr;
    
}

string SimulateStep::toString() const{
    return "step " + std::to_string(numOfSteps) + " "+ getStatusString();
}

SimulateStep * SimulateStep::clone() const{
    return new SimulateStep(*this);
}

/// add customer///
AddCustomer::AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders) :
 customerName(customerName), customerType(whichCustomerType(customerType)), distance(distance), maxOrders(maxOrders) {}

CustomerType AddCustomer::whichCustomerType(const string &customerType)
{
    if(customerType == "soldier")
        return CustomerType::Soldier;
    return CustomerType::Civilian;
}

void AddCustomer::act(WareHouse &wareHouse){
    int customerId = wareHouse.newCustomerId();
    Customer* customer;
    switch((int)customerType){
        case 0:
            customer = new SoldierCustomer(customerId, customerName, distance, maxOrders);
            break;
        case 1:
            customer = new CivilianCustomer(customerId, customerName, distance, maxOrders);
            break;
    }
    wareHouse.addCustomer(customer);
}

AddCustomer* AddCustomer::clone() const { return new AddCustomer(*this);}
string AddCustomer::toString() const {
    return "customer " + customerName + " " + typeCustomerToString() + " " + std::to_string(distance) + " " + std::to_string(maxOrders) + " " + actionStatusToString(getStatus());
}

string AddCustomer::typeCustomerToString() const{
    if (customerType == CustomerType::Soldier)
        return "soldier";
    return "civilian";
}


//////////////// PRINT ORDER STATUS//////////////////
PrintOrderStatus::PrintOrderStatus(int id): 
     orderId(id)
{
}

void PrintOrderStatus::act(WareHouse& warehouse){
    if(!warehouse.isOrderExist(orderId)){
        error("Order doesn't exist");
    }else{
        Order& order = warehouse.getOrder(orderId);
        std::cout << "OrderId: " << order.getId() << std::endl;

        switch ((int)order.getStatus())
        {
        case 0:
            std::cout << "OrderStatus: Pending" << std::endl;
            break;
        case 1:
            std::cout << "OrderStatus: Collecting" << std::endl;
            break;
        case 2:
            std::cout << "OrderStatus: Delivering" << std::endl;
            break;
        case 3:
            std::cout << "OrderStatus: Completed" << std::endl;
            break;
        default:
            break;
        }
        std::cout << "CustomerID: " << order.getCustomerId() << std::endl;
        if(order.getCollectorId() == -1)
            {std::cout << "Collector: " << "None" << std::endl;}
        else
            {std::cout << "Collector: " << order.getCollectorId() << std::endl;}
        if(order.getDriverId() == -1)
            {std::cout << "Driver: " << "None" << std::endl;}
        else
            {std::cout << "Driver: " << order.getDriverId() << std::endl;}

    }
}


PrintOrderStatus *PrintOrderStatus::clone() const
{
    return new PrintOrderStatus(*this);
}

string PrintOrderStatus::toString() const
{
     string orderStatusToString = "orderStatus " ;
     orderStatusToString += std::to_string(orderId);
     orderStatusToString += " " + actionStatusToString(getStatus());
     return orderStatusToString;
}

////////////PRINT COSTUMER STATUS//////////////////
PrintCustomerStatus::PrintCustomerStatus(int customerId)
 : customerId(customerId) {}

PrintCustomerStatus* PrintCustomerStatus::clone() const
 { 
    return new PrintCustomerStatus(*this);
 }
string PrintCustomerStatus::toString() const 
{
     return "customerStatus " + std::to_string(customerId) + getStatusString();
}

void PrintCustomerStatus::act(WareHouse &wareHouse) {
    if (!wareHouse.isCustomerExist(customerId)){
        error("Customer does not exist");
        std::cout << getErrorMsg() << std::endl;
    }
    else {
        Customer& customer = wareHouse.getCustomer(customerId);
        std::cout << "CustomerID: " + std::to_string(customerId) << std::endl;
        for (int orderId : customer.getOrdersIds()) {
            std::cout << "OrderID: " + std::to_string(orderId) << std::endl;
            std::cout << "OrderStatus: " + wareHouse.getOrder(orderId).statusToString() << std::endl;
        }
        std::cout << "numOrdersLeft: " + std::to_string(customer.getMaxOrders()-customer.getNumOrders()) << std::endl;
        complete();
    }
}

////////////////////PRINT VOLUNTEER STATUS/////////////////////

PrintVolunteerStatus::PrintVolunteerStatus(int id) : volunteerId(id) {}
PrintVolunteerStatus* PrintVolunteerStatus::clone() const { return new PrintVolunteerStatus(*this);}
string PrintVolunteerStatus::toString() const { return "volunteerStatus " + std::to_string(volunteerId) + getStatusString();}

void PrintVolunteerStatus::act(WareHouse &wareHouse) {
    
    if (wareHouse.isVolunteerExist(volunteerId)){
        Volunteer& volunteer = wareHouse.getVolunteer(volunteerId);
        std::cout << volunteer.toString() << std::endl;
        complete();
    }
    else{
        error("Volunteer does not exist");
        std::cout << getErrorMsg() << std::endl;
    }
}


/////// PRINT ACTIONS LOG/////
PrintActionsLog::PrintActionsLog()
{
}

void PrintActionsLog::act(WareHouse &wareHouse) {
    for(BaseAction* action : wareHouse.getActions()){
        std::cout << action->toString() << std::endl;
    }
    complete();
}

PrintActionsLog* PrintActionsLog::clone() const 
{ 
    return new PrintActionsLog(*this);
}

string PrintActionsLog::toString() const 
{ 
    return "log" + getStatusString();
}

///////// close////////
Close::Close() {}
Close* Close::clone() const { return new Close(*this);}
string Close::toString() const { return "close" + getStatusString();}

void Close::act(WareHouse &wareHouse) {
    wareHouse.close();
    complete();
}

extern WareHouse *backup;
BackupWareHouse::BackupWareHouse() : BaseAction() {}

BackupWareHouse *BackupWareHouse::clone() const
{
    return new BackupWareHouse(*this);
}

void BackupWareHouse::act(WareHouse &wareHouse)
{
    if (backup != nullptr)
        delete backup;
    backup = new WareHouse(wareHouse);
    complete();
}

string BackupWareHouse::toString() const { return "backup" + getStatusString();
}

RestoreWareHouse::RestoreWareHouse()  {}

RestoreWareHouse *RestoreWareHouse::clone() const
{
    return new RestoreWareHouse(*this);
}

string RestoreWareHouse::toString() const
{
    if (getStatus() == ActionStatus::ERROR)
        return "restore " + getErrorMsg() + " ERROR";
    return "restore COMPLETED";
}
void RestoreWareHouse::act(WareHouse &wareHouse)
{
   
    if (backup == nullptr){
   
        error("No backup Available");
        std::cout<<"ERROR: "<<getErrorMsg()<<std::endl;
    }
    else
    {
        wareHouse = *backup;
        complete();
    }
}

AddOrder::AddOrder(int id) : BaseAction::BaseAction(), customerId(id) {}


void AddOrder::act(WareHouse &wareHouse) {
    if(!wareHouse.isCustomerExist(customerId)){
        error("Cannot place this order");
    }
    else if(!wareHouse.getCustomer(customerId).canMakeOrder()){
        error("Cannot place this order");
    }else{
        int orderId = wareHouse.newOrderId();
        Customer& customer = wareHouse.getCustomer(customerId);
        int distance = customer.getCustomerDistance();
        Order* order = new Order(orderId, customerId, distance);
        if(customer.addOrder(orderId) == -1){
            error("Cannot place this order");
        }else{
            wareHouse.addOrder(order);
            complete();
        }
    }
}


string AddOrder::toString() const {
    return "order "+std::to_string(customerId) +" "+ getStatusString();
}

AddOrder *AddOrder::clone() const {
    return new AddOrder(*this);
}
