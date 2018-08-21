
#pragma once
#include "Include.hpp"
extern "C"
{
    u32 __nx_applet_type = AppletType_None;
    char fake_heap[KIP_HEAP];
    void checkErrThrow(Result err)
    {
        if(R_SUCCEEDED(err)) return;
        Handle srv;
        while(R_FAILED(smGetServiceOriginal(&srv, smEncodeName("fatal:u")))) svcSleepThread(1000000000L);
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
    void __libnx_initheap()
    {
        extern char *fake_heap_start;
        extern char *fake_heap_end;
        fake_heap_start = fake_heap;
        fake_heap_end = fake_heap + KIP_HEAP;
    }
    void registerFspLr()
    {
        if(kernelAbove400()) return;
        Result rc = fsprInitialize();
        checkErrThrow(rc);
        u64 pid;
        svcGetProcessId(&pid, CUR_PROCESS_HANDLE);
        rc = fsprRegisterProgram(pid, KIP_TITLEID, FsStorageId_NandSystem, NULL, 0, NULL, 0);
        checkErrThrow(rc);
        fsprExit();
    }
    void __appInit()
    {
        Result rc;
        svcSleepThread(10000000000L);
        rc = smInitialize();
        checkErrThrow(rc);
        rc = fsInitialize();
        checkErrThrow(rc);
        registerFspLr();
        rc = fsdevMountSdmc();
        checkErrThrow(rc);
        rc = timeInitialize();
        checkErrThrow(rc);
        rc = hidInitialize();
        checkErrThrow(rc);
        Handle hdl;
        rc = smRegisterService(&hdl, KIP_NAME, false, 1);
        checkErrThrow(rc);
    }
    void __appExit()
    {
        smUnregisterService(KIP_NAME);
        fsdevUnmountAll();
        fsExit();
        smExit();
        timeExit();
        hidExit();
    }
}