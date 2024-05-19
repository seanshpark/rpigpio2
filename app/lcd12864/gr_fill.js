//
// Test application: LCD12864 + MC23017 + I2C
//

// TODO fix load path
const rpigpio2 = require(process.cwd() + '/build/Debug/rpigpio2');

const gpio = new rpigpio2.GPIO();
const i2c = new rpigpio2.I2C();
const mcp23017 = new rpigpio2.MCP23017();
const lcd12864 = new rpigpio2.LCD12864();

// command -> lcd12864 -> mcp23017 -> i2c (-> gpio)
gpio.init();
i2c.init(gpio, 1, 0x20);
// 1 is for I2C-1 as (SDA @ pin2, CLK @ pin3)
mcp23017.init(i2c);
lcd12864.init(mcp23017);

function exitHandler(options, exitCode) {
  if (options.exit) {
    lcd12864.release();
    mcp23017.release();
    i2c.release();
    gpio.release();
    process.exit(0);
  }
}

process.stdin.resume();
process.on('SIGINT', exitHandler.bind(null, {exit: true}));   // ctrl+c
process.on('exit', exitHandler.bind(null, {cleanup: true}));  // app closing

lcd12864.display(true);
lcd12864.graphic(true);
lcd12864.clear();

function rand(range) {
  return Math.floor(Math.random() * range);
}

let XEND = 127;
let YEND = 64;
let line_x = true;
let xy = 0;
let step;
let xc;
let yc;

timerId = setInterval(function() {
  if (line_x && xy === 0) {
    step = rand(3) + 2;
    xc = rand(XEND - 10) + 5;
    yc = rand(YEND - 10) + 5;
    console.log("Lines at", xc, yc);
  }

  if (line_x) {
    lcd12864.line(xy, 0, xc, yc, 1);
    lcd12864.line(xc, yc, xy, YEND, 1);
    xy += step;
    if (xy > XEND) {
      line_x = false;
      xy = 0;
    }
  }
  else {
    lcd12864.line(0, xy, xc, yc, 1);
    lcd12864.line(xc, yc, XEND, xy, 1);
    xy += step;
    if (xy > YEND) {
      line_x = true;
      xy = 0;
      lcd12864.clear();
    }
  }
}, 0);
