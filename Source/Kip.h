#include <switch.h>

// All sysmodule stuff, leave Play.cpp for playback stuff
#define TITLE_ID 0x420000000000000F
#define HEAP_SIZE 0x000540000

extern "C" u32 __nx_applet_type = AppletType_None;
char fake_heap[HEAP_SIZE];

extern "C" void fatalLater(Result err)
{
    if(R_SUCCEEDED(err)) return;
    Handle srv;
    while (R_FAILED(smGetServiceOriginal(&srv, smEncodeName("fatal:u")))) svcSleepThread(1000000000L);
    IpcCommand c;
    ipcInitialize(&c);
    ipcSendPid(&c);
    
    struct
    {
        u64 magic;
        u64 cmd_id;
        u64 result;
        u64 unknown;
    } *raw;
    raw = ipcPrepareHeader(&c, sizeof(*raw));
    raw->magic = SFCI_MAGIC;
    raw->cmd_id = 1;
    raw->result = err;
    raw->unknown = 0;
    ipcDispatch(srv);
    svcCloseHandle(srv);
}

extern "C" void __libnx_initheap(void)
{
    extern char *fake_heap_start;
    extern char *fake_heap_end;
    fake_heap_start = fake_heap;
    fake_heap_end = fake_heap + HEAP_SIZE;
}

extern "C" void registerFspLr(void)
{
    if (kernelAbove400())
        return;

    Result rc = fsprInitialize();
    if (R_FAILED(rc))
        fatalLater(rc);

    u64 pid;
    svcGetProcessId(&pid, CUR_PROCESS_HANDLE);

    rc = fsprRegisterProgram(pid, TITLE_ID, FsStorageId_NandSystem, NULL, 0, NULL, 0);
    if (R_FAILED(rc))
        fatalLater(rc);
    fsprExit();
}

extern "C" void __appInit(void)
{
    Result rc;
    svcSleepThread(10000000000L);
    rc = smInitialize();
    if (R_FAILED(rc))
        fatalLater(rc);
    rc = fsInitialize();
    if (R_FAILED(rc))
        fatalLater(rc);
    registerFspLr();
    rc = fsdevMountSdmc();
    if (R_FAILED(rc))
        fatalLater(rc);
    rc = timeInitialize();
    if (R_FAILED(rc)) fatalLater(rc);
}

extern "C" void __appExit(void)
{
    fsdevUnmountAll();
    fsExit();
    smExit();
    audoutExit();
    timeExit();
}