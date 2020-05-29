#include "provided.h"
#include <queue>
#include <vector>
using namespace std;

class DeliveryPlannerImpl
{
public:
    DeliveryPlannerImpl(const StreetMap* sm);
    ~DeliveryPlannerImpl();
    DeliveryResult generateDeliveryPlan(
                                        const GeoCoord& depot,
                                        const vector<DeliveryRequest>& deliveries,
                                        vector<DeliveryCommand>& commands,
                                        double& totalDistanceTravelled) const;
private:
    const StreetMap* streetMap;
};

DeliveryPlannerImpl::DeliveryPlannerImpl(const StreetMap* sm)
{
    streetMap = sm;
}

DeliveryPlannerImpl::~DeliveryPlannerImpl()
{
}

DeliveryResult DeliveryPlannerImpl::generateDeliveryPlan(
                                                         const GeoCoord& depot,
                                                         const vector<DeliveryRequest>& deliveries,
                                                         vector<DeliveryCommand>& commands,
                                                         double& totalDistanceTravelled) const
{
    PointToPointRouter ptp(streetMap);
    DeliveryOptimizer dp(streetMap);
    vector<DeliveryRequest> newDeliveries(deliveries);
    double oldD,newD;
    dp.optimizeDeliveryOrder(depot, newDeliveries, oldD, newD);
    
    GeoCoord pre = depot;
    string preStreetName;
    for (int i = 0; i< newDeliveries.size();i++){
        double totalDis;
        list<StreetSegment> currRoute;
        ptp.generatePointToPointRoute(pre, newDeliveries[i].location, currRoute, totalDis);
        if (currRoute.size()==0) return NO_ROUTE;
        list<StreetSegment>::iterator it;
        list<StreetSegment>::iterator p;
        list<StreetSegment>::iterator last = currRoute.begin();
        double toNext=distanceEarthMiles(last->end, last->start);
        for (it = currRoute.begin();it!=currRoute.end();it++){
            if (it == currRoute.begin())continue;
            
            if (it->name!=p->name){
                
                DeliveryCommand cd;
                cd.initAsProceedCommand(to_string(angleOfLine(*last)), last->name, toNext);
                commands.push_back(cd);
                DeliveryCommand t;
                t.initAsTurnCommand(to_string(angleOfLine(*it)), it->name);
                commands.push_back(t);
                last = it;
                toNext=0;
            }else{
                toNext+=distanceEarthMiles(it->end, it->start);
            }
            p = it;
            
        }
        DeliveryCommand cd;
        cd.initAsProceedCommand(to_string(angleOfLine(*last)), last->name, distanceEarthMiles(last->start, it->start));
        commands.push_back(cd);
        
        DeliveryCommand deliv;
        deliv.initAsDeliverCommand(newDeliveries[i].item);
    }
    
    
    
    
    return DELIVERY_SUCCESS;  // Delete this line and implement this function correctly
}

//******************** DeliveryPlanner functions ******************************

// These functions simply delegate to DeliveryPlannerImpl's functions.
// You probably don't want to change any of this code.

DeliveryPlanner::DeliveryPlanner(const StreetMap* sm)
{
    m_impl = new DeliveryPlannerImpl(sm);
}

DeliveryPlanner::~DeliveryPlanner()
{
    delete m_impl;
}

DeliveryResult DeliveryPlanner::generateDeliveryPlan(
                                                     const GeoCoord& depot,
                                                     const vector<DeliveryRequest>& deliveries,
                                                     vector<DeliveryCommand>& commands,
                                                     double& totalDistanceTravelled) const
{
    return m_impl->generateDeliveryPlan(depot, deliveries, commands, totalDistanceTravelled);
}
