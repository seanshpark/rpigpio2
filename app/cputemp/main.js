//
// Test application: CPU temperature to TM1637 + LED4x7Seg with clock pin 23, data pin 24
//
let fs = require('fs');
let os = require('os');
let net = require('net');

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

let cur_temperature = "30000";
let cur_avail_mem = "8190";

let clientConnection = null;
const TPC_SERVER_PORT = 8181;

let tcp_server = net.createServer(function(client) {
  client.setEncoding('utf8');
  clientConnection = client;

  client.on('data', function(data) {
    const data_str = data.toString();
    console.log("RECV:", data_str);
    const values = data_str.split(":");
    if (values.length >= 2) {
      if (values[0] === "TEMP")
        cur_temperature = values[1];
      else if (values[0] === "AMEM")
        cur_avail_mem = values[1]
    }
  });

  client.on('end', function() {
    console.log("client end");
    clientConnection = null;
  });

  client.on('error', function(err) {
    console.log('client error: ', JSON.stringify(err));
  });

  client.on('timeout', function() {
    console.log('client timeout');
  });
});

tcp_server.listen(TPC_SERVER_PORT, function() {
  console.log("Listen at", JSON.stringify(tcp_server.address()));

  tcp_server.on('close', function() {
    console.log("server close");
  });

  tcp_server.on('error', function(err) {
    console.log("server error", JSON.stringify(err));
  });
});

// use 'nc' command line tool as dummy client
// $ nc -v 192.168.1.2 8181

//-----------------------------------------------------------------------------
let led_mode = 2;
// 1 = cpu temperature  : ab:c° = ab.c °C
// 2 = available memory : ab cd = ab.cd GB
// 3 = time             : ab:cd
const led_mode_max = 3;

const timer_interval = 250;
const tick_per_sec = parseInt(1000 / timer_interval)
const mode_change_secs = 5;
const mode_change_ticks = tick_per_sec * mode_change_secs;

function fetchLocal() {
  if (clientConnection === null) {
    fs.readFile(file_temp, 'ascii', function(err, data) {
      if (err) return;
      let strcuts = data.split('\n');
      cur_temperature = strcuts[0];
    });
    let memfree = Math.round(os.freemem() / 1024 / 1024);
    cur_avail_mem = memfree.toString();
  }
}

function readTemperature() {
  return cur_temperature;
}

function readAvilMem() {
  return cur_avail_mem;
}

function readTime() {
  const now = new Date;
  const hh = now.getHours();
  const mm = now.getMinutes();
  let hs = `0${hh}`; hs = hs.substring(hs.length - 2);
  let ms = `0${mm}`; ms = ms.substring(ms.length - 2);
  //const se = now.getSeconds();
  if (now.getMilliseconds() < 500)
    return `${hs} ${ms}`;
  return `${hs}:${ms}`;
}

function formatTemperature(input) {
  let temp = parseInt(input) / 1000.0;
  let tempstr = temp.toPrecision(4);
  return tempstr.substring(0, 2) + ':' + tempstr.substring(3, 4) + 'o';
}

function formatAvailMem(input) {
  // input is in MB string
  //  8192 for  8192MB -> 08 19
  // 16384 for 16384MB -> 16 38
  let memfree = parseInt(input) / 10
  let mstr1 = '00000' + (memfree).toFixed(0);
  let mstr2 = mstr1.substring(mstr1.length - 4);
  let mstr3 = mstr2.substring(0, 2) + ' ' + mstr2.substring(2, 4);
  let mfmt = mstr3.substring(mstr3.length - 5);
  // console.log("!!!", memfree, mstr1, mstr2, mstr3, mfmt);
  return mfmt;
}

let led_value, value;
let tick_mode_chg = mode_change_ticks;

function timerInterval() {
  let prev_value = "";
  switch (led_mode) {
    case 1:
      value = readTemperature();
      led_value = formatTemperature(value);
      break;
    case 2:
      value = readAvilMem();
      led_value = formatAvailMem(value);
      break;
    case 3:
      led_value = readTime();
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

setInterval(fetchLocal, 100);
setInterval(timerInterval, timer_interval);
