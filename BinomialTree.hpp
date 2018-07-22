//
//  BinomialTree.hpp
//  pricing_moderls
//
//  Created by Hankun Zhang on 09/07/2018.
//  Copyright © 2018 Julius Cheung. All rights reserved.
//

#ifndef BinomialTree_hpp
#define BinomialTree_hpp

#include <stdio.h>
#include "PricingModelsI.hpp"

class BinomialTree : public PricingModelsI
{
public:
    BinomialTree();
    virtual ~BinomialTree();
    
    virtual R price(const DateTime &asOfTime,  const DateTime &expiryTime,
                    R strike, B isCall, B isAmer, R spot, R vol, R r, R q);
    virtual void displayInfo();
};

#endif /* BinomialTree_hpp */
