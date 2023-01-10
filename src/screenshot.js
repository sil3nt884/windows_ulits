 const screenshot  = require('../c++/build/Release/sceenshot')

async function takeScreenshot() {
    const byteArray = new Uint8Array(screenshot.takeScreenshot());
    return Buffer.from(byteArray)
}

module.exports =  {takeScreenshot}