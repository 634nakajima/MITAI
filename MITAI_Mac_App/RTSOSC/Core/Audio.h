//
//  Audio.h
//  
//
//  Created by NAKAJIMA Musashi on 12/05/27.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef _Audio_h
#define _Audio_h

#include <iostream>

class Audio
{
public:
    short           *audio;
    unsigned long    length;
    
    Audio(unsigned long l, short *a);
};

#endif
