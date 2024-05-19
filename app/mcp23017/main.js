//
// Test application: LED doc matrix (1088AS) + MCP23017 + I2C
//   MCP23017 GBPA -> LED 0 ~ 7
//   MCP23017 GBPB -> LED 8 ~ 15
//

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

// LED 8 x 8 Matirx pin description
//
// LED COL  : 01 02 03 04 05 06 07 08
// LED A pin: 13 03 04 10 06 11 15 16
// LED K pin: 09 14 08 12 01 07 02 05
// LED ROW  : 01 02 03 04 05 06 07 08

// LED A pin -> COL
// 13 0b 0001 0000 0000 0000 : 0x1000
// 03 0b 0000 0000 0000 0100 : 0x0004
// 04 0b 0000 0000 0000 1000 : 0x0008
// 10 0b 0000 0010 0000 0000 : 0x0200
// 06 0b 0000 0000 0010 0000 : 0x0020
// 11 0b 0000 0100 0000 0000 : 0x0400
// 15 0b 0100 0000 0000 0000 : 0x4000
// 16 0b 1000 0000 0000 0000 : 0x8000

// LED K pin -> ROW
// 09 0b 0000 0001 0000 0000 : 0x0100
// 14 0b 0010 0000 0000 0000 : 0x2000
// 08 0b 0000 0000 1000 0000 : 0x0080
// 12 0b 0000 1000 0000 0000 : 0x0800
// 01 0b 0000 0000 0000 0001 : 0x0001
// 07 0b 0000 0000 0100 0000 : 0x0040
// 02 0b 0000 0000 0000 0010 : 0x0002
// 05 0b 0000 0000 0001 0000 : 0x0010

const val_col = [0x1000, 0x0004, 0x0008, 0x0200, 0x0020, 0x0400, 0x4000, 0x8000];
const val_row = [0x0100, 0x2000, 0x0080, 0x0800, 0x0001, 0x0040, 0x0002, 0x0010];

// ROW L makes LED ON, H as OFF
// COL H makes LED ON

// get all other ROW as H to make them OFF
function row_value(row) {
  let exrow = 0;
  for (r = 0; r < 8; r++) {
    if (r == row)
      continue;
    exrow = exrow | val_row[r];
  }
  return exrow;
}

let idx_col = 0;
let idx_row = 0;
let inv_row = 0;

inv_row = row_value(idx_row);

// loop 8x8 with single led to turn on
// - loop row with OFF, others ON
// - loop col with ON, others OFF
function timerInterval() {
  let result = val_col[idx_col] | inv_row;
  mcp23017.write(result);
  
  idx_col++;
  if (idx_col >= 8) {
    idx_col = 0;
    idx_row++;
    if (idx_row >= 8) {
      idx_row = 0;
    }
    inv_row = row_value(idx_row);
  }
}

setInterval(timerInterval, 100);
