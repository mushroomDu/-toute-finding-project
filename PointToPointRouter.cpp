#include <list>
#include <queue>
#include "provided.h"
#include "ExpandableHashMap.h"
using namespace std;

class PointToPointRouterImpl
{
public:
    PointToPointRouterImpl(const StreetMap* sm);
    ~PointToPointRouterImpl();
    DeliveryResult generatePointToPointRoute(
                                             const GeoCoord& start,
                                             const GeoCoord& end,
                                             list<StreetSegment>& route,
                                             double& totalDistanceTravelled) const;
private:
    const StreetMap* streetMap;
};

PointToPointRouterImpl::PointToPointRouterImpl(const StreetMap* sm)
{
    streetMap = sm;
}

PointToPointRouterImpl::~PointToPointRouterImpl()
{
}

DeliveryResult PointToPointRouterImpl::generatePointToPointRoute(
                                                                 const GeoCoord& start,
                                                                 const GeoCoord& end,
                                                                 list<StreetSegment>& route,
                                                                 double& totalDistanceTravelled) const
{
    
    vector<StreetSegment> temp;
    if (!streetMap->getSegmentsThatStartWith(start, temp)||!streetMap->getSegmentsThatStartWith(end, temp)){
        return BAD_COORD;
    }
    if (end==start){
        route.clear();
        totalDistanceTravelled = 0;
        return DELIVERY_SUCCESS;
    }
    queue<GeoCoord> qe;
    ExpandableHashMap<GeoCoord, StreetSegment> pre;//if key is part of the path, what's before
    ExpandableHashMap<GeoCoord, int> visited;
    qe.push(start);
    visited.associate(start, 1);
    GeoCoord curr;
    while (!qe.empty()) {
        curr = qe.front();
        if(curr == end) break;
        qe.pop();
        streetMap->getSegmentsThatStartWith(curr, temp);
        for (int i = 0; i<temp.size();i++){
            if (visited.find(temp[i].end))continue;
            qe.push(temp[i].end);
            pre.associate(temp[i].end, temp[i]);
            visited.associate(temp[i].end, 1);
        }
    }
    if(curr != end) return NO_ROUTE;
    route.clear();
    totalDistanceTravelled=0;
    curr= end;
    while (curr!=start){
        StreetSegment* temp =pre.find(curr);
        totalDistanceTravelled+=distanceEarthMiles(temp->start,temp->end);
        route.push_front(*temp);
        curr=temp->start;
    }
    return DELIVERY_SUCCESS;
}

//******************** PointToPointRouter functions ***************************

// These functions simply delegate to PointToPointRouterImpl's functions.
// You probably don't want to change any of this code.

PointToPointRouter::PointToPointRouter(const StreetMap* sm)
{
    m_impl = new PointToPointRouterImpl(sm);
}

PointToPointRouter::~PointToPointRouter()
{
    delete m_impl;
}

DeliveryResult PointToPointRouter::generatePointToPointRoute(
                                                             const GeoCoord& start,
                                                             const GeoCoord& end,
                                                             list<StreetSegment>& route,
                                                             double& totalDistanceTravelled) const
{
    return m_impl->generatePointToPointRoute(start, end, route, totalDistanceTravelled);
}
