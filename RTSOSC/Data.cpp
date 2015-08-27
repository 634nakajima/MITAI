//
//  Data.cpp
//  
//
//  Created by NAKAJIMA Musashi on 12/05/27.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#include "Data.h"

Data::Data(int v, int ID)
{
    value = v;
    dataID = ID;
}

void Data::copyData(Data *d)
{
    value = d->value;
    dataID = d->dataID;
}

bool Data::compareData(Data *d)
{
    if (value != d->value) return false;
    if (dataID != d->dataID) return false;
    return true;
}