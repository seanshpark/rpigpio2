//
// Test application: LCD Color 240x240 + SPI + GPIO
//

// TODO fix load path
const rpigpio2 = require(process.cwd() + '/build/Debug/rpigpio2');
const DEF = rpigpio2.DEF;
const gpio = new rpigpio2.GPIO();
const spi = new rpigpio2.SPI();
const lcdco240 = new rpigpio2.LCDCO240();

gpio.init();
gpio.pin(22, DEF.OUT);
spi.init(gpio, 0, 40 * 1000 * 1000);
lcdco240.init(spi, gpio, 22); // for Data/Command

function exitHandler(options, exitCode) {
  if (options.exit) {
    lcdco240.release();
    spi.release();
    gpio.release();
    process.exit(0);
  }
}

process.stdin.resume();
process.on('SIGINT', exitHandler.bind(null, {exit: true}));   // ctrl+c
process.on('exit', exitHandler.bind(null, {cleanup: true}));  // app closing

lcdco240.clear();

function rand(range) {
  return Math.floor(Math.random() * range);
}

let XEND = 240;
let YEND = 240;
let line_x = true;
let xy = 0;
let step;
let xc;
let yc;
let cc;

timerId = setInterval(function() {
  if (line_x && xy === 0) {
    step = rand(3) + 2;
    xc = rand(XEND - 10) + 5;
    yc = rand(YEND - 10) + 5;
    cc = rand(0x10000);
    let cchex = ('0000' + cc.toString(16).toUpperCase()).slice(-4);
    console.log("Lines at", xc, yc, cchex);
  }
  // cc = rand(0x10000);

  if (line_x) {
    lcdco240.line(xy, 0, xc, yc, cc);
    lcdco240.line(xc, yc, xy, YEND, cc);
    xy += step;
    if (xy > XEND) {
      line_x = false;
      xy = 0;
    }
  }
  else {
    lcdco240.line(0, xy, xc, yc, cc);
    lcdco240.line(xc, yc, XEND, xy, cc);
    xy += step;
    if (xy > YEND) {
      line_x = true;
      xy = 0;
      //lcdco240.clear();
    }
  }
}, 0);
