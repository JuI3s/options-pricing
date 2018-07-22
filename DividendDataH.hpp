//
//  DividendDataH.hpp
//  pricing_moderls
//
//  Created by Hankun Zhang on 09/07/2018.
//  Copyright © 2018 Julius Cheung. All rights reserved.
//

#ifndef DividendDataH_hpp
#define DividendDataH_hpp

#include <stdio.h>
#include "DataAlias.hpp"

using namespace alias;

class DividendDataH
{
public:
    DividendDataH(VecDateTime divTimes,
                  VecR divCashs,
                  VecR divProps
                  );
    ~DividendDataH();
    void const Display();
    VecDateTime getDivTimes() const;
    VecR getDivCashs() const;
    VecR getDivProps() const;
    
private:
    VecDateTime divTimes_;
    VecR divCashs_;
    VecR divProps_;
};
#endif /* DividendDataH_hpp */
