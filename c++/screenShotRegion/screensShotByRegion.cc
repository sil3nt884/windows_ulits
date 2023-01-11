#include <node.h>
#include <Windows.h>
#include <vector>
#include <v8.h>

using namespace v8;

namespace screenshot {

std::vector<unsigned char> takeScreenshotRegion(int x, int y, int width, int height) {
    // Get the dimensions of the screen

    // Create a device context for the screen
    HDC hdcScreen = GetDC(NULL);
    HDC hdcMemDC = CreateCompatibleDC(hdcScreen);

    // Create a compatible bitmap the size of the screen
    HBITMAP hbmScreen = CreateCompatibleBitmap(hdcScreen, width, height);

    // Select the bitmap into the compatible device context
    SelectObject(hdcMemDC, hbmScreen);

    // Bit block transfer the screen to our compatible bitmap
    BitBlt(hdcMemDC, 0, 0, width, height, hdcScreen, x, y, SRCCOPY);

    // Get the bitmap data
    BITMAPINFOHEADER bi = {0};
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = width;
    bi.biHeight = -height;
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;

    HDC hdc = GetDC(NULL);
    DWORD dwBmpSize = ((width * bi.biBitCount + 31) / 32) * 4 * height;
    HANDLE hDIB = GlobalAlloc(GHND, dwBmpSize);
    char *lpbitmap = (char *)GlobalLock(hDIB);
    GetDIBits(hdc, hbmScreen, 0,
        (UINT)height,
        lpbitmap,
        (BITMAPINFO *)&bi, DIB_RGB_COLORS);

    // Copy the bitmap data to the vector
    std::vector<unsigned char> bitmap(lpbitmap, lpbitmap + dwBmpSize);

    // Clean up
    GlobalUnlock(hDIB);
    ReleaseDC(NULL, hdc);
    DeleteDC(hdcScreen);
    DeleteDC(hdcMemDC);
    ReleaseDC(NULL, hdc);

    return bitmap;
}

void takeScreenshotWithRegion(const v8::FunctionCallbackInfo<v8::Value>& args) {
       v8::Isolate* isolate = args.GetIsolate();

	   if (args.Length() != 4) {
			Local<String> message = String::NewFromUtf8(isolate, "Expected 4 number as argument").ToLocalChecked();
                           isolate->ThrowException(Exception::TypeError(message));
      return;
    }

	   if (!args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber() || !args[3]
    ->IsNumber()) {
     Local<String> message = String::NewFromUtf8(isolate, "Expected args are not numbers").ToLocalChecked();
               isolate->ThrowException(Exception::TypeError(message));
     return;
    }


   Local<Context> context = isolate->GetCurrentContext();
    int x = args[0]->NumberValue(context).ToChecked();
    int y = args[1]->NumberValue(context).ToChecked();
    int width = args[2]->NumberValue(context).ToChecked();
    int height = args[3]->NumberValue(context).ToChecked();



       // Get the screenshot data
       std::vector<unsigned char> bitmap = takeScreenshotRegion(x,y,width,height);

      BITMAPINFOHEADER bih;
        memset(&bih, 0, sizeof(BITMAPINFOHEADER));
        bih.biSize = sizeof(BITMAPINFOHEADER);
        bih.biWidth = width;
        bih.biHeight = -height;
        bih.biPlanes = 1;
        bih.biBitCount = 32;
        bih.biCompression = BI_RGB;

        // Get the BITMAPINFO data
        BITMAPINFO bmi;
        memset(&bmi, 0, sizeof(BITMAPINFO));
        bmi.bmiHeader = bih;

        // Get the BITMAPFILEHEADER data
        BITMAPFILEHEADER bfh;
        memset(&bfh, 0, sizeof(BITMAPFILEHEADER));
        bfh.bfType = 0x4d42;
        bfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(BITMAPINFO) + bitmap.size();
        bfh.bfReserved1 = 0;
        bfh.bfReserved2 = 0;
        bfh.bfOffBits = sizeof(BITMAP);

    // Combine the BITMAPINFOHEADER, BITMAPINFO and bitmap data into a single byte array
     std::vector<unsigned char> data;
     data.reserve(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(BITMAPINFO) + bitmap.size());
     data.insert(data.end(), (unsigned char*)&bfh, (unsigned char*)&bfh + sizeof(BITMAPFILEHEADER));
     data.insert(data.end(), (unsigned char*)&bih, (unsigned char*)&bih + sizeof(BITMAPINFOHEADER));
     data.insert(data.end(), (unsigned char*)&bmi, (unsigned char*)&bmi + sizeof(BITMAPINFO));
     data.insert(data.end(), bitmap.begin(), bitmap.end());


       // Create the ArrayBuffer and copy the data
       auto buffer = v8::ArrayBuffer::New(isolate, data.size());
       memcpy(buffer->GetBackingStore()->Data(), &data[0], data.size());
       args.GetReturnValue().Set(buffer);

}

void init(v8::Local<v8::Object> exports) {
    NODE_SET_METHOD(exports, "takeScreenshotRegion", takeScreenshotWithRegion);
}

NODE_MODULE(screenshot, init)

}  // namespace screenshot
