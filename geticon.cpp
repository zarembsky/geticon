#include "geticon.h"
#include "base64.h"

std::string geticon::geticon(int pid) {
  HICON hIcon = NULL;
  char outBuffer[10000];

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
	if (hProcess) {
	__try {
			SetLastError(0);
			char buffer[MAX_PATH + 1];
      LPSTR pBuffer = &buffer[ 0 ];
	 		DWORD dwSize = _countof(buffer);
	 		BOOL bRet = QueryFullProcessImageNameA(hProcess, 0, pBuffer, &dwSize);
			if (!bRet) {
				DWORD errCode = GetLastError();
        itoa((int)errCode, outBuffer, 10);
				return outBuffer;
			}
      WORD index = 0;
      hIcon = ExtractAssociatedIconA(NULL, pBuffer, &index);
      if(hIcon) {
        // Create the IPicture intrface
        PICTDESC desc = { sizeof(PICTDESC) };
        desc.picType = PICTYPE_ICON;
        desc.icon.hicon = hIcon;
        IPicture* pPicture = 0;
        HRESULT hr = OleCreatePictureIndirect(&desc, IID_IPicture, FALSE, (void**)&pPicture);
        if (FAILED(hr)) return "ERROR 1";

        // Create a stream and save the image
        IStream* pStream = 0;
        CreateStreamOnHGlobal(0, TRUE, &pStream);
        LONG cbSize = 0;
        hr = pPicture->SaveAsFile(pStream, TRUE, &cbSize);
        if(FAILED(hr)) return "ERROR 2";
        // Write the stream content buffer
        HGLOBAL hBuf = 0;
        GetHGlobalFromStream(pStream, &hBuf);
        void* buffer = GlobalLock(hBuf);
        std::string retVal = base64_encode((unsigned char const*)buffer, cbSize);

        HANDLE hFile = CreateFileA("c:/tempICO/addon.ico", GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
        if (!hFile) hr = HRESULT_FROM_WIN32(GetLastError());
        else {
            DWORD written = 0;
            WriteFile(hFile, buffer, cbSize, &written, 0);
            CloseHandle(hFile);
        }
        GlobalUnlock(buffer);

        return retVal;
      }
//////////////////////////////////////////////////////        
    }
    __finally {
      CloseHandle(hProcess);
    }
  }

  char buffer[128];
  strcpy(buffer, "Unknown");
  // if(hIcon) {
  //   itoa((int)hIcon, buffer, 10);
  // }
  return buffer;
}

Napi::String geticon::GetIconWrapped(const Napi::CallbackInfo& info) 
{
  Napi::Env env = info.Env();
  if (info.Length() < 1 || !info[0].IsNumber()) {
    Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
  }
  Napi::Number pid = info[0].As<Napi::Number>();
  Napi::String returnValue = Napi::String::New(env, geticon::geticon(pid.Int32Value()));
  
  return returnValue;
}

Napi::Object geticon::Init(Napi::Env env, Napi::Object exports) 
{
  exports.Set("geticon", Napi::Function::New(env, geticon::GetIconWrapped)
  );
 
  return exports;
}

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  return geticon::Init(env, exports);
}

NODE_API_MODULE(geticon, InitAll)