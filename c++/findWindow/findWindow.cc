#include <node.h>
#include <v8.h>
#include <Windows.h>
#include <winuser.h>

using namespace v8;

int findWindowHandle(const char* className) {
    return (int)FindWindowA(NULL, className);
}

bool setForegroundWindow (HWND  hwid) {
    ShowWindow(hwid, SW_SHOWMAXIMIZED);
    return SetForegroundWindow(hwid);
}

void findWindowHandleWrapper(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    if (args.Length() < 1) {
      Local<String> message = String::NewFromUtf8(isolate, "Expected 1 number as argument").ToLocalChecked();
                     isolate->ThrowException(Exception::TypeError(message));
        return;
    }

    if (!args[0]->IsString()) {
       Local<String> message = String::NewFromUtf8(isolate, "Wrong type").ToLocalChecked();
                      isolate->ThrowException(Exception::TypeError(message));
        return;
    }

       String::Utf8Value param(isolate, args[0]);
       int result = findWindowHandle(*param);
       args.GetReturnValue().Set(result);


}

void setForegroundWindowWrapper(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

        if (args.Length() < 1) {
          Local<String> message = String::NewFromUtf8(isolate, "Expected 1 number as argument").ToLocalChecked();
                         isolate->ThrowException(Exception::TypeError(message));
            return;
        }

        if (!args[0]->IsNumber()) {
           Local<String> message = String::NewFromUtf8(isolate, "Wrong type").ToLocalChecked();
                          isolate->ThrowException(Exception::TypeError(message));
            return;
        }

      Local<Context> context = isolate->GetCurrentContext();
      int handle = args[0]->NumberValue(context).ToChecked();
      HWND hwnd = (HWND)handle;

      int result = setForegroundWindow(hwnd);


    args.GetReturnValue().Set(result);
}


void Init(Local<Object> exports) {
    NODE_SET_METHOD(exports, "findWindowHandle", findWindowHandleWrapper);
    NODE_SET_METHOD(exports, "setForegroundWindow", setForegroundWindowWrapper);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Init)
