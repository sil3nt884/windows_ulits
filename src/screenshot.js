 const screenshot  = require('../c++/build/Release/sceenshot')
 const screenshotRegion = require('../c++/screenShotRegion/build/Release/screenShotByRegion')
 const fs = require('fs')
async function takeScreenshot() {
    const byteArray = new Uint8Array(screenshot.takeScreenshot());
    return Buffer.from(byteArray)
}

async function takeScreenshotRegion (x,y, width, height)  {
    const byteArray = new Uint8Array(screenshotRegion.takeScreenshotRegion(x,y,width,height));
    return Buffer.from(byteArray)
 }


module.exports =  {takeScreenshot, takeScreenshotRegion}