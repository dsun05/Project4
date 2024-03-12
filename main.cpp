#include "HashMap.h"
#include "geopoint.h"
#include "geodb.h"
#include <iostream>
using namespace std;

int main() {
    GeoDatabase g;
    g.load("mapdata.txt");  // assume this works to avoid error checking 
    cerr<<"Loaded"<<endl;
    GeoPoint p1("34.0602175", "-118.4464952");
    GeoPoint p2("34.0600768", "-118.4467216");
    cout << g.get_street_name(p1, p2) << endl;
    cout << g.get_street_name(p2, p1) << endl;
}

void copy() {
    GeoDatabase g;
    GeoPoint p;
    if (g.get_poi_location("Diddy Riese", p))
        cout << "The PoI is at " << p.sLatitude << ", " << p.sLongitude << endl;
    else
        cout << "PoI not found!\n";


}