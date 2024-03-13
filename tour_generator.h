//
//  TourGenerator.hpp
//  Project4
//
//  Created by David Sun on 3/8/24.
//

#ifndef tour_generator_h
#define tour_generator_h

#include "base_classes.h"

class TourGenerator: public TourGeneratorBase
{
public:
    TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router) : m_geodb(geodb), m_router(router) {};
    virtual ~TourGenerator() {};
    virtual std::vector<TourCommand> generate_tour(const Stops& stops) const;
private:
    const GeoDatabaseBase& m_geodb;
    const RouterBase& m_router;
};

#endif /* tour_generator_h */
