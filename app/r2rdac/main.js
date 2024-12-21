//
// Test application: LED doc matrix (1088AS) + MCP23017 + I2C
//   MCP23017 GBPA -> R-2R DAC
//

// TODO fix load path
const rpigpio2 = require(process.cwd() + '/build/Debug/rpigpio2');

const gpio = new rpigpio2.GPIO();
const i2c = new rpigpio2.I2C();
const mcp23017 = new rpigpio2.MCP23017();

// command -> mcp23017 -> i2c (-> gpio)
gpio.init();
i2c.init(gpio, 6, 0x20);
// 1 is for I2C-6 as (SDA @ pin2, CLK @ pin3)
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

let da_value = 0x00;
let da_idx = 0x00;
let da_dir = 0;

function adjust_saw0() {
  if (da_dir == 0) {
    da_value = da_value + 1;
    if (da_value > 255) {
      da_value = 255;
      da_dir = 1;
    }
  }
  else {
    da_value = da_value - 1;
    if (da_value < 0) {
      da_value = 0;
      da_dir = 0;
    }
  }
}

function adjust_saw1() {
  da_value = da_value + 1;
  if (da_value > 255) {
    da_value = 0;
  }
}

function adjust_saw2() {
  da_value = da_value - 1;
  if (da_value < 0) {
    da_value = 255;
  }
}

function adjust_sin() {
  da_value = Math.sin(da_idx * Math.PI / 128) * 127 + 128;
  da_idx = (da_idx + 1) % 256;
}

function timerInterval() {
  mcp23017.write(da_value);
  adjust_sin();

  console.log(`v = ${da_value}`);
}

setInterval(timerInterval, 1);
