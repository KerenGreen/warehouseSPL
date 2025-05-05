#include "../include/Volunteer.h"

Volunteer::Volunteer(int id, const string &name):
    completedOrderId(NO_ORDER),
    activeOrderId(NO_ORDER),
    id(id), name(name)
    
{
}


int Volunteer::getId() const
{
    return id;
}

const string &Volunteer::getName() const
{
    return name;
}

int Volunteer::getActiveOrderId() const
{

    return activeOrderId;
}

int Volunteer::getCompletedOrderId() const
{
   
    return completedOrderId;
}

bool Volunteer::isBusy() const
{
    return (activeOrderId != -1);
}


//collectorVolunteer
CollectorVolunteer::CollectorVolunteer(int id, const string &name, int coolDown): 
   Volunteer(id,name), coolDown(coolDown), timeLeft(coolDown)
{
}


CollectorVolunteer *CollectorVolunteer::clone() const
{
    return new CollectorVolunteer(*this);
}

void CollectorVolunteer::step()
{
    if(isBusy()){
        if(timeLeft > 0){
            bool done = decreaseCoolDown();
            timeLeft = std::max(0, timeLeft);
            if(done){
                completedOrderId = activeOrderId;
                activeOrderId = NO_ORDER;
            }
        }
    }
}

int CollectorVolunteer::getCoolDown() const
{
    return coolDown;
}

int CollectorVolunteer::getTimeLeft() const
{
    return timeLeft;
}

bool CollectorVolunteer::decreaseCoolDown()
{
    timeLeft = timeLeft -1;
    if(timeLeft == 0){
        return true;
    }
    return false;
}

bool CollectorVolunteer::hasOrdersLeft() const
{
    return true;
}

bool CollectorVolunteer::canTakeOrder(const Order &order) const
{
    return !isBusy();
}

void CollectorVolunteer::acceptOrder(const Order &order)
{
    activeOrderId = order.getId();
    timeLeft = coolDown;
}

string CollectorVolunteer::toString() const
{
    string activeOrderIdString = (activeOrderId == -1) ? "None" : std::to_string(activeOrderId);
    string timeLeftSring = (getTimeLeft() <= 0) ? "None" : std::to_string(getTimeLeft());
    string isbusyString = (Volunteer::isBusy()) ? "True" : "False";
    return "VolunteerID: " + std::to_string(getId()) + "\n"
         + "isBusy:" + isbusyString + "\n"
         + "OrderID: " + activeOrderIdString + "\n"
         + "TimeLeft: " + timeLeftSring + "\n"
         + "OrdersLeft: No Limit";
}

string CollectorVolunteer::type() const
{
    return "collector";
}


//limitedCollectorVolunteer

LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, const string &name, int coolDown, int maxOrders) :
  CollectorVolunteer(id,name,coolDown), maxOrders(maxOrders), ordersLeft(maxOrders)
{
}

LimitedCollectorVolunteer *LimitedCollectorVolunteer::clone() const
{
    return new LimitedCollectorVolunteer(*this);
}

bool LimitedCollectorVolunteer::hasOrdersLeft() const
{
    if(maxOrders > 0){
        return true;
    }
    return false;
}

bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const
{
    return CollectorVolunteer::canTakeOrder(order) && hasOrdersLeft();
}

void LimitedCollectorVolunteer::acceptOrder(const Order &order)
{
    CollectorVolunteer::acceptOrder(order);
    ordersLeft = ordersLeft - 1;
}


int LimitedCollectorVolunteer::getMaxOrders() const
{
    return maxOrders;
}

int LimitedCollectorVolunteer::getNumOrdersLeft() const
{
    return ordersLeft;
}

string LimitedCollectorVolunteer::toString() const
{
    string activeOrderIdString = (activeOrderId == -1) ? "None" : std::to_string(activeOrderId);
    string timeLeftString = (getTimeLeft() <= 0) ? "None" : std::to_string(getTimeLeft());
    string isbusyStgring = (Volunteer::isBusy()) ? "True" : "False";
    return "VolunteerID: " + std::to_string(getId()) + "\n"
         + "isBusy:" + isbusyStgring + "\n"
         + "OrderID: " + activeOrderIdString + "\n"
         + "TimeLeft: " + timeLeftString + "\n"
         + "OrdersLeft: " + std::to_string(ordersLeft);
}

string LimitedCollectorVolunteer::type() const
{
    return "limitedCollector";
}

// Driver volunteer
DriverVolunteer::DriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep):
 Volunteer(id,name), maxDistance(maxDistance), distancePerStep(distancePerStep), distanceLeft(NO_ORDER)
{
}

DriverVolunteer *DriverVolunteer::clone() const
{
    return new DriverVolunteer (*this);
}

int DriverVolunteer::getDistanceLeft() const
{
    return distanceLeft;
}

int DriverVolunteer::getMaxDistance() const
{
    return maxDistance;
}

int DriverVolunteer::getDistancePerStep() const
{
    return distancePerStep;
}

bool DriverVolunteer::decreaseDistanceLeft()
{
    distanceLeft = distanceLeft - distancePerStep;
    if (distanceLeft == 0)
    {
    return true;
    }
    return false;
}

bool DriverVolunteer::hasOrdersLeft() const
{
    return true;
}

bool DriverVolunteer::canTakeOrder(const Order &order) const
{
    if (!(isBusy()))
    {
        if(order.getDistance() <= maxDistance)
        {
            return true;
        }
    }
    return false;
}

void DriverVolunteer::acceptOrder(const Order &order)
{
    activeOrderId = order.getId();
    distanceLeft = order.getDistance();
}

void DriverVolunteer::step()
{
    if(isBusy()){
        if(distanceLeft > 0){
            bool done = decreaseDistanceLeft();
            distanceLeft = std::max(0, distanceLeft);
            if(done){
                completedOrderId = activeOrderId;
                activeOrderId = NO_ORDER;
            }
        }
    }
}

string DriverVolunteer::toString() const
{
    string activeOrderIdString = (activeOrderId == -1) ? "None" : std::to_string(activeOrderId);
    string distanceLeftString = (getDistanceLeft() <= 0) ? "None" : std::to_string(getDistanceLeft());
    string isbusyString = (Volunteer::isBusy()) ? "True" : "False";
    return "VolunteerID: " + std::to_string(getId()) + "\n"
         + "isBusy:" + isbusyString + "\n"
         + "OrderID: " + activeOrderIdString + "\n"
         + "DistanceLeft: " + distanceLeftString + "\n"
         + "OrdersLeft: No Limit";
}

string DriverVolunteer::type() const
{
    return "driver";
}

/////////////////// limitedDriverVolunteer /////////////////////
LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep, int maxOrders):
   DriverVolunteer(id,name,maxDistance,distancePerStep), maxOrders(maxOrders), ordersLeft(maxOrders)
{
}

LimitedDriverVolunteer *LimitedDriverVolunteer::clone() const
{
    return new LimitedDriverVolunteer(*this);
}

int LimitedDriverVolunteer::getMaxOrders() const
{
    return maxOrders;
}

int LimitedDriverVolunteer::getNumOrdersLeft() const
{
    return ordersLeft;
}

bool LimitedDriverVolunteer::hasOrdersLeft() const
{
    if(ordersLeft > 0)
    {
        return true;
    }
    return false;
}


bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const
{
    if(!(isBusy()))
    {
        if(hasOrdersLeft()){
            if(canTakeOrder(order)){
                return true;
            }
        }
    }
    return false;
}

void LimitedDriverVolunteer::acceptOrder(const Order &order)
{
    DriverVolunteer::acceptOrder(order);
    ordersLeft = ordersLeft - 1;
}

string LimitedDriverVolunteer::toString() const
{
    string activeOrderIdString = (activeOrderId == -1) ? "None" : std::to_string(activeOrderId);
    string distanceLeftString = (getDistanceLeft() <= 0) ? "None" : std::to_string(getDistanceLeft());
    string isbusyString = (Volunteer::isBusy()) ? "True" : "False";
    return "VolunteerID: " + std::to_string(getId()) + "\n"
         + "isBusy:" + isbusyString + "\n"
         + "OrderID: " + activeOrderIdString + "\n"
         + "DistanceLeft: " + distanceLeftString + "\n"
         + "OrdersLeft: " + std::to_string(ordersLeft);


}

string LimitedDriverVolunteer::type() const
{
    return "limitedDriver";
}
