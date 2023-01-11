#include <node.h>
#include <Windows.h>
#include <v8.h>

using namespace v8;

void getMousePosition(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  POINT cursorPos;

  if (GetCursorPos(&cursorPos)) {
    Local<Context> context = isolate->GetCurrentContext();
    Local<Object> result = Object::New(isolate);
    result->Set(context, String::NewFromUtf8(isolate, "x").ToLocalChecked(), Number::New(isolate, cursorPos.x));
    result->Set(context, String::NewFromUtf8(isolate, "y").ToLocalChecked(), Number::New(isolate, cursorPos.y));
    args.GetReturnValue().Set(result);
  }
}

void Init(Local<Object> exports) {
    NODE_SET_METHOD(exports, "getMousePosition", getMousePosition);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Init)
