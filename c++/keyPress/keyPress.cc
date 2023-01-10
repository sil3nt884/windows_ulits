#include <node.h>
#include <windows.h>

using namespace v8;

void sendKeyDownEvent(int keyCode) {
    INPUT input;
    // Press the key
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = keyCode;
    input.ki.dwFlags = 0;
    // Send the event
    SendInput(1, &input, sizeof(INPUT));
}

void sendKeyUpEvent(int keyCode) {
    INPUT input;
    // Release the key
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = keyCode;
    input.ki.dwFlags = KEYEVENTF_KEYUP;
    // Send the event
    SendInput(1, &input, sizeof(INPUT));
}

void SendKeyDownEventWrapper(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    if (args.Length() < 1 || !args[0]->IsNumber()) {
        Local<String> message = String::NewFromUtf8(isolate, "Expected 1 number as argument").ToLocalChecked();
                isolate->ThrowException(Exception::TypeError(message));
                return;
    }

    Local<Context> context = isolate->GetCurrentContext();
    int keyCode = args[0]->NumberValue(context).ToChecked();

    sendKeyDownEvent(keyCode);
}

void SendKeyUpEventWrapper(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    if (args.Length() < 1 || !args[0]->IsNumber()) {
     Local<String> message = String::NewFromUtf8(isolate, "Expected 1 number as argument").ToLocalChecked();
          isolate->ThrowException(Exception::TypeError(message));
          return;
    }

    Local<Context> context = isolate->GetCurrentContext();
    int keyCode = args[0]->NumberValue(context).ToChecked();

    sendKeyUpEvent(keyCode);
}

void init(Local<Object> exports) {
    NODE_SET_METHOD(exports, "sendKeyDownEvent", SendKeyDownEventWrapper);
    NODE_SET_METHOD(exports, "sendKeyUpEvent", SendKeyUpEventWrapper);
}

NODE_MODULE(keyboard, init)
