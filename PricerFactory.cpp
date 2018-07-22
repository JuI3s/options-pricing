//
//  Pricer.cpp
//  pricing_moderls
//
//  Created by Hankun Zhang on 09/07/2018.
//  Copyright © 2018 Julius Cheung. All rights reserved.
//

#include "PricerFactory.hpp"
#include <iostream>

PricingModelsH PricerFactory::makeModel(PricingModels pricingModel)
{
    switch (pricingModel)
    {
        case alias::TBinomialTree:
            return new BinomialTree();
            break;
        case alias::TLSMC:
            return new LSMC();
            break;
        default:
            std::cout << "Error creating factory model. " << endl;
            return NULL;
            break;
    }
}
