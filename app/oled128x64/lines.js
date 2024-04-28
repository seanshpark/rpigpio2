//
// Test application: OLED126x64 0.96" + SSD1306  + I2C
//

var fs = require('fs');

// TODO fix load path
const rpigpio2 = require(process.cwd() + '/build/Debug/rpigpio2');

const gpio = new rpigpio2.GPIO();
const i2c1 = new rpigpio2.I2C();
const oled128x64 = new rpigpio2.OLED128x64();

gpio.init();
i2c1.init(gpio, 1, 0x3c);
// 1 is for I2C-1 as (SDA @ pin2, CLK @ pin3)
// OLED address is 0x3c
oled128x64.init(i2c1);

process.stdin.resume();
process.on('SIGINT', exitHandler.bind(null, {exit: true}));   // ctrl+c
process.on('exit', exitHandler.bind(null, {cleanup: true}));  // app closing

oled128x64.addrMode(0);
oled128x64.colAddr(0x00, 0x7f);
oled128x64.pageAddr(0x00, 0x07);

oled128x64.clear();

function rand(range) {
  return Math.floor(Math.random() * range);
}

timerId = setInterval(function() {
  let step = rand(3) + 2;
  let xc = rand(118) + 5;
  let yc = rand(54) + 5;
  let x, y;

  for (x=0; x<128; x+=step) {
    oled128x64.line(x, 0, xc, yc, 1);
    oled128x64.line(xc, yc, x, 63, 1);
  }
  for (y=0; y<64; y+=step) {
    oled128x64.line(0, y, xc, yc, 1);
    oled128x64.line(xc, yc, 127, y, 1);
  }

  oled128x64.clear();
}, 1);

function exitHandler(options, exitCode) {
  if (options.exit) {
    oled128x64.clear();
    oled128x64.release();

    i2c1.release();
    gpio.release();
    process.exit(0);
  }
}
