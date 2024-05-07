//
// Test application: CPU temperature to TM1637 + LED4x7Seg with clock pin 23, data pin 24
//
let fs = require('fs');
let os = require('os');

const rpigpio2 = require(process.cwd() + '/build/Debug/rpigpio2');
const gpiobj = new rpigpio2.GPIO();
gpiobj.init();

const tm1637 = new rpigpio2.TM1637();
tm1637.init(gpiobj, 23, 24);

const led4x7seg = new rpigpio2.LED4x7Seg();
led4x7seg.init(tm1637);

function exitHandler(options, exitCode) {
  if (options.exit) {
    led4x7seg.release();
    tm1637.release();
    gpiobj.release();
    process.exit(0);
  }
}

process.stdin.resume();
process.on('SIGINT', exitHandler.bind(null, {exit: true}));   // ctrl+c
process.on('exit', exitHandler.bind(null, {cleanup: true}));  // app closing

led4x7seg.clear();
led4x7seg.bright(2);

// for raspberry pi 5
const file_temp = '/sys/class/thermal/thermal_zone0/temp';

let led_mode = 2;
// 1 = cpu temperature  : ab:c° = ab.c °C
// 2 = available memory : ab cd = ab.cd GB
// 3 = time             : ab:cd
const led_mode_max = 3;

const timer_interval = 250;
const tick_per_sec = parseInt(1000 / timer_interval)
const mode_change_secs = 5;
const mode_change_ticks = tick_per_sec * mode_change_secs;

let cur_temperature = "30:0o";

function timerTemperature() {
  fs.readFile(file_temp, 'ascii', function(err, data) {
    if (err) return;
    let strcuts = data.split('\n');
    let temperature = parseInt(strcuts[0]) / 1000.0;
    let strTemp = temperature.toPrecision(4);
    cur_temperature = strTemp.substring(0, 2) + ':' + strTemp.substring(3, 4) + 'o';
  });
}

function read_temperature() {
  return cur_temperature;
}

function read_avil_mem() {
  let memfree = Math.round(os.freemem() / 1024 / 1024);
  let memfre2 = (memfree / 10.0).toFixed(1);
  let mfgbstr = `0${memfre2}`
  let mfgbst2 = mfgbstr.substring(0, 2) + ' ' + mfgbstr.substring(2, 4);
  let mfgbfmt = mfgbst2.substring(mfgbst2.length - 5);
  // console.log("!!!", memfree, memfre2, mfgbst2, mfgbfmt);
  return mfgbfmt;
}

function read_time() {
  const now = new Date;
  const hh = now.getHours();
  const mm = now.getMinutes();
  let hs = `0${hh}`; hs = hs.substring(hs.length - 2);
  let ms = `0${mm}`; ms = ms.substring(ms.length - 2);

  //const se = now.getSeconds();
  const mi = now.getMilliseconds();
  const blink = mi < 500;
  let result;
  if (blink)
    result = `${hs} ${ms}`;
  else
    result = `${hs}:${ms}`;
  return result;
}

let led_value;
let tick_mode_chg = mode_change_ticks;

function timerInterval() {
  let prev_value = "";
  switch (led_mode) {
    case 1:
      led_value = read_temperature();
      break;
    case 2:
      led_value = read_avil_mem();
      break;
    case 3:
      led_value = read_time();
      break;
  }
  if (prev_value !== led_value) {
    led4x7seg.show(led_value);
    prev_value = led_value;
  }

  tick_mode_chg = tick_mode_chg - 1;
  if (tick_mode_chg == 0)
  {
    led_mode = led_mode + 1;
    if (led_mode > led_mode_max)
      led_mode = 1;
    tick_mode_chg = mode_change_ticks;
  }
}

setInterval(timerTemperature, 100);
setInterval(timerInterval, timer_interval);
