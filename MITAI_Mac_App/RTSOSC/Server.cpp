//
//  Server.cpp
//  
//
//  Created by NAKAJIMA Musashi on 12/10/26.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#include "Server.h"
#include <stdio.h>

Server::Server() {
    st = lo_server_thread_new("6340", NULL);
    if (st) lo_server_thread_start(st);
    else { printf("err: starting Server\n"); while(1);}
    
    st_tcp = lo_server_thread_new_with_proto("6341", LO_TCP, NULL);
    if (st_tcp) lo_server_thread_start(st_tcp);
    else { printf("err: starting Server\n"); while(1);}
}

Server::~Server(){}

void Server::setAudioCallback(void (*callback)(void *), int interval, void *user_data) {
    t.setAudioCallback(callback, interval, user_data);
}

void Server::setDataCallback(void (*callback)(void *), float interval, void *user_data) {
    t.setDataCallback(callback, interval, user_data);
}

void Server::removeAudioCallback(void (*callback)(void *)) {
    t.removeAudioCallback(callback);
}

void Server::removeDataCallback(void (*callback)(void *)) {
    t.removeDataCallback(callback);
}