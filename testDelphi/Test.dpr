program Test;

{$APPTYPE CONSOLE}

uses
  Windows, SysUtils, tlhelp32;

type
  PMemoryBasicInformation64 = ^TMemoryBasicInformation64;
  {$ALIGN ON}
  _MEMORY_BASIC_INFORMATION64 = record
    BaseAddress: LONGLONG;
    AllocationBase: LONGLONG;
    AllocationProtect: DWORD;
    _alignment1: DWORD;
    RegionSize: LONGLONG;
    State: DWORD;
    Protect: DWORD;
    _Type: DWORD;
    _alignment2: DWORD;
  end;
  TMemoryBasicInformation64 = _MEMORY_BASIC_INFORMATION64;
  MEMORY_BASIC_INFORMATION64 = _MEMORY_BASIC_INFORMATION64;

  TSetLastNTError64     = procedure(Status: LONGLONG); cdecl;
  TGetLastError64       = function(): LONGLONG; cdecl;
  TVirtualQueryEx64     = function(hProcess: THandle; lpAddress: LONGLONG; var lpBuffer: TMemoryBasicInformation64; dwLength: Cardinal): Cardinal; cdecl;
  TVirtualProtectEx64   = function(hProcess: THandle; lpAddress: LONGLONG; dwSize, flNewProtect: DWORD; var lpflOldProtect: DWORD): BOOL; cdecl;
  TReadProcessMemory64  = function(hProcess: THandle; lpBaseAddress: LONGLONG; lpBuffer: Pointer; nSize: Cardinal; var lpNumberOfBytesRead: Cardinal): BOOL; cdecl;
  TWriteProcessMemory64 = function(hProcess: THandle; lpBaseAddress: LONGLONG; lpBuffer: Pointer; nSize: Cardinal; var lpNumberOfBytesWritten: Cardinal): BOOL; cdecl;

var
  WoW64ExtDLL: THandle = 0;
  SetLastNTError64: TSetLastNTError64 = nil;
  GetLastError64: TGetLastError64 = nil;
  VirtualQueryEx64: TVirtualQueryEx64 = nil;
  VirtualProtectEx64: TVirtualProtectEx64 = nil;
  ReadProcessMemory64: TReadProcessMemory64 = nil;
  WriteProcessMemory64: TWriteProcessMemory64 = nil;

  WOTProcInfo: PProcessInformation;
  WOTStartInfo: TStartupInfo;
  CommLine: string;

const
  WOT_EXE      = 'c:\Games\World_of_Tanks\win64\WorldOfTanks.exe';
  WOT_PARAMS   = '';
  SIGNATURE    = '00 00 48 8B 01 BA 03';
  START_OFFSET = $140000000;
  CODE_SIZE    = 10737418240;

type
  RSign = array of Integer;
  TByteArray = array of Byte;

  function GetHexRSign(const SignHexText: string): RSign;
  var i: Integer;
      Str, Hex: string;
  begin
  Str:=SignHexText;
  if Str[1]<>'$' then
   begin
   if Str[1]<>' ' then
    Str:=' '+Str;
   Str:=StringReplace(Str, ' ', '$', [rfReplaceAll]);
   end;
  SetLength(Result, Length(Str) div 3);
  for i:=1 to Length(Result) do
   begin
   Hex:=Copy(Str, 3*i-2, 3);
   if Hex[2]='X' then Result[i-1]:=-1
   else Result[i-1]:=Byte(StrToIntDef(Hex,0));
   end;
  end;

var
  hProcess: THandle;
  Addr: Int64;
  BytesRead: Cardinal;
  Mbi64: TMemoryBasicInformation64;
  Sign: RSign;
  Value, i, j: Cardinal;
  Buf: TByteArray;

begin

WoW64ExtDLL:=LoadLibrary(PChar(ExtractFilePath(ParamStr(0))+'..\bin\wow64ext.dll'));
if WoW64ExtDLL<>0 then
 begin
 @SetLastNTError64:=GetProcAddress(WoW64ExtDLL, 'SetLastErrorFromX64Call');
 @GetLastError64:=GetProcAddress(WoW64ExtDLL, 'GetLastErrorFromX64Call');
 @VirtualQueryEx64:=GetProcAddress(WoW64ExtDLL, 'VirtualQueryEx64');
 @VirtualProtectEx64:=GetProcAddress(WoW64ExtDLL, 'VirtualProtectEx64');
 @ReadProcessMemory64:=GetProcAddress(WoW64ExtDLL, 'ReadProcessMemory64');
 @WriteProcessMemory64:=GetProcAddress(WoW64ExtDLL, 'WriteProcessMemory64');
 end;

if Assigned(SetLastNTError64) then
 SetLastNTError64(0);

if Assigned(VirtualQueryEx64) and Assigned(ReadProcessMemory64) then
 begin

 New(WOTProcInfo);
 FillChar(WOTStartInfo, SizeOf(TStartupInfo), #0);
 with WOTStartInfo do
  begin
  cb:=SizeOf(StartupInfo);
  dwFlags:=STARTF_USESHOWWINDOW or STARTF_FORCEONFEEDBACK;
  wShowWindow:=SW_HIDE;
  end;

 CommLine:=Format('"%s" %s', [WOT_EXE, WOT_PARAMS]);
 if not CreateProcess(nil, PChar(CommLine), nil, nil, False, CREATE_NEW_CONSOLE or NORMAL_PRIORITY_CLASS,
                      nil, PChar(ExtractFilePath(WOT_EXE)), WOTStartInfo, WOTProcInfo^) then Exit;
 Sleep(5000);

 hProcess:=WOTProcInfo^.hProcess;
 Addr:=START_OFFSET;
 Sign:=GetHexRSign(SIGNATURE);
 Value:=Length(Sign);
 while VirtualQueryEx64(hProcess, Addr, Mbi64, SizeOf(Mbi64))<>0 do
  begin
  if (Mbi64.RegionSize>=Value)and(MBi64.State=MEM_COMMIT)and(not(MBi64.Protect=PAGE_GUARD))and(not(MBi64.Protect=PAGE_NOACCESS)) then
   begin
   SetLength(Buf, Mbi64.RegionSize);
   if ReadProcessMemory64(hProcess, Mbi64.BaseAddress, Buf, Mbi64.RegionSize, BytesRead) then
    begin
    i:=0;
    while i<=(BytesRead-Value) do
     begin
     if Buf[i]=Sign[0] then
      begin
      j:=1;
      while j<Value do
       begin
       if (Sign[j]=-1)or(Buf[i+j]=Sign[j]) then
        begin
        if j=(Value-1) then
         begin
         Writeln(IntToHex(Int64(Mbi64.BaseAddress) + i, 16));
         Readln;
         Exit;
         end;
        end
       else Break;
       Inc(j);
       end;
      end;
     Inc(i);
     end;
    end;
   end;
  Addr:=Addr+Mbi64.RegionSize;
  if Addr>=(START_OFFSET+CODE_SIZE) then Exit;
  end;

 end;

if Assigned(GetLastError64) then
 Writeln(GetLastError64);

Readln;

if WoW64ExtDLL<>0 then
 FreeLibrary(WoW64ExtDLL);

end.
