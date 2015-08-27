//
//  Tile.h
//  
//
//  Created by NAKAJIMA Musashi on 12/10/14.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef _Tile_h
#define _Tile_h

#include "Coordinator.h"

#define TH  20

class Coordinator;
class Tile
{
public:
    int		tID;
    int     mColor;
    char    XBeeAddr64[8];
	char	XBeeAddr16[2];
    unsigned int		data, stream;

    Coordinator *co;
    
    Tile(int t, unsigned char *a64, unsigned char *a16);
    Tile(Coordinator *c, int t, unsigned char *a64, unsigned char *a16);
    Tile(Coordinator *c, int t, int mc);

    ~Tile();
   
	void    isAlive();
    int     deadCheck();
    void    connectDataTo(Tile *t);
    void    disconnectDataFrom(Tile *t);
    void    disconnectDataAll();

    void    connectAudioTo(Tile *t);
    void    disconnectAudioFrom(Tile *t);
    void    disconnectAudioAll();
    
    void    connectTo(Tile *t, const char *a);
    void    connectTo(Tile *t, int outID, const char *a);
    void    disconnectFrom(Tile *t, const char *a);
    void    disconnectFrom(Tile *t, int outID, const char *a);
    void    disconnectAll(const char *a);

    void    createModule();
    void    deleteModule();
    
private:
	int     dead, alive, standby;
    int     deadCount;

};

#endif
