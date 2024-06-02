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
mcp23017.modeA(0b00000000); // GPA as output for data write
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

// fixed to write "blink.bin" file
const filename = process.cwd() + "/blink.bin";
const filedata = fs.readFileSync(filename, null);
const file_leng = filedata.length;

let buff16 = [];
let i16 = 0;
let addr = 0;

function dump_buff16() {
  let hex_str = Array.from(buff16).map((b) => b.toString(16).padStart(2, "0")).join(" ");
  let hex_addr = addr.toString(16).padStart(4,'0');
  console.log(hex_addr, hex_str);
  buff16 = [];
  i16 = 0;
}

for (let idx = 0; idx < file_leng; idx++) {
  buff16[i16] = filedata[idx];
  i16++;
  if (i16 == 16) {
    dump_buff16();
    addr = idx + 1;
  }
}

if (i16 > 0) {
  dump_buff16();
}


// WRITE MODE
// - RST/VPP to 12V
// - P3.2 H->L->H, P3.3 L, P3.4 H, P3.5 H, P3.7 H
// - PEROM Address is set to 0x0000 on rising edoge of RST
// - Address is incremented at positive pulse of XTAL1
const WRITE_MODE_5V = RST_VPP_5V | P3_2 | P3_4 | P3_5 | P3_7;
const WRITE_MODE_12V = RST_VPP_12V | P3_2 | P3_4 | P3_5 | P3_7;
const READ_MODE = RST_VPP_5V | P3_2 | P3_5 | P3_7;
const POWER_OFF = RST_VPP_0V;

// WRITE Sequence
// - RST to 12V
// - Assume ADDR as 0x0000
// - WRITE data to GPA
// - Pulse to XTAL1
// - Assume ADDR++
// - Loop until ADDR 2048 and program length

let gpa_data = 0xff;
let gpb_data = WRITE_MODE_5V;
let gpa_veri = 0x00;

mcp23017.writeB(gpb_data);

let intervalID = 0;
let Address = 0x0000;
let AddrEnd = 0x0800; // 2048 byte

function sleep(ms) {
  return new Promise((resolve) => setTimeout(resolve, ms));
}

async function end_write() {
  clearInterval(intervalID);
  console.log("Write End");

  await sleep(1000);

  let options = {};
  options.exit = true;
  exitHandler(options, 0);
}

Address = 0;
AddrEnd = file_leng;

async function timerInterval() {
  if (Address < AddrEnd) {
    // VPP to 5V, write ready mode
    gpb_data = WRITE_MODE_5V;
    mcp23017.writeB(gpb_data);

    // data from file
    gpa_data = filedata[Address];
    mcp23017.writeA(gpa_data);
    await sleep(0);

    // VPP to 12V, write mode
    gpb_data = WRITE_MODE_12V;
    mcp23017.writeB(gpb_data)
    await sleep(1);
    
    // pulse P3_2
    gpb_data = gpb_data & ~P3_2;
    mcp23017.writeB(gpb_data)
    await sleep(5);
    gpb_data = gpb_data | P3_2;
    mcp23017.writeB(gpb_data)
    await sleep(5);

    // VPP to 5V, read mode
    gpb_data = READ_MODE;
    mcp23017.writeB(gpb_data)
    await sleep(1);

    // verify data
    mcp23017.modeA(0xff); // GPA as read
    await sleep(1);
    gpa_veri = mcp23017.readA();
    if (gpa_veri != gpa_data) {
      console.log("!!! data write error:", gpa_data, gpa_veri);
      gpb_data = POWER_OFF;
      mcp23017.writeB(gpb_data);
      end_write();
    }
    mcp23017.modeA(0x00); // GPA as write

    // increment address, exit if end
    gpb_data |= XTAL1;
    mcp23017.writeB(gpb_data);
    await sleep(1);
    gpb_data &= ~XTAL1;
    mcp23017.writeB(gpb_data);
    await sleep(1);

    Address++;
    if (Address >= AddrEnd) {
      gpb_data = POWER_OFF;
      mcp23017.writeB(gpb_data);
      end_write();
    }
  }
}

intervalID = setInterval(timerInterval, 50);
