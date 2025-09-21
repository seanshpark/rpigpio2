//
// test of custom font
//

const rpigpio2 = require(process.cwd() + '/build/Debug/rpigpio2');

const gpio = new rpigpio2.GPIO();
const i2c = new rpigpio2.I2C();
const pcf8574 = new rpigpio2.PCF8574();
const lcd1602 = new rpigpio2.LCD1602();

// command -> lcd1602 -> pcf8574 -> i2c (-> gpio)
gpio.init();
//i2c.init(gpio, 1, 0x27); // green
i2c.init(gpio, 1, 0x3f); // blue
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

lcd1602.clear();
lcd1602.cursor(true);

// add custom fonts
const custom_fonts_1 = new Uint8Array([0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f]);
const custom_fonts_2 = new Uint8Array([0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x1f]);
const custom_fonts_3 = new Uint8Array([0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x1f, 0x1f]);
const custom_fonts_4 = new Uint8Array([0x00, 0x00, 0x00, 0x00, 0x1f, 0x1f, 0x1f, 0x1f]);
const custom_fonts_5 = new Uint8Array([0x00, 0x00, 0x00, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f]);
const custom_fonts_6 = new Uint8Array([0x00, 0x00, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f]);
const custom_fonts_7 = new Uint8Array([0x00, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f]);
const custom_fonts_8 = new Uint8Array([0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f]);

let custom_fonts = [
  custom_fonts_1, custom_fonts_2, custom_fonts_3, custom_fonts_4,
  custom_fonts_5, custom_fonts_6, custom_fonts_7, custom_fonts_8
];

let ch = 0;
for (ch = 0; ch < 8; ch++) {
  lcd1602.cgram(ch, custom_fonts[ch]);
}

let cx = 0;
let cy = 0;
lcd1602.move(cy, cx);
for (ch = 0; ch < 8; ch++) {
  lcd1602.putc(ch);
}
cy = 1;
lcd1602.move(cy, cx);
lcd1602.puts("Hello!");

function timerInterval() {
}

// setInterval(timerInterval, 100);
