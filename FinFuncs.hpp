//
//  FinFuncs.hpp
//  pricing_moderls
//
//  Created by Hankun Zhang on 22/07/2018.
//  Copyright © 2018 Julius Cheung. All rights reserved.
//

#ifndef FinFuncs_hpp
#define FinFuncs_hpp

#include <stdio.h>
#include "DataAlias.hpp"

using namespace std;

namespace alias {
    inline R calcShiftAcrossDivTime(R nextDoF, R preF, R preResult, R drift, R delT)
    {
        R result;
        result = (preResult - preF * nextDoF) * std::exp(drift * delT);
        return result;
    }
    
    
    inline R calcFutureAcrossDivTime(R drift, R delT, R nextDivProp, R nextDivCash, R preFuture)
    {
        R future;
        future = (preFuture * std::exp(delT * drift)) * (1 - nextDivProp) - nextDivCash;
        return future;
    }
    
    inline R calcPropGrowthFactorAcrossDividendDate(R preResult, R nextDivProp, R delT)
    {
        R result;
        result = preResult * std::exp(delT) * (1 - nextDivProp);
        return result;
    }
}

#endif /* FinFuncs_hpp */
