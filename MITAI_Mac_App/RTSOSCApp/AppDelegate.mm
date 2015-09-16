//
//  AppDelegate.m
//  RTSOSCApp
//
//  Created by Musashi Nakajima on 2014/08/13.
//  Copyright (c) 2014年 Musashi Nakajima. All rights reserved.
//

#import "AppDelegate.h"



@implementation AppDelegate

NSStatusItem *_statusItem;

- (void)setupStatusItem
{
    NSStatusBar *systemStatusBar = [NSStatusBar systemStatusBar];
    _statusItem = [systemStatusBar statusItemWithLength:NSVariableStatusItemLength];
    [_statusItem setHighlightMode:YES];
    [_statusItem setTitle:@"MITAI"];
    [_statusItem setImage:[NSImage imageNamed:@"GrindTemplate"]];
    [_statusItem setMenu:self->menu];
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    
    [self setupStatusItem];
    
    cm.init(&s, "");
    dac.init(&s, "");
    dac.setMInfo("/DAC", "/AudioIn,/Volume", "/AudioOut,/Level", "Speaker.png");

    tw.init(&s, "");
    tw.setMInfo("/Twinkle", "/Tempo", "/Audio");

    delay.init(&s, "");
    delay.setMInfo("/Delay", "/AudioIn,/Duration,/Gain,/Bypass", "/AudioOut");

    lp.init(&s, "");
    lp.setMInfo("/Lowpass", "/AudioIn,/Freq,/Q,/Bypass", "/AudioOut");

    fan.init(&s, "");
    fan.setMInfo("/Fan", "/Intensity", "/Knob", "Fan.png");
    fan.objectID = 0;

    light.init(&s, "");
    light.setMInfo("/Light", "/Intensity", "/Knob");
    light.objectID = 1;

    drawer.init(&s, "");
    drawer.setMInfo("/Drawer", "", "/Open", "Drawer.png");
    drawer.objectID = 2;

    /*knob.init(&s, "");
    knob.setMInfo("/Knob", "", "/Value");
    knob.objectID = 3;

    fader.init(&s, "");
    fader.setMInfo("/Fader", "", "/Value");
    fader.objectID = 4;*/

    ams.init(&s, "");
    ams.setMInfo("/Synth", "/Freq,/PlayStop", "/AudioOut,/Level");

    furin.init(&s, "");
    furin.setMInfo("/Furin", "", "/AudioOut");
    furin.setFileName("Furin.wav");
    
    disny.init(&s, "");
    disny.setMInfo("/Disny", "", "/AudioOut");
    disny.setFileName("Disny.wav");

    serial.setDevice("/dev/cu.usbmodem621");

    //シリアル→シリアルパーサへのコールバック登録
    serial.setCallback(serialParser.serialCallback, &serialParser);
    
    //シリアルパーサ→オブジェクトへのコールバック登録（オブジェクトIDが登録順になることに注意）
    serialParser.setCallback(fan.serialCallback, &fan);
    serialParser.setCallback(light.serialCallback, &light);
    serialParser.setCallback(drawer.serialCallback, &drawer);
    serialParser.setCallback(knob.serialCallback, &knob);
    serialParser.setCallback(fader.serialCallback, &fader);

    //オブジェクト→シリアルへのコールバック登録
    fan.setSerialCallback(serial.writeDataCallback, &serial);
    light.setSerialCallback(serial.writeDataCallback, &serial);
    
}

@end
