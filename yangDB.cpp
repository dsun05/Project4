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
        //cout << "street: " << line << endl;
        //extract the street
        string street = line;
        //move onto next line
        getline(file, line);
        istringstream iss2(line);
        string lat1, lon1, lat2, lon2;
        //cout << "line: " << line << endl;

        iss2 >> lat1 >> lon1 >> lat2 >> lon2;
        //cout << lat1 << lon1 << lat2 << lon2 << endl;
        GeoPoint p1(lat1, lon1);
        GeoPoint p2(lat2, lon2);
        //extract the # of points of interests
        getline(file, line);
        istringstream iss3(line);
        int num_poi;
        iss3 >> num_poi;
        //cout << "num_poi: " << num_poi << endl;
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
        bool hasInsertedMidpoint = false;
        for (int i = 0; i < num_poi; i++)
        {
            getline(file, line);
            //cout << "line: " << line << endl;
            size_t pos = line.find('|');

            // Extract the name and coordinates substrings
            std::string poi = line.substr(0, pos);
            //cout << "poi: " << poi << endl;
            std::string coordinates = line.substr(pos + 1);
            istringstream iss4(coordinates);
            string lat, lon;
            iss4 >> lat >> lon;
            //cout << "lat: " << lat << " lon: " << lon << endl;
            GeoPoint p(lat, lon);
            POI.insert(poi, p);
            //update the connected_points hashmap with midpoint paths
            GeoPoint mp = midpoint(p1, p2);
            connected_points.insert(p.to_string(), vector<GeoPoint>{mp});
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
            else if (!hasInsertedMidpoint)
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
            else if (!hasInsertedMidpoint)
            {
                connected_points.find(p2.to_string())->push_back(mp);
                hasInsertedMidpoint = true;
            }

            //update the street_name hashmap
            street_name.insert(p1.to_string() + " " + mp.to_string(), street);
            street_name.insert(p2.to_string() + " " + mp.to_string(), street);
            street_name.insert(mp.to_string() + " " + p1.to_string(), street);
            street_name.insert(mp.to_string() + " " + p2.to_string(), street);
            street_name.insert(mp.to_string() + " " + p.to_string(), "a path");
            street_name.insert(p.to_string() + " " + mp.to_string(), "a path");
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