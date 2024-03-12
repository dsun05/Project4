//
//  GeoDatabase.hpp
//  Project4
//
//  Created by David Sun on 3/8/24.
//

#ifndef geodb_h
#define geodb_h

#include <string>
#include "base_classes.h"
#include "HashMap.h"

class GeoDatabase: public GeoDatabaseBase
{
public:
    GeoDatabase() : m_map(), m_poi(), m_midpoints(), m_connected_points() {};
    ~GeoDatabase();
    bool load(const std::string& map_data_file);
    bool get_poi_location(const std::string& poi,
    GeoPoint& point) const;
    
    std::vector<GeoPoint> get_connected_points(const GeoPoint& pt)
    const;
    std::string get_street_name(const GeoPoint& pt1,
    const GeoPoint& pt2) const;
private:
    void insertConnection(const std::string& street_name, const GeoPoint& p1, const GeoPoint& p2);
    void insertStreet(const std::string& street_name, const std::vector<GeoPoint>& points);

    struct Street
    {
		std::string name;
		std::vector<GeoPoint> points;
	};

    HashMap<Street> m_map;
    HashMap<GeoPoint> m_poi;
    HashMap<GeoPoint> m_midpoints;
    HashMap<std::vector<GeoPoint>> m_connected_points;
};


#endif /* geodb_h */
