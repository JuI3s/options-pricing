//
//  MonteCarlo.cpp
//  pricing_moderls
//
//  Created by Hankun Zhang on 09/07/2018.
//  Copyright © 2018 Julius Cheung. All rights reserved.
//

#include "MonteCarloI.hpp"
#include <iostream>
#include <numeric>
#include <cmath>

MonteCarloI::MonteCarloI():PricingModelsI() {};
MonteCarloI::MonteCarloI(DividendDataH* dividendData):PricingModelsI(dividendData){};
MonteCarloI::~MonteCarloI() {};

void MonteCarloI::displayInfo()
{
    std::cout << "Monte Carlo pricing model interface." << endl;
}

LSMC::LSMC():numOfPaths_(1000), numOfSteps_(32), betas_(32, laguerreOrder), MonteCarloI() {};
LSMC::LSMC(int numOfPaths, int numOfSteps):
    numOfPaths_(numOfPaths), numOfSteps_(numOfSteps), betas_(numOfSteps, laguerreOrder), MonteCarloI() {};
LSMC::LSMC(DividendDataH* dividendData):
    numOfPaths_(1000), numOfSteps_(32), betas_(32, laguerreOrder), MonteCarloI(dividendData) {};
LSMC::LSMC(int numOfPaths, int numOfSteps, DividendDataH* dividendData):
    numOfPaths_(numOfPaths), numOfSteps_(numOfSteps), betas_(numOfSteps, laguerreOrder), MonteCarloI(dividendData) {};
LSMC::~LSMC() {};

R LSMC::laguerre0(R x) const
{
    return (1);
}

R LSMC::laguerre1(R x) const
{
    //return (-x + 1);
    return x;
}

R LSMC::laguerre2(R x) const
{
    //return ( (std::pow(x, 2.0) - 4 * x + 2.0) / 2.0);
    return ( (std::pow(x, 2.0) - 1) / std::sqrt(2));
    //return pow(x,2);
}

R LSMC::laguerre3(R x) const
{
    //return ( (0 - std::pow(x, 3.0) + 9 * std::pow(x, 2.0) - 18 * x + 6) / 6.0);
    return (( std::pow(x, 3.0) - 3 * x) / std::sqrt(6));
    //return pow(x,3);
}

R LSMC::laguerre4(R x) const
{
    //return ( (std::pow(x, 4.0) - 16 * std::pow(x, 3.0) + 72 * std::pow(x, 2.0) - 96 * x + 24) / 24.0);
    return ( (std::pow(x, 4.0) - 6 * std::pow(x, 2.0) + 3) / sqrt(24));
    //return pow(x,4);
}

R LSMC::laguerre5(R x) const
{
    return ((0 - std::pow(x, 5.0) + 25 * std::pow(x, 4.0) - 200 * std::pow(x, 3.0) + 600 * std::pow(x, 2.0) - 600 * x + 120) / 120.0);
    //return pow(x,5);
}

R LSMC::weightedLaguerre(R x, int ord) const
{
    R result = 0;
    switch (ord)
    {
        case 0:
            result += laguerre0(x);
            break;
        case 1:
            result += laguerre1(x);
            break;
        case 2:
            result += laguerre2(x);
            break;
        case 3:
            result += laguerre3(x);
            break;
        case 4:
            result += laguerre4(x);
            break;
        /*
        case 5:
            result += laguerre5(x);
            break;
        */
        default:
            throw runtime_error("Invalid input. Only laguerre polynomials of order less than 5 are supported.");
            break;
    }
    //result *= exp((0 - std::pow(x, 2)) / 2.0);
    return result;
}

R LSMC::price(const DateTime &asOfTime,  const DateTime &expiryTime,
                R strike, B isCall, B isAmer, R spot, R vol, R r, R q)
{
    R timeToExpire = expiryTime - asOfTime;
    R price = 0;
    R drift = r - q;
    Matrix simulatedStocks(numOfPaths_, numOfSteps_);
    R delT = this->PricingModelsI::calDelT(asOfTime, expiryTime, numOfSteps_);
    VecR shifts;
    Gen generator;
    Norm distribution(0, sqrt(delT));
    
    std::cout << "Princing an option.. " << endl;
    
    if (checkHasDivs())
    {
        DividendDataH* divDataH = getDivData();
        VecDateTime divTimes = divDataH->getDivTimes();
        VecR divCashs = divDataH->getDivCashs();
        VecR divProps = divDataH->getDivProps();
        DateTime nextDivTime = divTimes[0];
        divTimes.erase(divTimes.begin());
        R nextDivProp;
        R nextDivCash;
        R stockPrice;
        R x;
        R future;
        R shift;
        VecR dOverF;    //array to store terms of the cash dividends over proportional growth factors for use in computation
        R nextDoF;
        R preF;         //proportional growth factor at the previous step
        int choice;
        nextDivProp = divProps[0];
        nextDivCash = divCashs[0];
        divProps.erase(divProps.begin());
        divCashs.erase(divCashs.begin());
        
        //calculating dOverF
        R propGrowthFactor = 1;
        R entry;
        for (int i = 0; i < divTimes.size(); i++)
        {
            propGrowthFactor = propGrowthFactor * std::exp(drift * delT) * (1 - divProps[i]);
            entry = divCashs[i] / propGrowthFactor;
            dOverF.emplace_back(entry);
        }
        
        do
        {
            std::cout << "Enter which hybrid model to use?" << endl;
            std::cin >> choice;
        } while (choice < 1 || choice > 3);
        std::cout << "Using model HYB" << choice << endl << endl;
        for (int path = 0; path < numOfPaths_; path++)
        {
            preF = 1;
            stockPrice = spot;
            x = 1;    //martingale measure of expectation 1
            future = spot;

            //calculating shifts at t = 0
            switch (choice)
            {
                case 1:
                    shift = accumulate(dOverF.begin(), dOverF.end(), 0);
                    break;
                case 2:
                    shift = 0;
                    for (int i = 0; i < divTimes.size(); i++)
                    {
                        if (divTimes[i].getTime() <= timeToExpire)
                        {
                            shift += dOverF[i];
                        }
                    }
                    break;
                case 3:
                    shift = 0;
                    for (int i = 0; i < divTimes.size(); i++)
                    {
                        if (divTimes[i].getTime() <= timeToExpire)
                        {
                            shift += (1 - divTimes[i].getTime() / timeToExpire) * dOverF[i];
                        }
                    }
                    break;
                default:
                    break;
            }
            simulatedStocks.setEntry(path, 0, stockPrice);
            for (int step = 1; step < numOfSteps_; step++)
            {
                if ((nextDivTime - step * delT * DateTime::nanosPerYear) > 0)
                {
                    //not passing through a dividend date future and shift grow by constant factor
                    future *= std::exp(delT * drift);
                    shift *= std::exp(delT * drift);
                    preF *= std::exp(delT * drift);
                } else
                {
                    nextDoF = dOverF[0];
                    dOverF.erase(dOverF.begin());
                    future = calcFutureAcrossDivTime(drift, delT, nextDivProp, nextDivCash, future);
                    shift = calcShiftAcrossDivTime(nextDoF, preF, shift, drift, delT);
                    preF = calcPropGrowthFactorAcrossDividendDate(preF, nextDivProp, delT);
                    if (divTimes.size() != 0)
                    {
                        nextDivTime = divTimes[0];
                        nextDivProp = divProps[0];
                        nextDivCash = divCashs[0];
                        divTimes.erase(divTimes.begin());
                        divProps.erase(divProps.begin());
                        divCashs.erase(divCashs.begin());
                    }
                }
                shifts.emplace_back(shift);
                x = stockPriceGB(x, vol, 0, delT, generator, distribution);
                //stockprice calculated as the shifted price of the pure stock
                stockPrice = (future - shift) * x + shift;
                simulatedStocks.setEntry(path, step, stockPrice);
            }
        }
    } else
    {
        //First set of simulations
        for (int n = 0; n < numOfPaths_; n++)
        {
            R stockPrice = spot;
            simulatedStocks.setEntry(n, 0, stockPrice);
            for (int step = 1; step < numOfSteps_; step++)
            {
                stockPrice = stockPriceGB(stockPrice, vol, drift, delT, generator, distribution);
                simulatedStocks.setEntry(n, step, stockPrice);
            }
            std::cout << "Stock: " << simulatedStocks.getEntry(n, numOfSteps_ - 1) << endl;
        }
    }
    
    if (isAmer) //American options
    {
        const int laguerreOrder = 5;
        Matrix values(numOfPaths_, numOfSteps_);
        Matrix &betas = betas_;
        Matrix phi(numOfPaths_ , laguerreOrder);     //basis functions for interpolation
        VecR stocks (numOfPaths_);
        VecR valuesNext (numOfPaths_);
        VecR continuationValues (numOfPaths_);
        VecR betaForStep (laguerreOrder);
        VecR phiStocksForStep (laguerreOrder);
        VecR v;
        
        //initialise the last column of the value matrix
        for (int i = 0; i < numOfPaths_; i++)
        {
            values.setEntry(i, numOfSteps_ - 1, payoff(isCall, simulatedStocks.getEntry(i, numOfSteps_ - 1), strike));
           // std::cout << "Value: " << values.getEntry(i, numOfSteps_ - 1) << endl;
        }
        
        //backward induction
        for (int n = numOfSteps_ - 2; n > 0; n--)
        {
            R value;
            copyColVec(stocks, simulatedStocks, n);
            copyColVec(valuesNext, values, n + 1);
            
            //setting the coefficients beta for step n
            setBetaAndContinuationValues(betas, phi, stocks, valuesNext, n, spot, delT, vol, drift);
            copyRowVec(betaForStep, betas, n);
            copyRowVec(phiStocksForStep, phi, n);
            
            for (int  i = 0; i < numOfPaths_; i++)
            {
                R adjustedStrike = strike;
                copyRowVec(phiStocksForStep, phi, i);
                continuationValues[i] = calcContinuationValue(betaForStep, phiStocksForStep);
                value = alias::max(payoff(isCall, stocks[i], strike), std::exp(0 - delT) * continuationValues[i]);
                if (checkHasDivs())
                {
                    //strike adjustment
                    adjustedStrike = strike - shifts[i];
                }
                if (payoff(isCall, stocks[i], adjustedStrike) > std::exp(0 - delT) * continuationValues[i])
                    //exercise the option
                {
                    values.setEntry(i, n + 1, 0);
                    values.setEntry(i, n, payoff(isCall, stocks[i], adjustedStrike));
                } else
                {
                    values.setEntry(i, n, (std::exp(0 - delT) * continuationValues[i]));
                }
            }
        }
        
        VecR optimalStoppingTimes (numOfPaths_);
        R stockprice = spot;
        Gen gen;
        Norm norm(0, sqrt(delT));
        for (int path = 0; path < numOfPaths_; path++)
        {
            int step = 1;
            while (values.getEntry(path, step) != 0)
            {
                step++;
            }
            optimalStoppingTimes[path] = step--;
            for (int i = 0; i < 50; i++)
            {
                stockprice = spot;
                for (int j = 0; j < step; j++)
                {
                    stockprice = stockPriceGB(stockprice, vol, drift, delT, gen, norm);
                }
                price += payoff(isCall, stockprice, strike);
            }
        }
        price /= (numOfPaths_ * 50);
    } else //European options
    {
        VecR values (numOfPaths_);
        simulatedStocks.display();
        for(int path = 0; path < numOfPaths_; path++)
        {
            values[path] = payoff(isCall, simulatedStocks.getEntry(path, numOfSteps_ - 1), strike);
            //std::cout << simulatedStocks.getEntry(path, numOfSteps_ - 1) << " | " << strike << endl;
        }
        price = accumulate(values.begin(), values.end(), 0)/(R)values.size();
    }
    return price;
}

void LSMC::setBetaAndContinuationValues(alias::Matrix &beta, Matrix &phi, VecR stocks, VecR valuesNext, int step, R spot, R delT, R vol, R r)
{
    Matrix B1(laguerreOrder, laguerreOrder);
    VecR B2 (laguerreOrder);
    VecR phi_ (laguerreOrder);
    VecR betaVec(laguerreOrder);
    R stockPrice;
    R value;
    for(int n = 0; n < numOfPaths_; n++)
    {
        stockPrice = stocks[n];
        value = valuesNext[n];
        stockPrice = (std::log(stockPrice) - std::log(spot) - (r - std::pow(vol, 2) / 2.0) * step * delT)  / (vol * std::sqrt(step * delT));
        for (int i = 0; i < laguerreOrder; i++)
        {
           // std::cout << "phi" << i <<": " << weightedLaguerre(stockPrice, i) << endl;
            phi.setEntry(n, i, weightedLaguerre(stockPrice, i));
        }
        copyRowVec(phi_, phi, n);
       // displayVector(phi_);
        Matrix phiMat = vecToMatrix(phi_);
        //B1 = (B1 + (phiMat * transpose(phiMat)) );
        B2 = sumVec(B2, scaleVec(phi_, value));
    }
   // B1 = B1 / (R)numOfPaths_;
   // B1.display();
    B2 = scaleVec(B2, (1 / (R)numOfPaths_));
    setRowVec(B2, beta, step);
}

R LSMC::calcContinuationValue(VecR betaForStep, VecR phiStocksForN)
{
    return dotProduct(betaForStep, phiStocksForN);
}

void LSMC::displayInfo()
{
    std::cout << "This is an implementation of the least square Monte Carlo pricing model." << endl; 
}

void LSMC::display()
{
    this->dividendData_->Display();
}
