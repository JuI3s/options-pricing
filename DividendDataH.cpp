//
//  DividendDataH.cpp
//  pricing_moderls
//
//  Created by Hankun Zhang on 09/07/2018.
//  Copyright © 2018 Julius Cheung. All rights reserved.
//

#include <iostream>
#include "DividendDataH.hpp"


DividendDataH::DividendDataH(VecDateTime divTimes,
                             VecR divCashs,
                             VecR divProps)
{
    if (!((divTimes.size() == divCashs.size()) && (divTimes.size() == divProps.size()) ))
    {
        throw runtime_error("Incorrect dividend data.");
    }
    divTimes_ = divTimes;
    divCashs_ = divCashs;
    divProps_ = divProps;
}

DividendDataH::~DividendDataH() {}

void const DividendDataH::Display()
{
    VecRItr itrR;
    VecDateTimeItr itrDT;
    
    std::cout << "Displaying dividend payment dates: " << endl << endl;
    for (itrDT = divTimes_.begin(); itrDT != divTimes_.end(); ++itrDT)
    {
        itrDT->Display();
    }
    
    std::cout << endl << endl;
    std::cout << "Displaying cash dividends: " << endl << endl;
    for (itrR = divCashs_.begin(); itrR != divCashs_.end(); ++itrR)
    {
        std::cout << *itrR << " ";
    }
    
    std::cout << endl << endl;
    std::cout << "Displaying proportional dividend rates: " << endl << endl;
    
    for (itrR = divProps_.begin(); itrR != divProps_.end(); ++itrR)
    {
        std::cout << *itrR << " ";
    }
}

VecDateTime DividendDataH::getDivTimes() const
{
    return divTimes_;
}

VecR DividendDataH::getDivCashs() const
{
    return divCashs_;
}

VecR DividendDataH::getDivProps() const
{
    return divProps_;
}
