#include "router.h"
#include "geotools.h"
#include "geopoint.h"
#include "HashMap.h"
#include <queue>
#include <algorithm>
using namespace std;

// Helper struct to represent a state in the BFS traversal
struct State {

    GeoPoint point; // Current GeoPoint
    double distance; // Distance traveled so far

    // Constructor
    State(const GeoPoint& p, double d) : point(p), distance(d) {}

    // Operator overloading for priority comparison in the priority queue
    bool operator<(const State& other) const {
        // Priority based on distance traveled so far (shortest distance first)
        return distance > other.distance;
    }
};

std::vector<GeoPoint> Router::route(const GeoPoint& pt1, const GeoPoint& pt2) const {

    std::vector<GeoPoint> path;

    if (pt1.to_string() == pt2.to_string()) {
        path.push_back(pt1);
        return path;
    }

    std::priority_queue<State> pq;
    pq.push(State(pt1, 0.0));

    HashMap<GeoPoint> visited;
    visited.insert(pt1.to_string(), pt1);

    HashMap<GeoPoint> parent;
    parent.insert(pt1.to_string(), GeoPoint());

    while (!pq.empty()) {
        State current = pq.top();
        pq.pop();

        // If destination reached, reconstruct and return the path
        if (current.point.to_string() == pt2.to_string()) {
            GeoPoint p = current.point;
            while (p.to_string() != pt1.to_string()) {
                path.push_back(p);
                p = *parent.find(p.to_string());
            }
            path.push_back(pt1);
            std::reverse(path.begin(), path.end());
            return path;
        }

        // Iterate through connected GeoPoints
        for (const GeoPoint& next : m_geodb.get_connected_points(current.point)) {

            double new_distance = current.distance + distance_earth_km(current.point, next);

            // If next GeoPoint not visited or new distance shorter, update priority queue and parent
            if (visited.find(next.to_string()) == nullptr || new_distance < current.distance) {
                pq.push(State(next, new_distance));
                visited.insert(next.to_string(), next);
                parent.insert(next.to_string(), current.point);
            }
        }
    }
    // No path found
    return path;
}
