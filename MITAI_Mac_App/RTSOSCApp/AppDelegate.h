//
//  AppDelegate.h
//  RTSOSCApp
//
//  Created by Musashi Nakajima on 2014/08/13.
//  Copyright (c) 2014年 Musashi Nakajima. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include "Server.h"
#include "Coordinator.h"
#include "CoffeeMill.h"
#include "Twinkle.h"
#include "DAC.h"
#include "Serial.h"
#include "SerialParser.h"
#include "Object.h"
#include "AudioSource.h"
#include "Delay.h"
#include "Lowpass.h"
#include "ModuleManager.h"
#include "AMSynth.h"

@interface AppDelegate : NSObject <NSApplicationDelegate>
{
    IBOutlet NSMenu *menu;
    Server          s;
    Serial          serial;
    SerialParser    serialParser;

    CMManager                   cm;
    ObjectManager               fan, drawer, light, knob, fader;
    ModuleManager<DAC>          dac;
    ModuleManager<Lowpass>      lp;
    ModuleManager<Twinkle>      tw;
    ModuleManager<class Delay>  delay;
    ModuleManager<AMSynth>      ams;
    AudioSourceManager          furin, disny;
}

@end
