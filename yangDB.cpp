//
// Created by Yang Gao on 3/11/24.
//

#include "geodb.h"
#include <iostream> // needed for any I/O
#include <fstream>  // needed in addition to <iostream> for file I/O
#include <sstream>  // needed in addition to <iostream> for string stream I/O
#include "geotools.h"
#include "HashMap.h"

using namespace std;

GeoDatabase::GeoDatabase()
{
}

GeoDatabase::~GeoDatabase()
{
}

bool GeoDatabase::load(const std::string& map_data_file)
{
    ifstream file(map_data_file);
    if (!file)
    {
        return false;
    }
    string line;
    while (getline(file, line))
    {
        //extract the street
        istringstream iss(line);
        string street;
        iss >> street;
        //extract the geopoints
        getline(iss, line);
        istringstream iss2(line);
        string lat1, lon1, lat2, lon2;
        iss2 >> lat1 >> lon1 >> lat2 >> lon2;
        GeoPoint p1(lat1, lon1);
        GeoPoint p2(lat2, lon2);
        //extract the # of points of interests
        getline(iss2, line);
        istringstream iss3(line);
        int num_poi;
        iss3 >> num_poi;

        //update the connected_points hashmap
        //for the first point
        if (connected_points.find(p1.to_string()) == nullptr)
        {
            vector<GeoPoint> v;
            v.push_back(p2);
            connected_points.insert(p1.to_string(), v);
        }
        else
        {
            connected_points.find(p1.to_string())->push_back(p2);
        }
        //for the second point
        if (connected_points.find(p2.to_string()) == nullptr)
        {
            vector<GeoPoint> v;
            v.push_back(p1);
            connected_points.insert(p2.to_string(), v);
        }
        else
        {
            connected_points.find(p2.to_string())->push_back(p1);
        }

        //update the street_name hashmap
        street_name.insert(p1.to_string() + " " + p2.to_string(), street);
        street_name.insert(p2.to_string() + " " + p1.to_string(), street);

        //extract the points of interests and put them into the hashmap POI
        for (int i = 0; i < num_poi; i++)
        {
            getline(iss3, line);
            istringstream iss4(line);
            string lat, lon, poi;
            iss4 >> lat >> lon;
            getline(iss4, poi);
            GeoPoint p(lat, lon);
            POI.insert(poi, p);
            //update the connected_points hashmap with midpoint paths
            GeoPoint mp = midpoint(p1, p2);
            if (connected_points.find(mp.to_string()) == nullptr)
            {
                vector<GeoPoint> v;
                v.push_back(p1);
                v.push_back(p2);
                v.push_back(p);
                connected_points.insert(mp.to_string(), v);
            }
            else
            {
                //insert each POI as a connected point
                connected_points.find(mp.to_string())->push_back(p);
            }
            //update p1 and p2 to be connected to the midpoint
            //for p1
            if (connected_points.find(p1.to_string()) == nullptr)
            {
                vector<GeoPoint> v;
                v.push_back(mp);
                connected_points.insert(p1.to_string(), v);
            }
            else
            {
                connected_points.find(p1.to_string())->push_back(mp);
            }
            //for p2
            if (connected_points.find(p2.to_string()) == nullptr)
            {
                vector<GeoPoint> v;
                v.push_back(mp);
                connected_points.insert(p2.to_string(), v);
            }
            else
            {
                connected_points.find(p2.to_string())->push_back(mp);
            }

            //update the street_name hashmap
            street_name.insert(p1.to_string() + " " + mp.to_string(), street);
            street_name.insert(p2.to_string() + " " + mp.to_string(), street);
            street_name.insert(mp.to_string() + " " + p1.to_string(), street);
            street_name.insert(mp.to_string() + " " + p2.to_string(), street);
        }
    }
    return true;
}

bool GeoDatabase::get_poi_location(const std::string& poi, GeoPoint& point) const
{
    if (POI.find(poi) == nullptr)
    {
        return false;
    }
    point = *POI.find(poi);
    return true;
}

std::vector<GeoPoint> GeoDatabase::get_connected_points(const GeoPoint& pt) const
{
    if (connected_points.find(pt.to_string()) == nullptr)
    {
        return {};
    }
    return *connected_points.find(pt.to_string());
}

std::string GeoDatabase::get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const
{
    if (street_name.find(pt1.to_string() + " " + pt2.to_string()) == nullptr)
    {
        return "";
    }
    return *street_name.find(pt1.to_string() + " " + pt2.to_string());
}