//
// Test application: OLED126x32 0.96" + SSD1309  + I2C
//

var fs = require('fs');

// TODO fix load path
const rpigpio2 = require(process.cwd() + '/build/Debug/rpigpio2');

const gpio = new rpigpio2.GPIO();
const i2c1 = new rpigpio2.I2C();
const oled128x32 = new rpigpio2.OLED128x32();

gpio.init();
i2c1.init(gpio, 1, 0x3c);
// 1 is for I2C-1 as (SDA @ pin2, CLK @ pin3)
// OLED address is 0x3c
oled128x32.init(i2c1);

process.stdin.resume();
process.on('SIGINT', exitHandler.bind(null, {exit: true}));   // ctrl+c
process.on('exit', exitHandler.bind(null, {cleanup: true}));  // app closing

oled128x32.addrMode(0);
oled128x32.colAddr(0x00, 0x7f);
oled128x32.pageAddr(0x00, 0x07);

oled128x32.clear();

function rand(range) {
  return Math.floor(Math.random() * range);
}

let XEND = 127;
let YEND = 31;
let line_x = true;
let xs = 0;
let xy = xs;
let step;
let xc;
let yc;

timerId = setInterval(function() {
  if (line_x && xy === 0) {
    step = rand(3) + 2;
    xc = rand(118) + 5;
    yc = rand(25) + 5;
  }

  if (line_x) {
    oled128x32.line(xy, 0, xc, yc, 1);
    oled128x32.line(xc, yc, xy, YEND, 1);
    xy += step;
    if (xy > XEND) {
      line_x = false;
      xy = 0;
    }
  }
  else {
    oled128x32.line(0, xy, xc, yc, 1);
    oled128x32.line(xc, yc, XEND, xy, 1);
    xy += step;
    if (xy > YEND) {
      line_x = true;
      xy = 0;
      oled128x32.clear();
    }
  }
}, 0);

function exitHandler(options, exitCode) {
  if (options.exit) {
   oled128x32.clear();
    oled128x32.release();

    i2c1.release();
    gpio.release();
    process.exit(0);
  }
}
