//
// Test application: AT80C2051 as I2C target device
//

// TODO fix load path
const rpigpio2 = require(process.cwd() + '/build/Debug/rpigpio2');

const gpio = new rpigpio2.GPIO();
const i2c = new rpigpio2.I2C();

gpio.init();
i2c.init(gpio, 6, 0x11);
// 6 is for I2C-6

let arbr = new ArrayBuffer(5);
let data = new Uint8Array(arbr);
let val = 1;

function exitHandler(options, exitCode) {
  if (options.exit) {
    data[0] = 0xff;
    i2c.write(arbr);
    i2c.release();
    gpio.release();
    process.exit(0);
  }
}

process.stdin.resume();
process.on('SIGINT', exitHandler.bind(null, {exit: true}));   // ctrl+c
process.on('exit', exitHandler.bind(null, {cleanup: true}));  // app closing

function rand(range) {
  return Math.floor(Math.random() * range);
}

let dir = 0;
let ptr = 0;
function timerInterval() {
  //console.log(val);
  data[ptr] = val;
  i2c.write(arbr);

  if (dir == 0) {
    if (val == 0x40) {
      dir = 1;
      val = 0x20;
    } else {
      val = val << 1;
    }
  } else  {
    if (val == 0x01) {
      dir = 0;
      val = 0x02;
    } else {
      val = val >> 1;
    }
  }
  ptr = (ptr + 1) % 5;
}

setInterval(timerInterval, 100);
