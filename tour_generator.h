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
    TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router);
    virtual ~TourGenerator();
    virtual std::vector<TourCommand> generate_tour(const Stops& stops) const;
};

#endif /* tour_generator_h */
