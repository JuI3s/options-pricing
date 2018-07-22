//
//  MonteCarlo.hpp
//  pricing_moderls
//
//  Created by Hankun Zhang on 09/07/2018.
//  Copyright © 2018 Julius Cheung. All rights reserved.
//

#ifndef MonteCarloI_hpp
#define MonteCarloI_hpp

#include <stdio.h>
#include "PricingModelsI.hpp"
#include "DividendDataH.hpp"
#include "FinFuncs.hpp"

class MonteCarloI : public PricingModelsI
{
public:
    MonteCarloI();
    MonteCarloI(DividendDataH* dividendData);
    virtual ~MonteCarloI();
    
    virtual R price(const DateTime &asOfTime,  const DateTime &expiryTime,
                    R strike, B isCall, B isAmer, R spot, R vol, R r, R q) = 0;
    virtual void displayInfo();
};

class LSMC : public MonteCarloI
//Least square Monte Carlo pricing model
{
public:
    LSMC();
    LSMC(int numOfSteps, int numOfPaths);
    LSMC(DividendDataH* dividendData);
    LSMC(int numOfSteps, int numOfPaths, DividendDataH* DividendDataH);
    ~LSMC();
    
    virtual R price(const DateTime &asOfTime,  const DateTime &expiryTime,
                    R strike, B isCall, B isAmer, R spot, R vol, R r, R q);    
    void display();
    virtual void displayInfo();
protected:
    const int J = 5;    //order of the Laguerre polynomial approximation
    int numOfSteps_;    //default value 32
    int numOfPaths_;    //default value 1000
    Matrix betas_;    //linear model basis coefficients to be optimised
    
    //First five orders of Laguerre polynomials
    //used for approximating value if to hold the stock
    R laguerre0(R x) const;
    R laguerre1(R x) const;
    R laguerre2(R x) const;
    R laguerre3(R x) const;
    R laguerre4(R x) const;
    R laguerre5(R x) const;
    R weightedLaguerre(R x, int ord) const;
    
    void setBetaAndContinuationValues(Matrix &beta, Matrix &phi, VecR stocks, VecR valuesNext, int step, R spot, R delT, R vol, R r);
    R calcContinuationValue(VecR betaForStep, VecR phiStocksForN);
};

#endif /* MonteCarlo_hpp */
