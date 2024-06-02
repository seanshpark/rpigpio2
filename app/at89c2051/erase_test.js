//
// Test application: MCP23017 + I2C
//   MCP23017 test for AT89c2051 Programmer

const rpigpio2 = require(process.cwd() + '/build/Debug/rpigpio2');

const gpio = new rpigpio2.GPIO();
const i2c = new rpigpio2.I2C();
const mcp23017 = new rpigpio2.MCP23017();

// command -> mcp23017 -> i2c (-> gpio)
gpio.init();
i2c.init(gpio, 1, 0x20);
// 1 is for I2C-1 as (SDA @ pin2, CLK @ pin3)
mcp23017.init(i2c);
mcp23017.modeA(0b11111111); // GPA as input for data read
mcp23017.modeB(0b00000000); // GPB as output for control

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

const GPB0 = 0b00000001;
const GPB1 = 0b00000010;
const GPB2 = 0b00000100;
const GPB3 = 0b00001000;
const GPB4 = 0b00010000;
const GPB5 = 0b00100000;
const GPB6 = 0b01000000;
const GPB7 = 0b10000000;

const RST_VPP_0V  = GPB1 | GPB0; // H H
const RST_VPP_5V  = GPB1; // H L
const RST_VPP_12V = 0; // L L

const P3_2 = GPB2;
const P3_3 = GPB3;
const P3_4 = GPB4;
const P3_5 = GPB5;
const P3_7 = GPB7;
const XTAL1 = GPB6;

// ERASE MODE
// - RST/VPP to 12V
// - P3.2 H->L->H for 10ms
// - P3.3 H, P3.4 L, P3.5 L, P3.7 L
// - PEROM Address is set to 0x0000 on rising edoge of RST
// - Address is incremented at positive puls of XTAL1
const ERASE_MODE = RST_VPP_5V | P3_2 | P3_3;
const POWER_OFF = RST_VPP_0V;

// READ Sequence
// - RST to 5V
// - Set erase mode
// - RST to 12V
// - P3.2: H->L->H for 10msec

function sleep(ms) {
  return new Promise((resolve) => setTimeout(resolve, ms));
}

console.log("== Begin erase");
let gpb_data = ERASE_MODE;
mcp23017.writeB(gpb_data);
sleep(10);

console.log("== Erase VPP +12V");
// VPP +12V
gpb_data = (gpb_data & ~RST_VPP_0V) | RST_VPP_12V;
mcp23017.writeB(gpb_data);
sleep(10);

console.log("== Erase Prog H");
// P3_2 L
gpb_data = gpb_data & ~P3_2;
mcp23017.writeB(gpb_data)
sleep(20);

console.log("== Erase Prog L");
// P3_2 H
gpb_data = gpb_data | P3_2;
mcp23017.writeB(gpb_data)
sleep(1000);
// P3.1 will be L (BUSY) while erase
// just wait 1000msec for now

console.log("== Erase VPP 0V");
// VPP +0V
gpb_data = (gpb_data & ~RST_VPP_12V) | POWER_OFF;
mcp23017.writeB(gpb_data);

console.log("== Erase End");

let options = {};
options.exit = true;
exitHandler(options, 0);
