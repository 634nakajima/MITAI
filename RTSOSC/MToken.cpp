//
//  MToken.cpp
//  SimpleSimulator
//
//  Created by NAKAJIMA Musashi on 12/06/18.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "MToken.h"

MToken::MToken()
{
    tID = -1;
    mColor = -1;
    ip[0] = NULL;
    osc[0] = NULL;
}

void MToken::setInputInfo(char *inInfo) {
    char *addr, *info;
    if ((addr = strtok(inInfo, ",")) == NULL) return;
    else {
        info = (char *)malloc(sizeof(addr));
        strcpy(info, addr);
        inputInfo.push_back(info);
        
        while ((addr = strtok(NULL, ",")) != NULL) {
            info = (char *)malloc(sizeof(addr));
            strcpy(info, addr);
            inputInfo.push_back(info);
        }
    }
    auto it = inputInfo.begin();
    while (it != inputInfo.end()) {
        printf("input:%s\n",(char *)*it);
        ++it;
    }
}

void MToken::setOutputInfo(char *outInfo) {
    char *addr, *info;
    if ((addr = strtok(outInfo, ",")) == NULL) return;
    else {
        info = (char *)malloc(sizeof(addr));
        strcpy(info, addr);
        outputInfo.push_back(info);
        
        while ((addr = strtok(NULL, ",")) != NULL) {
            info = (char *)malloc(sizeof(addr));
            strcpy(info, addr);
            outputInfo.push_back(info);
        }
    }
    auto it= outputInfo.begin();
    while (it != outputInfo.end()) {
        printf("output:%s\n",(char *)*it);
        ++it;
    }
}

MToken::~MToken() {
    auto it = inputInfo.begin();
    while (it != inputInfo.end()) {
        char *m = *it;
        free(m);
        it = inputInfo.erase(it);
    }
}