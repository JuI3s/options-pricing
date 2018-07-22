//
//  main.cpp
//  pricing_moderls
//
//  Created by Hankun Zhang on 11/06/2018.
//  Copyright © 2018 Julius Cheung. All rights reserved.
//

#include <iostream>
#include <math.h>
#include "DataAlias.hpp"
#include "DividendDataH.hpp"
#include "PricerFactory.hpp"
#include "MonteCarloI.hpp"

using namespace std;
using namespace alias;

int main(int argc, const char * argv[]) {
    
    std::cout << "Testing class DateTime: ";
    DateTime test(23151);
    
    VecDateTime divTimes;
    VecR divCashs;
    VecR divProps;
    
    std::cout << "Testing class DividendDataH: ";
    for (int i = 0; i < 5; ++i)
    {
        divTimes.emplace_back(DateTime((TimeNanos)(i * 0.25 * DateTime::nanosPerYear)));
        divCashs.emplace_back(0.5);
        divProps.emplace_back(0.01);
    }
    DividendDataH dividendData( divTimes, divCashs, divProps);
    dividendData.Display();
    DividendDataH* divData = &dividendData;
    
    std::cout << endl << endl;
    std::cout << "Testing factory class PricerFactory: ";
    
    DateTime asOfTime( TimeNanos( 0 ) );
    R spot = 5;
    R r    = 0.06; // 0.04 = 4% interest rate
    R q    = 0.01; // 0.01 = 1% borrow cost
    R vol  = 0.40; // 0.3  = 30% vol
    R timeToExpiry = 1.0; //1.123
    DateTime expiryTime( TimeNanos( timeToExpiry * DateTime::nanosPerYear ) );
    R strike = 20;
    B isCall = false;
    B isAmer = true;
    R price;
    Gen gen;
    Norm distribution(0, (vol * vol));
    
    std::cout << "Spot: " << spot << endl;
    std::cout << "Strike: " << strike << endl;
    std::cout << "isCall: " << isCall << endl;
    std::cout << "isAmer: " << isAmer << endl;
    std::cout << "r: " << r << endl;
    std::cout << "q: " << q << endl;
    std::cout << "vol: " << vol << endl;
    std::cout << "Time to expire in years: " << spot << endl;

    auto MonteCarlo = PricerFactory::makeModel(TLSMC);
    MonteCarlo->setDivData(divData);
    price = MonteCarlo->price(asOfTime, expiryTime, strike, isCall, isAmer, spot, vol, r, q);
    std::cout << "The price of the American option is: " << price << endl;
    
    return 0;
}
