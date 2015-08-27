//
//  Data.h
//  
//
//  Created by NAKAJIMA Musashi on 12/05/27.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef _Data_h
#define _Data_h

class Data
{
public:
    int value;
    int dataID;

    Data(int v, int ID);

    void copyData(Data *d);
    bool compareData(Data *d);
    
};

#endif
