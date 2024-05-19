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

const fonts = require("./font_led");

let xpos = 0, ypos = 0;
let ch = 32;

function timerInterval() {
  let idx = ch - 32;
  lcd12864.bitmap(xpos, ypos, 24, 32, fonts.led[idx]);
  xpos = xpos + 32;
  if (xpos > 128-32) {
    xpos = 0;
    ypos = ypos + 32;
  }
  if (ypos > 64 - 32) {
    xpos = 0;
    ypos = 0;
  }
  ch = ch + 1;
  if (ch >= 64) {
    ch = 32;
  }
}

setInterval(timerInterval, 1000);