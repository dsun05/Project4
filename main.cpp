#include <cstdio>
#include <iomanip>
#include <iostream>
#include <map>
#include <vector>

#include "geodb.h"
#include "router.h"
#include "stops.h"
#include "tourcmd.h"
#include "tour_generator.h"

using namespace std;

void print_tour(vector<TourCommand>& tcs)
{
    double total_dist = 0;
    std::string direction;
    double street_distance = 0;

    cout << "Starting tour...\n";

    for (size_t i = 0; i < tcs.size(); ++i)
    {
        if (tcs[i].get_command_type() == TourCommand::commentary)
        {
            cout << "Welcome to " << tcs[i].get_poi() << "!\n";
            cout << tcs[i].get_commentary() << "\n";
        }
        else if (tcs[i].get_command_type() == TourCommand::turn)
        {
            cout << "Take a " << tcs[i].get_direction() << " turn on " << tcs[i].get_street() << endl;
        }
        else if (tcs[i].get_command_type() == TourCommand::proceed)
        {
            total_dist += tcs[i].get_distance();
            if (direction.empty())
                direction = tcs[i].get_direction();
            street_distance += tcs[i].get_distance();
            if (i + 1 < tcs.size() && tcs[i + 1].get_command_type() == TourCommand::proceed
                && tcs[i + 1].get_street() == tcs[i].get_street() && tcs[i].get_street() != "a path")
            {
                continue;
            }

            cout << "Proceed " << std::fixed << std::setprecision(3) << street_distance << " miles " << direction << " on " << tcs[i].get_street() << endl;
            street_distance = 0;
            direction.clear();
        }
    }

    cout << "Your tour has finished!\n";
    cout << "Total tour distance: " << std::fixed << std::setprecision(3) << total_dist << " miles\n";
}

int main(int argc, char* argv[])
{
    GeoDatabase geodb;
    if (!geodb.load("mapdata.txt"))
    {
        cout << "Unable to load map data" << endl;
    }

    GeoPoint p1("34.0625329", "-118.4470263");
    GeoPoint p2("34.0685657", "-118.4489289");

    Router router(geodb);
    vector<GeoPoint> path = router.route(p1,p2);

    for(auto p : path)
	{
        cerr << p.to_string() << endl;
	}

    /*
        TourGenerator tg(geodb, router);

    Stops stops;
    if (!stops.load("stops.txt"))
    {
        cout << "Unable to load tour data"<< endl;
    }

    std::cout << "Routing...\n\n";
    vector<TourCommand> tcs = tg.generate_tour(stops);
    if (tcs.empty())
        cout << "Unable to generate tour!\n";
    else
        print_tour(tcs);
    */

}
