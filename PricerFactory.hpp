//
//  Pricer.hpp
//  pricing_moderls
//
//  Created by Hankun Zhang on 09/07/2018.
//  Copyright © 2018 Julius Cheung. All rights reserved.
//

#ifndef Pricer_hpp
#define Pricer_hpp

#include <stdio.h>
#include "PricingModelsI.hpp"
#include "BinomialTree.hpp"
#include "MonteCarloI.hpp"

using namespace alias;

namespace alias {
    enum PricingModels {TBinomialTree = 1, TLSMC = 2, TBlackScholes = 3};
}

//abstract factory class
class PricerFactory
{
public:
    static PricingModelsH makeModel(PricingModels);
};

#endif /* Pricer_hpp */
