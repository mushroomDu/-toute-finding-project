#include "ExpandableHashMap.h"
#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
//#include <functional>
using namespace std;

unsigned int hasher(const GeoCoord& g)
{
    return std::hash<string>()(g.latitudeText + g.longitudeText);
}
unsigned int hasher(const int& in){
    return in;
}

class StreetMapImpl
{
public:
    StreetMapImpl();
    ~StreetMapImpl();
    bool load(string mapFile);
    bool getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const;
private:
    ExpandableHashMap<GeoCoord, vector<int>>* adjList;
    vector<StreetSegment*>* segs;
    
};
StreetMapImpl::StreetMapImpl()
{
    adjList = new ExpandableHashMap<GeoCoord, vector<int>>(0.2);
    segs = new vector<StreetSegment*>;
}

StreetMapImpl::~StreetMapImpl()
{
    delete adjList;
    for (int i = 0; i< segs->size();i++){
        delete segs->at(i);
    }
    delete segs;
    
}
bool StreetMapImpl::load(string mapFile)
{
    std::ifstream f(mapFile);
    string curr;
    getline(f, curr);
    
    if (curr =="") return false;
    while(curr !=""){
        string name = curr;
        getline(f, curr);
        string numSeg=curr;
        getline(f, curr);
        for (int i = 0; i<stoi(numSeg);i++){
            vector<string> locs;
            int slow = 0;
            for (int j = 0; j<=curr.length();j++){
                if (j==curr.length()||curr[j]==' '){
                    locs.push_back(curr.substr(slow,j-slow));
                    slow = j+1;
                }
            }
            string stLa=locs[0],stLo=locs[1],edLa=locs[2],edLo=locs[3];
            GeoCoord start(stLa,stLo),end(edLa,edLo);
            segs->push_back(new StreetSegment(start,end,name));
            segs->push_back(new StreetSegment(end,start,name));
            
            
            if ((adjList->find(end)==nullptr)){
                vector<int> tempVec;
                adjList->associate(end, tempVec);
            }
            (adjList->find(end))->push_back(segs->size()-1);
            if ((adjList->find(start)==nullptr)){
                vector<int> tempVec;
                adjList->associate(start, tempVec);
            }
            (adjList->find(start))->push_back(segs->size()-2);
            getline(f, curr);
        }
    }
    return true;
}

bool StreetMapImpl::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
    if (adjList->find(gc)==nullptr) return false;
    segs.clear();//clear what's orininally in the segs
    for(int i = 0; i< (adjList->find(gc))->size(); i++){
        segs.push_back(*(this->segs->at((adjList->find(gc))->at(i))));
    }
    return true;
}

//******************** StreetMap functions ************************************
//
// These functions simply delegate to StreetMapImpl's functions.
// You probably don't want to change any of this code.

StreetMap::StreetMap()
{
    m_impl = new StreetMapImpl;
}

StreetMap::~StreetMap()
{
    delete m_impl;
}

bool StreetMap::load(string mapFile)
{
    return m_impl->load(mapFile);
}

bool StreetMap::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
    return m_impl->getSegmentsThatStartWith(gc, segs);
}

