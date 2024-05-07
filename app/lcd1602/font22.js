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
i2c.init(gpio, 1, 0x27);
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
lcd1602.cursor(false);
lcd1602.blink(false);

lcd1602.move(0, 0);
lcd1602.puts('Hello!');

// font 1 LT
const custom_font_11 = new Uint8Array([0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18]);
// font 2 
const custom_font_20 = new Uint8Array([0x1f, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x1f]);
const custom_font_21 = new Uint8Array([0x1f, 0x1f, 0x03, 0x03, 0x03, 0x03, 0x1f, 0x1f]);
const custom_font_22 = new Uint8Array([0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1f, 0x1f]);
const custom_font_23 = new Uint8Array([0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x1f]);

const custom_font_33 = new Uint8Array([0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x1f, 0x1f]);

const custom_font_43 = new Uint8Array([0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03]);

const custom_font_51 = new Uint8Array([0x1f, 0x1f, 0x18, 0x18, 0x18, 0x18, 0x1f, 0x1f]);

const custom_font_71 = new Uint8Array([0x1f, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]);
const custom_font_72 = new Uint8Array([0x1f, 0x1f, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03]);

const custom_font_01 = new Uint8Array([0x1f, 0x1f, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18]);

const custom_fonts_all = [
  custom_font_11, // 0
  custom_font_20, // 1
  custom_font_21, // 2
  custom_font_22, // 3
  custom_font_23, // 4
  custom_font_33, // 5
  custom_font_43, // 6
  custom_font_51, // 7
  custom_font_71, // 8
  custom_font_72, // 9
  custom_font_01  // 10
];
// there are 11 bitmaps
// index is 0 ~ 10
// use null for empty, that is ascii code 32


const code_1_idx = [ null, 0, null, 0];
const code_2_idx = [ 1, 2, 3, 4];
const code_3_idx = [ 1, 2, 4, 5];
const code_4_idx = [ 3, 5, null, 6];
const code_5_idx = [ 7, 1, 4, 5];
const code_6_idx = [ 7, 1, 3, 5];
const code_7_idx = [ 8, 9, null, 6];
const code_8_idx = [ 7, 2, 3, 5];
const code_9_idx = [ 7, 2, 4, 5];
const code_0_idx = [ 10, 9, 3, 5];

const code_idx = [
  code_0_idx,
  code_1_idx,
  code_2_idx,
  code_3_idx,
  code_4_idx,
  code_5_idx,
  code_6_idx,
  code_7_idx,
  code_8_idx,
  code_9_idx
];



// each numbers have 4 codes for four bitmaps
let num_to_code = new Array(16);
let used = new Map()
let code_next = 0;

function prepare_num(digit, number) {
  let code_n_idx = code_idx[number];

  for (let i = 0; i < 4; i++) {
    let code;
    bmpidx = code_n_idx[i];
    if (bmpidx === null) {
      code = 32;
    } else  {
      if (used.has(bmpidx)) {
        code = used.get(bmpidx);
      } else {
        code = code_next
        code_next = code_next + 1;

        bmpdata = custom_fonts_all[bmpidx]
        lcd1602.cgram(code, bmpdata);
        used.set(bmpidx, code);
      }
    }
    let ptr = digit * 4 + i
    num_to_code[ptr] = code;
    console.log("cg:", ptr, bmpidx, code );
  }
}

prepare_num(0, 5);
c0 = num_to_code[0];
c1 = num_to_code[1];
c2 = num_to_code[2];
c3 = num_to_code[3];
console.log(c0, c1, c2, c3);
lcd1602.move(0, 0); lcd1602.putc(c0); lcd1602.putc(c1);
lcd1602.move(1, 0); lcd1602.putc(c2); lcd1602.putc(c3);

prepare_num(1, 6);
c0 = num_to_code[1*4+0];
c1 = num_to_code[1*4+1];
c2 = num_to_code[1*4+2];
c3 = num_to_code[1*4+3];
console.log(c0, c1, c2, c3);
lcd1602.move(0, 2); lcd1602.putc(c0); lcd1602.putc(c1);
lcd1602.move(1, 2); lcd1602.putc(c2); lcd1602.putc(c3);

prepare_num(2, 0);
c0 = num_to_code[2*4+0];
c1 = num_to_code[2*4+1];
c2 = num_to_code[2*4+2];
c3 = num_to_code[2*4+3];
console.log(c0, c1, c2, c3);
lcd1602.move(0, 5); lcd1602.putc(c0); lcd1602.putc(c1);
lcd1602.move(1, 5); lcd1602.putc(c2); lcd1602.putc(c3);

prepare_num(3, 9);
c0 = num_to_code[3*4+0];
c1 = num_to_code[3*4+1];
c2 = num_to_code[3*4+2];
c3 = num_to_code[3*4+3];
console.log(c0, c1, c2, c3);
lcd1602.move(0, 7); lcd1602.putc(c0); lcd1602.putc(c1);
lcd1602.move(1, 7); lcd1602.putc(c2); lcd1602.putc(c3);
