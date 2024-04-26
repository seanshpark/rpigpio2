//
// Test application: LCD1602 , CPU Temperature + Clock
//

let fs = require('fs');
let os = require('os');

// TODO fix load path
const rpigpio2 = require(process.cwd() + '/build/Debug/rpigpio2');

const gpio = new rpigpio2.GPIO();

const i2cpcf = new rpigpio2.I2C();
const pcf8574 = new rpigpio2.PCF8574();
const lcd1602 = new rpigpio2.LCD1602();

gpio.init();

i2cpcf.init(gpio, 1, 0x27);
// 1 is for I2C-1 as (SDA @ pin2, CLK @ pin3)
// 0x27 is for green backlight LCD1602
// 0x3f is for white backlight on blue LCD1602
pcf8574.init(i2cpcf);
lcd1602.init(pcf8574);

process.stdin.resume();
process.on('SIGINT', exitHandler.bind(null, {exit: true}));   // ctrl+c
process.on('exit', exitHandler.bind(null, {cleanup: true}));  // app closing

const _temp_file = "/sys/class/thermal/thermal_zone0/temp";

let clockTempObj = null;

class ClockTemp {
  constructor(c) {
    this._temperature = "00.00";
    this._cpuload = "000 000 000 000";
    this._timerId = null;
    this._oldCPUTime = 0;
    this._oldCPUIdle = 0;
  }

  readTime() {
    const now = new Date;
    const hh = now.getHours();
    const mm = now.getMinutes();
    let hs = `0${hh}`; hs = hs.substring(hs.length - 2);
    let ms = `0${mm}`; ms = ms.substring(ms.length - 2);

    //const mi = now.getMilliseconds();
    //const blink = mi > 500 ? true : false;
    const se = now.getSeconds();
    const blink = se & 1;

    if (blink)
      this._timenow = `${hs} ${ms}`;
    else
      this._timenow = `${hs}:${ms}`;
  }

  readTemp() {
    let self = this;
    fs.readFile(_temp_file, 'ascii', function(err, data) {
      if (err) return;
      let strcuts = data.split('\n');
      let temperature = parseInt(strcuts[0]) / 1000.0;
      let strTemp = temperature.toPrecision(4);

      self._temperature = strTemp.substring(0, 2) + '.' + strTemp.substring(3, 5);
    });
  }

  readCpuInfo() {
    let totalTime = -this._oldCPUTime;
    let totalIdle = -this._oldCPUIdle;
    let cpus = os.cpus();
    for(let i = 0, len = cpus.length; i < len; i++) {
      let cpu = cpus[i];
      for(let type in cpu.times) {
        totalTime += cpu.times[type];
      }
      totalIdle += cpu.times['idle'];
    }
    this._oldCPUTime = totalTime;
    this._oldCPUIdle = totalIdle;

    let speed = cpus[0].speed;
    let loadper = 100 - Math.round(100 * totalIdle / totalTime);
    let loadstr = '000' + loadper.toString()
    loadstr = loadstr.substring(loadstr.length - 3);
    this._cpuinfo = speed.toString() + ' ' + loadstr + '% ' + 
                    Math.round(os.freemem() / 1024 / 1024) + 'MB';

    //console.log(os.cpus());
    //console.log(os.totalmem());
    //console.log(os.freemem())
  }


  updateDisplay() {
    lcd1602.move(0, 0);
    // temperature value can be value of previous tick
    lcd1602.puts(this._temperature);
    lcd1602.putc(0xdf); // °
    lcd1602.puts('C');

    lcd1602.move(0, 11);
    lcd1602.puts(this._timenow);

    lcd1602.move(1,0);
    lcd1602.puts(this._cpuinfo);
  }

  refresh() {
    this.readTemp();
    this.readTime();
    this.readCpuInfo();
    this.updateDisplay();
  }

  begin() {
    let self = this;
    this._timerId = setInterval(function() {
      self.refresh();
    }, 1000);
  }

  end() {
    if (this._timerId !== null) {
      clearInterval(this._timerId);
      this._timerId = null;
    }
  }
}

// lcd1602.puts('Hello World!');
lcd1602.cursor(false);
lcd1602.blink(false);

clockTempObj = new ClockTemp();
clockTempObj.begin();

function exitHandler(options, exitCode) {
  if (options.exit) {
    clockTempObj.end();

    lcd1602.release();
    pcf8574.release();
    i2cpcf.release();
    gpio.release();
    process.exit(0);
  }
}
