//
// Test application: MCP23017 + I2C
//   MCP23017 test for AT89c2051 Programmer

const fs = require('node:fs');
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

// READ MODE
// - RST/VPP to 5V
// - P3.2 H, P3.3 L, P3.4 L, P3.5 H, P3.7 H
// - PEROM Address is set to 0x0000 on rising edoge of RST
// - Address is incremented at positive pulse of XTAL1
const READ_MODE = RST_VPP_5V | P3_2 | P3_5 | P3_7;
const POWER_OFF = RST_VPP_0V;

// READ Sequence
// - RST to 5V
// - Assume ADDR as 0x0000
// - READ from GPA
// - Pulse to XTAL1
// - Assume ADDR++
// - Loop until ADDR 2048

let gpa_data = 0;
let gpb_data = READ_MODE;

mcp23017.writeB(gpb_data);

let intervalID = 0;
let Address = 0x0000;
let AddrEnd = 0x0800; // 2048 byte
let AddrPtrS = 0x0000; // dump address start
let AddrPtrC = 0x0000; // dump buffer count
let buffer = [];
let buffpk = [];

function sleep(ms) {
  return new Promise((resolve) => setTimeout(resolve, ms));
}

function end_dump() {
  clearInterval(intervalID);
  console.log("Interval End");

  console.log("Buffer leng:", buffer.length);
  // shrink buffer from end if 0xff
  let buff_end = buffer.length - 1;
  while(buffer[buff_end] === 0xff) {
    buffer.pop();
    buff_end = buffer.length - 1;
  }
  console.log("Actual leng:", buffer.length);

  let bin_to_save = new Uint8Array(buffer.length);
  for (let idx = 0; idx < buffer.length; idx++) {
    bin_to_save[idx] = buffer[idx];
  }

  fs.writeFile(process.cwd() + "/read.bin", bin_to_save, function(err) {
    if (err) throw err;
    console.log("Save to read.bin done.");

    let options = {};
    options.exit = true;
    exitHandler(options, 0);
  });
  console.log("End of read.");
}

// TODO enlarge this to 2048
AddrEnd = 0x0200; // 512 byte

async function timerInterval() {
  if (Address < AddrEnd) {
    // read data
    gpa_data = mcp23017.readA();
    buffer.push(gpa_data);
    buffpk.push(gpa_data);

    // increment address
    gpb_data |= XTAL1;
    mcp23017.writeB(gpb_data);
    await sleep(0);
    gpb_data &= ~XTAL1;
    mcp23017.writeB(gpb_data);
    Address++;
    AddrPtrC++;

    // dump for 16 bytes
    const align = 16;
    if (AddrPtrC >= align) {
      let hex_str = Array.from(buffpk).map((b) => b.toString(16).padStart(2, "0")).join(" ");
      let hex_addr = AddrPtrS.toString(16).padStart(4,'0');
      console.log(hex_addr, hex_str);
      AddrPtrS = AddrPtrS + align;
      AddrPtrC = 0;
      buffpk = [];
    }

    if (Address >= AddrEnd) {
      gpb_data = POWER_OFF;
      mcp23017.writeB(gpb_data);
      end_dump();
    }
  }
}

intervalID = setInterval(timerInterval, 3);
