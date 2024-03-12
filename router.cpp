//
//  Router.cpp
//  Project4
//
//  Created by David Sun on 3/8/24.
//

#include "router.h"
#include <list>
#include <queue>
#include "geotools.h"
using namespace std;

std::vector<GeoPoint> Router::route(const GeoPoint& pt1, const GeoPoint& pt2) const
{
	if(pt1.to_string() == pt2.to_string())
	{
		return;
	}

	//Get the distance between the two points
	double distance = distance_earth_km(pt1, pt2);

	std::vector<GeoPoint> m_visited;
	std::list<Route> m_routes;

	queue<GeoPoint> queue;

	for (auto it = m_geodb.get_connected_points(pt1).begin(); it != m_geodb.get_connected_points(pt1).end(); it++) 
	{
		//Create a new route for each connected point
		Route route;
		route.stops.push_back(*it);
		route.length = distance_earth_km(pt1, *it);
		m_routes.push_back(route);
	}

	for (auto it = m_routes.begin(); it != m_routes.end(); it++)
	{
		queue.push((*it).stops.back());

		while (!queue.empty()) {
			GeoPoint current = queue.front();
			queue.pop();

			if(current.to_string() == pt2.to_string())
			{
				break;
			}


			for (auto it = m_geodb.get_connected_points(current).begin(); it != m_geodb.get_connected_points(current).end(); it++) 
			{
				Route newRoute;
				newRoute.stops.push_back(*it);
				newRoute.length = distance_earth_km(current, *it);
				m_routes.push_back(newRoute);
				queue.push(*it);
			}
		}
	}




	return std::vector<GeoPoint>();
}
