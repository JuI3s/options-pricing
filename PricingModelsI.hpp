//
//  PricingModelsI.hpp
//  pricing_moderls
//
//  Created by Hankun Zhang on 09/07/2018.
//  Copyright © 2018 Julius Cheung. All rights reserved.
//

#ifndef PricingModelsI_hpp
#define PricingModelsI_hpp

#include <stdio.h>
#include <memory>
#include "DataAlias.hpp"
#include "DividendDataH.hpp"

using namespace alias;

//Options pricing models interface
class PricingModelsI
{
public:
    PricingModelsI();
    PricingModelsI(DividendDataH* dividendData);
    virtual ~PricingModelsI();

    //pricing methods
    virtual R price(const DateTime &asOfTime,  const DateTime &expiryTime,
                    R strike, B isCall, B isAmer, R spot, R vol, R r, R q) = 0;
    R payoff(B isCall, R spot, R strike) const;
    R stockPriceGB(R spot, const R vol, R drift, R delT, Gen &gen, Norm &distribution);
        //stock price following geomtric brownian motion
    R calDelT(const DateTime asOfTime, const DateTime expiryTime, const int numOfSteps) const;
    R calDrift(R r, R q) const;
    B checkHasDivs() const;
    DividendDataH* getDivData() const;
    void setDivData(DividendDataH* dividendData);
    
    virtual void displayInfo() = 0;
protected:
    DividendDataH* dividendData_;
};

namespace alias
{
    //using PricingModelsH = std::unique_ptr<PricingModelsI>;
    using PricingModelsH = PricingModelsI*;
}  //namespace alias

#endif /* PricingModelsI_hpp */
