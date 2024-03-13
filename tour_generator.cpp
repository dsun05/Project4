//
//  TourGenerator.cpp
//  Project4
//
//  Created by David Sun on 3/8/24.
//

#include "tour_generator.h"
#include "stops.h"
#include "tourcmd.h"
#include "HashMap.h"
#include <iostream>
#include <vector>
#include "geotools.h"
using namespace std;

struct stopParser {

    string m_poi;
	string m_comm;

    stopParser(const Stops& stop, int i)
    {
        stop.get_poi_data(i, m_poi, m_comm);
	}
};

string getDirection(double angle) {
	if (angle < 22.5) {
		return "east";
	}
	else if (angle < 67.5) {
		return "northeast";
	}
	else if (angle < 112.5) {
		return "north";
	}
	else if (angle < 157.5) {
		return "northwest";
	}
	else if (angle < 202.5) {
		return "west";
	}
	else if (angle < 247.5) {
		return "southwest";
	}
	else if (angle < 292.5) {
		return "south";
	}
	else if (angle < 337.5) {
		return "southeast";
	}
	else {
		return "east";
	}
}

std::vector<TourCommand> TourGenerator::generate_tour(const Stops& stops) const
{
    std::vector<TourCommand> tourCommands;

    for (int i = 0; i < stops.size(); i++) {

        stopParser currStop(stops, i);
        TourCommand commentary;
        commentary.init_commentary(currStop.m_poi, currStop.m_comm);
        tourCommands.push_back(commentary);

        //Not the last stop
        if (i + 1 < stops.size()) {

            stopParser nextStop(stops, i + 1);

            GeoPoint currPoint;
            GeoPoint nextPoint;
            m_geodb.get_poi_location(currStop.m_poi, currPoint);
            m_geodb.get_poi_location(nextStop.m_poi, nextPoint);

            vector<GeoPoint> path = m_router.route(currPoint, nextPoint);

            if (path.empty()) {
                return {};
            }

            for(int j = 0; j < path.size()-1; j++) {

                GeoPoint p1 = path[j];
                GeoPoint p2 = path[j + 1];

                string direction = getDirection(angle_of_line(p1, p2));

                TourCommand proceed;
                proceed.init_proceed(direction, m_geodb.get_street_name(p1, p2), distance_earth_miles(p1, p2), p1, p2);
                tourCommands.push_back(proceed);

                if (j + 2 < path.size()) {
                    GeoPoint p3 = path[j + 2];

                    //Conditions for turn command
                    if(m_geodb.get_street_name(p1, p2) == m_geodb.get_street_name(p2, p3) || direction == getDirection(angle_of_line(p2, p3))) {
						continue;
					}

                    double turnAngle = angle_of_turn(p1, p2, p3);
                    if (turnAngle < 180 && turnAngle > 0) {
                        direction = "left";
                    }else if (turnAngle >= 180 && turnAngle < 360) {
                        direction = "right";
					}

                    TourCommand turn;
					turn.init_turn(direction, m_geodb.get_street_name(p2, p3));
					tourCommands.push_back(turn);
                }
			}
        }
        else {
            return tourCommands;
        }
    }
    return tourCommands;
}
