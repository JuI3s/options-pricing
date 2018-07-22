//
//  BinomialPricingModel.hpp
//  pricing_moderls
//
//  Created by Hankun Zhang on 12/06/2018.
//  Copyright © 2018 Julius Cheung. All rights reserved.
//

#ifndef BinomialPricingModel_hpp
#define BinomialPricingModel_hpp

#include <stdio.h>
#include "FactoryModel.hpp"

#endif /* BinomialPricingModel_hpp */

class BinomialPricingModel : public OptionsPricingModelI {
public:
    BinomialPricingModel(CurretTime, TimeToExpiry, PropDividend, CashDividend, Spot, Strike, Vol, n);
};
