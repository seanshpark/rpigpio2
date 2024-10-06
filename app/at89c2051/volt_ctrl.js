//
// Test application: MCP23017 + I2C
//   MCP23017 test for AT89c2051 Programmer
//   GPB0:  +5V -> RST
//   GPB1: +12V -> RST

// TODO fix load path
const rpigpio2 = require(process.cwd() + '/build/Debug/rpigpio2');

const gpio = new rpigpio2.GPIO();
const i2c = new rpigpio2.I2C();
const mcp23017 = new rpigpio2.MCP23017();

// command -> mcp23017 -> i2c (-> gpio)
gpio.init();
i2c.init(gpio, 1, 0x20);
// 1 is for I2C-1 as (SDA @ pin2, CLK @ pin3)
mcp23017.init(i2c);

function exitHandler(options, exitCode) {
  if (options.exit) {
    mcp23017.release();
    i2c.release();
    gpio.release();
    process.exit(0);
  }
}

process.stdin.resume();
process.on('SIGINT', exitHandler.bind(null, {exit: true}));   // ctrl+c
process.on('exit', exitHandler.bind(null, {cleanup: true}));  // app closing

// PA = low 8bit
// PB = high 8bit
let value = 0x0300;
mcp23017.write(value);
console.log("value = ", value >> 8)

//                                               GPB       GPA
//const RST_VPP_0V  = GPB1 | GPB0; // H H --> 0b 0000 0011 0000 0000
//const RST_VPP_5V  = GPB1;        // H L --> 0b 0000 0010 0000 0000
//const RST_VPP_12V = 0;           // L L --> 0b 0000 0000 0000 0000

// set index to 0, 1 or 2 to check voltage to RST pin
// each are 9, 5v, and S12v
let values = [0x0300, 0x0200, 0x0000];
let volts  = [  "0v",   "5v",  "12v"];
let idx = 2;

function timerInterval() {
  let value = values[idx];
  mcp23017.write(value);

  let vv = value >> 8;
  console.log("value = ", idx, vv, volts[idx]);
  //idx = idx + 1;
  //if (idx >= values.length) {
  //  idx = 0;
  //}
}

setInterval(timerInterval, 2500);
