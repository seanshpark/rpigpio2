//
// Test application: LCD1602(HD44780) + PCF8574 + I2C
//

var fs = require('fs');

// TODO fix load path
const rpigpio2 = require(process.cwd() + '/build/Debug/rpigpio2');

const gpio = new rpigpio2.GPIO();
const i2c = new rpigpio2.I2C();
const pcf8574 = new rpigpio2.PCF8574();
const lcd1602 = new rpigpio2.LCD1602();

const tm1637 = new rpigpio2.TM1637();
const led4x7seg = new rpigpio2.LED4x7Seg();

// command -> lcd1602 -> pcf8574 -> i2c (-> gpio)
gpio.init();
i2c.init(gpio, 1, 0x27);
// 1 is for I2C-1 as (SDA @ pin2, CLK @ pin3)
// 0x27 is for green backlight LCD1602
pcf8574.init(i2c);
lcd1602.init(pcf8574);

tm1637.init(gpio, 23, 24);
led4x7seg.init(tm1637);

function exitHandler(options, exitCode) {
  if (options.exit) {
    led4x7seg.release();
    tm1637.release();
    lcd1602.release();
    pcf8574.release();
    i2c.release();
    gpio.release();
    process.exit(0);
  }
}

process.stdin.resume();
process.on('SIGINT', exitHandler.bind(null, {exit: true}));   // ctrl+c
process.on('exit', exitHandler.bind(null, {cleanup: true}));  // app closing

const filename = '/sys/class/thermal/thermal_zone0/temp';

lcd1602.puts('Hello World!');
lcd1602.cursor(false);
lcd1602.blink(false);

led4x7seg.clear();
led4x7seg.bright(2);

function timerInterval() {

  fs.readFile(filename, 'ascii', function(err, data) {
    if (err) return;
    let strcuts = data.split('\n');
    let temperature = parseInt(strcuts[0]) / 1000.0;
    let strTemp = temperature.toPrecision(4);
    let value = strTemp.substring(0, 2) + '.' + strTemp.substring(3, 5);
    console.log('T', value, '°C')
    led4x7seg.show(value);

    lcd1602.move(1, 0);
    lcd1602.puts('CPU ');
    lcd1602.puts(value);
    lcd1602.putc(0xdf); // °
    lcd1602.puts('C');
  });
}

setInterval(timerInterval, 1000);
