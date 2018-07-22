//
//  PricingModelsI.cpp
//  pricing_moderls
//
//  Created by Hankun Zhang on 09/07/2018.
//  Copyright © 2018 Julius Cheung. All rights reserved.
//

#include "PricingModelsI.hpp"
#include <iostream>

PricingModelsI::PricingModelsI():dividendData_(0) {}; //pricing an option with no dividend
PricingModelsI::PricingModelsI(DividendDataH* dividendData):dividendData_(dividendData) {}; //pricing dividend bearing options
PricingModelsI::~PricingModelsI() {};

R PricingModelsI::payoff(B isCall, R spot, R strike) const
{
    if (isCall) //call option
    {
        return max(0, spot - strike);
    } else  //pay option
    {
        return max(0, strike - spot);
    }
}

R PricingModelsI::stockPriceGB(R spot, const R vol, const R drift, const R delT,
                     Gen &gen, Norm &distribution)
{
    R stock;
    R x = distribution(gen);
    stock = spot * drift * delT + vol * x * spot + spot;
    return stock;
}

R PricingModelsI::calDelT(const DateTime asOfTime, const DateTime expiryTime, const int numOfSteps) const
{
    R delT;
    delT = (expiryTime - asOfTime) / (double)DateTime::nanosPerYear / (double)numOfSteps;
    //rounding error, speed?
    return delT;
}
R PricingModelsI::calDrift(R r, R q) const
{
    return (r - q);
}

B PricingModelsI::checkHasDivs() const
{
    if (dividendData_ != 0) return true;
    return false;
}

DividendDataH* PricingModelsI::getDivData() const
{
    if (dividendData_ == 0)
    {
        throw runtime_error("No dividend data.");
    }
    return dividendData_;

}

void PricingModelsI::setDivData(DividendDataH* dividendData)
{
    dividendData_ = dividendData; 
}
