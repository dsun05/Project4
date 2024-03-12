//
//  Router.hpp
//  Project4
//
//  Created by David Sun on 3/8/24.
//

#ifndef router_h
#define router_h

#include "base_classes.h"
#include "HashMap.h"
#include <list>

class Router: public RouterBase
{
public:
    Router(const GeoDatabaseBase& geo_db) : m_geodb(geo_db) {};
    virtual ~Router() {delete &m_geodb;};
    virtual std::vector<GeoPoint> route(const GeoPoint& pt1,
    const GeoPoint& pt2) const;
private:

    struct Route 
    {
        double length;
        std::vector<GeoPoint> stops;
    };    

    const GeoDatabaseBase& m_geodb;
};


#endif /* router_h */
