//
// Test application: MCP23017 + I2C
//   Read switch H/L and turn LED H/L
//   MCP23017 GBPA7 -> Push switch
//   MCP23017 GBPB0 -> LED

// TODO fix load path
const rpigpio2 = require(process.cwd() + '/build/Debug/rpigpio2');

const gpio = new rpigpio2.GPIO();
const i2c = new rpigpio2.I2C();
const mcp23017 = new rpigpio2.MCP23017();

// command -> mcp23017 -> i2c (-> gpio)
gpio.init();
// Test MCP23017 is set address to 0x27
i2c.init(gpio, 1, 0x27);
// 1 is for I2C-1 as (SDA @ pin2, CLK @ pin3)
mcp23017.init(i2c);
mcp23017.modeA(0b10000000); // A7 as input
mcp23017.modeB(0b00000000); // B0 as output

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

let b_led_0 = 1;
let a_input = 0;
let a_old = a_input;

function timerInterval() {
  a_input = mcp23017.readA();
  b_led_0 = (a_input & 0b10000000) ? 0b00000000 : 0b00000001;
  mcp23017.writeB(b_led_0);
  if (a_input != a_old) {
    a_old = a_input;
    console.log("b_led_0:", b_led_0);
  }
}

setInterval(timerInterval, 100);
