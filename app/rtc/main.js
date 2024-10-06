// RealTimeClock with DS1307

const rpigpio2 = require(process.cwd() + '/build/Debug/rpigpio2');

const gpio = new rpigpio2.GPIO();
const i2c = new rpigpio2.I2C();

gpio.init();
i2c.init(gpio, 1, 0x68);

function exitHandler(options, exitCode) {
  if (options.exit) {
    i2c.release();
    gpio.release();
    process.exit(0);
  }
}

process.stdin.resume();
process.on('SIGINT', exitHandler.bind(null, {exit: true}));   // ctrl+c
process.on('exit', exitHandler.bind(null, {cleanup: true}));  // app closing

// 2 byte for address
let rtc_addr_arbf2 = new ArrayBuffer(2);
let rtc_addr_data2 = new Uint8Array(rtc_addr_arbf2);

// 1 byte for address
let rtc_addr_arbf1 = new ArrayBuffer(1);
let rtc_addr_data1 = new Uint8Array(rtc_addr_arbf1);

// 8 byes from clock
let rtc_arbf8 = new ArrayBuffer(8);
let rtc_data8 = new Uint8Array(rtc_arbf8);

// initial data
// run only once after power on
{
  let rtc_arbf9 = new ArrayBuffer(9);
  let rtc_data9 = new Uint8Array(rtc_arbf9);

  rtc_data9[0] = 0x00; // from addr 0
  rtc_data9[1] = 0x00; // 00 sec
  rtc_data9[2] = 0x47; // 11 min
  rtc_data9[3] = 0x20; // 22 hour, in 24H mode
  rtc_data9[4] = 0x07; // SAT
  rtc_data9[5] = 0x05; // 04 day
  rtc_data9[6] = 0x10; // 10 month
  rtc_data9[7] = 0x24; // 2024 year
  rtc_data9[8] = 0x00; // SQWE off
  i2c.write(rtc_arbf9);
}

function timerInterval() {
  rtc_addr_data1[0] = 0x00;
  i2c.write(rtc_addr_arbf1);

  i2c.read(rtc_arbf8);
  let hexs = "";
  for (let i = 0; i < 8; i++) {
    let h1 = "00" + rtc_data8[i].toString(16);
    let h2 = h1.substring(h1.length-2);
    hexs = hexs + "0x" + h2 + " ";
  }
  console.log(hexs);
}

setInterval(timerInterval, 1000);
