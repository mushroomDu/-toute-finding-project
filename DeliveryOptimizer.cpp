#include "provided.h"
#include <vector>
using namespace std;

class DeliveryOptimizerImpl
{
public:
    DeliveryOptimizerImpl(const StreetMap* sm);
    ~DeliveryOptimizerImpl();
    void optimizeDeliveryOrder(
                               const GeoCoord& depot,
                               vector<DeliveryRequest>& deliveries,
                               double& oldCrowDistance,
                               double& newCrowDistance) const;
private:
    const StreetMap* sm;
};

DeliveryOptimizerImpl::DeliveryOptimizerImpl(const StreetMap* sm)
{
    this->sm=sm;
}

DeliveryOptimizerImpl::~DeliveryOptimizerImpl()
{
}

void DeliveryOptimizerImpl::optimizeDeliveryOrder(
                                                  const GeoCoord& depot,
                                                  vector<DeliveryRequest>& deliveries,
                                                  double& oldCrowDistance,
                                                  double& newCrowDistance) const
{
    oldCrowDistance = 0;
    GeoCoord pre = depot;
    for (int i = 0; i<deliveries.size();i++){
        oldCrowDistance+=distanceEarthMiles(pre,deliveries[i].location);
        pre=deliveries[i].location;
    }
    oldCrowDistance+=distanceEarthMiles(pre,depot);
    newCrowDistance=oldCrowDistance;
}

//******************** DeliveryOptimizer functions ****************************

// These functions simply delegate to DeliveryOptimizerImpl's functions.
// You probably don't want to change any of this code.

DeliveryOptimizer::DeliveryOptimizer(const StreetMap* sm)
{
    m_impl = new DeliveryOptimizerImpl(sm);
}

DeliveryOptimizer::~DeliveryOptimizer()
{
    delete m_impl;
}

void DeliveryOptimizer::optimizeDeliveryOrder(
                                              const GeoCoord& depot,
                                              vector<DeliveryRequest>& deliveries,
                                              double& oldCrowDistance,
                                              double& newCrowDistance) const
{
    return m_impl->optimizeDeliveryOrder(depot, deliveries, oldCrowDistance, newCrowDistance);
}

