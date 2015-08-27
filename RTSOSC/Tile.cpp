//
//  Tile.cpp
//  
//
//  Created by NAKAJIMA Musashi on 12/10/14.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "Tile.h"

Tile::Tile(int t, unsigned char *a64, unsigned char *a16)
{
    for (int i = 0; i < 8; i++) {
        XBeeAddr64[i] = a64[i];
    }
	for (int i = 0; i < 2; i++) {
        XBeeAddr16[i] = a16[i];
    }
	
    tID = t;
    mColor = -1;
    dead = 1;
    standby = 0;
    alive = 0;
    deadCount = 0;
	data = 0;
	stream = 0;
    co = NULL;
}

Tile::Tile(Coordinator *c, int t, unsigned char *a64, unsigned char *a16)
{
    for (int i = 0; i < 8; i++) {
        XBeeAddr64[i] = a64[i];
    }
	for (int i = 0; i < 2; i++) {
        XBeeAddr16[i] = a16[i];
    }
	
    tID = t;
    mColor = -1;
    dead = 1;
    standby = 0;
    alive = 0;
    deadCount = 0;
	data = 0;
	stream = 0;
    co = c;
}

Tile::Tile(Coordinator *c, int t, int mc)
{	
    tID = t;
    mColor = mc;
    dead = 1;
    standby = 0;
    alive = 0;
    deadCount = 0;
	data = 0;
	stream = 0;
    co = c;
    
    createModule();
}

void Tile::isAlive()
{
    if (alive) {
        deadCount = 0;
    }else {
        dead = 0;
        alive = 1;
    }
}

int Tile::deadCheck()
{
    if (dead) return 0;

    if(deadCount++ > TH) {
        dead = 1;
        standby = 0;
        alive = 0;
        mColor = -1;
        deadCount = 0;
		data = 0;
		stream = 0;
        return 1;
    }
    return 0;
}

void Tile::connectDataTo(Tile *t) {
    if (co) co->addConnection(tID, t->tID, "/Data");
}
void Tile::disconnectDataFrom(Tile *t) {
    if (co) co->disconnect(tID, t->tID, "/Data");
}
void Tile::disconnectDataAll() {
    if (co) co->disconnectAll(tID, "/Data");
}

void Tile::connectAudioTo(Tile *t) {
    if (co) co->addConnection(tID, t->tID, "/Audio");
}
void Tile::disconnectAudioFrom(Tile *t) {
    if (co) co->disconnect(tID, t->tID, "/Audio");
}
void Tile::disconnectAudioAll() {
    if (co) co->disconnectAll(tID, "/Audio");
}

void Tile::connectTo(Tile *t, const char *a) {
    if (co) co->addConnection(tID, t->tID, a);
}

void Tile::connectTo(Tile *t, int outID, const char *a) {
    if (co) co->addConnection(tID, t->tID, outID, a);
}
void Tile::disconnectFrom(Tile *t, const char *a) {
    if (co) co->disconnect(tID, t->tID, a);
}
void Tile::disconnectFrom(Tile *t, int outID, const char *a) {
    if (co) co->disconnect(tID, t->tID, outID, a);
}

void Tile::disconnectAll(const char *a) {
    if (co) co->disconnectAll(tID, a);
}

void Tile::createModule() {
    if(!co) return;
    co->createModule(tID, mColor);
}

void Tile::deleteModule() {
    if(!co) return;
    co->deleteModule(tID, mColor);
}

Tile::~Tile()
{
    deleteModule();
}