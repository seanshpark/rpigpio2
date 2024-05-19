// TODO fix load path
const rpigpio2 = require(process.cwd() + '/build/Debug/rpigpio2');

const gpio = new rpigpio2.GPIO();
const i2c = new rpigpio2.I2C();
const mcp23017 = new rpigpio2.MCP23017();
const lcd12864 = new rpigpio2.LCD12864();

// command -> lcd12864 -> mcp23017 -> i2c (-> gpio)
gpio.init();
i2c.init(gpio, 1, 0x20);
// 1 is for I2C-1 as (SDA @ pin2, CLK @ pin3)
mcp23017.init(i2c);
lcd12864.init(mcp23017);

function exitHandler(options, exitCode) {
  if (options.exit) {
    lcd12864.release();
    mcp23017.release();
    i2c.release();
    gpio.release();
    process.exit(0);
  }
}

process.stdin.resume();
process.on('SIGINT', exitHandler.bind(null, {exit: true}));   // ctrl+c
process.on('exit', exitHandler.bind(null, {cleanup: true}));  // app closing

const fonts = require("./font_bm_roboto_bold");

let l = 0;
let fl = fonts.font.length;
for (l = 0; l < fl; l++) {
  let bmp = fonts.font[l];
  console.log(l, "l=", bmp.length, fonts.width[l], bmp.length / fonts.width[l]);
}

let xpos, ypos;
let fontbmp;
let xxww = 32;

xpos = 0;
ypos = 0;

let bitmap32 = Array();

for (l = 0; l < 11; l++) {
  fontbmp = fonts.font[l];
  // 4 * 48 = 192;
  let fontbmp32 = new Uint8Array(192);

  // copy 4x48 from source (32 pixel x 48 line)
  for (let y = 0; y < 48; y++) {
    for (let x = 0; x < 4; x++) {
      let idxsrc = y * 6 + x;
      let idxdst = y * 4 + x;
      fontbmp32[idxdst] = fontbmp[idxsrc];
    }
  }
  bitmap32.push(fontbmp32);
}

function read_time() {
  const now = new Date;
  const hh = now.getHours();
  const mm = now.getMinutes();
  let hs = `0${hh}`; hs = hs.substring(hs.length - 2);
  let ms = `0${mm}`; ms = ms.substring(ms.length - 2);

  //const mi = now.getMilliseconds();
  //const blink = mi > 500 ? true : false;
  const se = now.getSeconds();
  const blink = se & 1;
  let timenow;

  if (blink)
    timenow = `${hs} ${ms}`;
  else
    timenow = `${hs}:${ms}`;

  return timenow;
}

lcd12864.display(true);

function timerInterval() {
  let timenow = read_time();

  let xpos = 0;
  let ypos = 16;
  for (let p=0; p<5; p++) {
    if (p == 2) continue;
    let ch = timenow.charCodeAt(p);
    let va = ch - '0'.charCodeAt(0)
    console.log(va);

    bmp = bitmap32[va];
    lcd12864.bitmap(xpos, ypos, 32, 48, bmp);
    xpos = xpos + 32;
  }
}

lcd12864.graphic(true);
lcd12864.clear();

setInterval(timerInterval, 1500);