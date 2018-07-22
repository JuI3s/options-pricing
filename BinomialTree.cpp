//
//  BinomialTree.cpp
//  pricing_moderls
//
//  Created by Hankun Zhang on 09/07/2018.
//  Copyright © 2018 Julius Cheung. All rights reserved.
//

#include "BinomialTree.hpp"
#include <iostream>

BinomialTree::BinomialTree() {};
BinomialTree::~BinomialTree() {};

R BinomialTree::price(const DateTime &asOfTime,  const DateTime &expiryTime,
                              R strike, B isCall, B isAmer, R spot, R vol, R r, R q)
{
    return 0;
}

void BinomialTree::displayInfo()
{
    std::cout << "Binomial Tree pricing model." << endl;
}
