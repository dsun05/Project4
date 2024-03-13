//
//  GeoDatabase.cpp
//  Project4
//
//  Created by David Sun on 3/8/24.
//

#include "geodb.h"
#include "geotools.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

bool GeoDatabase::load(const std::string& map_data_file)
{
	std::ifstream infile((map_data_file).c_str());
	if (!infile) {
		cerr<<"File not found"<<endl;
		return false;
	}

	string line;
	while (getline(infile, line)) {

		//Input the street name
		string streetName = line;

		//Get Coordinates
		string startLat, startLon, endLat, endLon;
		infile >> startLat >> startLon >> endLat >> endLon;

		// Create GeoPoints for start and end
		GeoPoint startPoint(startLat, startLon);
		GeoPoint endPoint(endLat, endLon);

		//Insert Street into all encompassing map, and connect the endpoints to each other
		insertStreet(streetName, { startPoint, endPoint });
		insertConnection(startPoint, endPoint);

		//Get number of vertices
		int numVertices = 0;
		infile >> numVertices;
		infile.ignore(10000, '\n');


		//Midpoint
		if (numVertices > 0) {

			//Add midpoint to the street map, connect with both points
			GeoPoint mid = midpoint(startPoint, endPoint);

			insertStreet(streetName, { startPoint, mid });
			insertStreet(streetName, { endPoint, mid });
			insertConnection(startPoint, mid);
			insertConnection(mid, endPoint);

			//Get Vertices

			stringstream ss(line);
			string segment;
			for (int i = 0; i < numVertices; i++) {
				getline(infile, line);

				std::stringstream ss(line);
				getline(ss, segment, '|');
				string vStreetName = segment;

				getline(ss, segment, '|');
				std::stringstream coords(segment);
				string vLat, vLon;
				coords >> vLat >> vLon;

				GeoPoint vPoint(vLat, vLon);

				insertStreet("a path", {vPoint, mid});
				m_poi.insert(vStreetName, vPoint);
				insertConnection(vPoint, mid);
			}
		}
	}
	infile.close();
	return true;
}

bool GeoDatabase::get_poi_location(const std::string& poi, GeoPoint& point) const
{
	if (m_poi.find(poi) == nullptr)
	{
		return false;
	}
	point = *m_poi.find(poi);
	return true;
}


std::vector<GeoPoint> GeoDatabase::get_connected_points(const GeoPoint& pt) const
{
	if (m_connected_points.find(pt.to_string()) == nullptr)
	{
		return {};
	}
	return *m_connected_points.find(pt.to_string());
}

std::string GeoDatabase::get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const
{
	string address = pt1.to_string() + "|" + pt2.to_string();
	if (m_map.find(address) == nullptr) {
		return "";
	}
	return m_map.find(address)->name;
}

void GeoDatabase::insertConnection(const GeoPoint& p1, const GeoPoint& p2)
{
	if(m_connected_points.find(p1.to_string()) == nullptr)
	{
		m_connected_points.insert(p1.to_string(), {p2});
	}
	else
	{
		m_connected_points.find(p1.to_string())->push_back(p2);
	}

	if (m_connected_points.find(p2.to_string()) == nullptr)
	{
		m_connected_points.insert(p2.to_string(), { p1 });
	}
	else
	{
		m_connected_points.find(p2.to_string())->push_back(p1);
	}
}

void GeoDatabase::insertStreet(const std::string& street_name, const std::vector<GeoPoint>& points)
{
	Street street = { street_name, points };
	string address = street.points[0].to_string() + "|" + street.points[1].to_string();
	m_map.insert(address, street);
	address = street.points[1].to_string() + "|" + street.points[0].to_string();
	m_map.insert(address, street);
}