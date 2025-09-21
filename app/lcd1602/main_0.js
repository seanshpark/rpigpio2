//
// Test application: LCD1602(HD44780) + PCF8574 + I2C
//

// TODO fix load path
const rpigpio2 = require(process.cwd() + '/build/Debug/rpigpio2');

const gpio = new rpigpio2.GPIO();
const i2c = new rpigpio2.I2C();
const pcf8574 = new rpigpio2.PCF8574();
const lcd1602 = new rpigpio2.LCD1602();

// command -> lcd1602 -> pcf8574 -> i2c (-> gpio)
gpio.init();
//i2c.init(gpio, 1, 0x27);
i2c.init(gpio, 1, 0x3f);
// 1 is for I2C-1 as (SDA @ pin2, CLK @ pin3)
// 0x27 is for green backlight LCD1602
pcf8574.init(i2c);
lcd1602.init(pcf8574);

function exitHandler(options, exitCode) {
  if (options.exit) {
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

var action = 0;

lcd1602.puts('Running...');

function timerInterval() {
  console.log('lcd1602/main.js action=', action);

  switch (action) {
    case 0:
      lcd1602.clear();
      lcd1602.cursor(true);
      lcd1602.blink(true);
      lcd1602.puts('Hello');
      action = action + 1;
      break;

    case 1:
      lcd1602.cursor(true);
      lcd1602.blink(false);
      lcd1602.move(1, 0);
      lcd1602.puts('Raspberry Pi');
      action = action + 1;
      break;

    case 2:
      lcd1602.clear();
      lcd1602.cursor(false);
      lcd1602.blink(false);
      lcd1602.move(0, 0);
      lcd1602.puts('Hello World!!!');
      action = action + 1;
      break;

    case 3:
      lcd1602.move(1, 0);
      lcd1602.cursor(false);
      lcd1602.blink(true);
      lcd1602.puts('^___^');
      action = action + 1;
      break;

    case 4:
      action = 0;
      break;
  }
}

setInterval(timerInterval, 2000);
