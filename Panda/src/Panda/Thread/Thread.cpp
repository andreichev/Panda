#include "Panda/Application/PlatformDetection.hpp"
#include "Panda/Base/Base.hpp"

#include "Thread.hpp"

#ifdef PND_PLATFORM_POSIX
#    include <pthread.h>
#elif defined(PND_PLATFORM_LINUX)
#    include <sys/prctl.h>
#elif defined(PND_PLATFORM_WINDOWS)
#    include <windows.h>
#    include <limits.h>
#    include <errno.h>
#endif

namespace Panda {

struct ThreadInternal {
#ifdef PND_PLATFORM_WINDOWS
    static DWORD WINAPI threadFunc(LPVOID _arg);
    HANDLE m_handle;
    DWORD m_threadId;
#elif defined(PND_PLATFORM_POSIX)
    static void *threadFunc(void *_arg);
    pthread_t m_handle;
#endif // PND_PLATFORM_
};

#ifdef PND_PLATFORM_WINDOWS
DWORD WINAPI ThreadInternal::threadFunc(LPVOID _arg) {
    Thread *thread = (Thread *)_arg;
    int32_t result = thread->entry();
    return result;
}
#else
void *ThreadInternal::threadFunc(void *_arg) {
    Thread *thread = (Thread *)_arg;
    union {
        void *ptr;
        int32_t i;
    } cast;
    cast.i = thread->entry();
    return cast.ptr;
}
#endif // PND_PLATFORM_

Thread::Thread()
    : m_fn(NULL)
    , m_userData(NULL)
    , m_stackSize(0)
    , m_exitCode(0)
    , m_running(false) {
    PND_STATIC_ASSERT(sizeof(ThreadInternal) <= sizeof(m_internal));
    ThreadInternal *ti = (ThreadInternal *)m_internal;
#ifdef PND_PLATFORM_WINDOWS
    ti->m_handle = INVALID_HANDLE_VALUE;
    ti->m_threadId = UINT32_MAX;
#elif defined(PND_PLATFORM_POSIX)
    ti->m_handle = 0;
#endif // PND_PLATFORM_
}

Thread::~Thread() {
    if (m_running) {
        shutdown();
    }
}

bool Thread::init(ThreadFn _fn, void *_userData, uint32_t _stackSize, const char *_name) {
    PND_ASSERT(m_running == false, "Already running!", _name);

    m_fn = _fn;
    m_userData = _userData;
    m_stackSize = _stackSize;

    ThreadInternal *ti = (ThreadInternal *)m_internal;
#ifdef PND_PLATFORM_WINDOWS
    ti->m_handle = ::CreateThread(NULL, m_stackSize, (LPTHREAD_START_ROUTINE)ti->threadFunc, this, 0, NULL);
    if (ti->m_handle == nullptr) {
        return false;
    }
#elif defined(PND_PLATFORM_POSIX)
    int result;

    pthread_attr_t attr;
    result = pthread_attr_init(&attr);
    if (result != 0) {
        PND_CRITICAL("pthread_attr_init failed! {}", result);
        return false;
    }

    if (m_stackSize > 0) {
        result = pthread_attr_setstacksize(&attr, m_stackSize);

        if (result != 0) {
            PND_CRITICAL("pthread_attr_setstacksize failed! {}", result);
            return false;
        }
    }

    result = pthread_create(&ti->m_handle, &attr, &ti->threadFunc, this);

    if (result != 0) {
        PND_ERROR("pthread_attr_setschedparam failed! {}", result);
        return false;
    }
#else
#    error "Not implemented!"
#endif // PND_PLATFORM_

    m_running = true;

    if (_name != nullptr) {
        setThreadName(_name);
    }

    return true;
}

void Thread::shutdown() {
    PND_ASSERT(m_running, "Not running!", m_running);
    ThreadInternal *ti = (ThreadInternal *)m_internal;

#ifdef PND_PLATFORM_WINDOWS
    WaitForSingleObject(ti->m_handle, INFINITE);
    GetExitCodeThread(ti->m_handle, (DWORD *)&m_exitCode);
    CloseHandle(ti->m_handle);
    ti->m_handle = INVALID_HANDLE_VALUE;
#elif defined(PND_PLATFORM_POSIX)
    union {
        void *ptr;
        int32_t i;
    } cast;
    pthread_join(ti->m_handle, &cast.ptr);
    m_exitCode = cast.i;
    ti->m_handle = 0;
#endif // PND_PLATFORM_

    m_running = false;
}

bool Thread::isRunning() const {
    return m_running;
}

int32_t Thread::getExitCode() const {
    return m_exitCode;
}

void Thread::setThreadName(const char *_name) {
    ThreadInternal *ti = (ThreadInternal *)m_internal;
#if defined(PND_PLATFORM_OSX) || defined(PND_PLATFORM_IOS)
    pthread_setname_np(_name);
#elif defined(PND_PLATFORM_LINUX)
    prctl(PR_SET_NAME, _name, 0, 0, 0);
#elif defined(PND_PLATFORM_WINDOWS)
    // Try to use the new thread naming API from Win10 Creators update onwards if we have it
    typedef HRESULT(WINAPI * SetThreadDescriptionProc)(HANDLE, PCWSTR);
    SetThreadDescriptionProc SetThreadDescription =
        (SetThreadDescriptionProc)::GetProcAddress((HMODULE)GetModuleHandleA("Kernel32.dll"), "SetThreadDescription");
    // SetThreadDescriptionProc SetThreadDescription =
    //     dlsym<SetThreadDescriptionProc>((void *)GetModuleHandleA("Kernel32.dll"), "SetThreadDescription");

    if (NULL != SetThreadDescription) {
        uint32_t length = (uint32_t)strlen(_name) + 1;
        uint32_t size = length * sizeof(wchar_t);
        wchar_t *name = (wchar_t *)alloca(size);
        mbstowcs(name, _name, size - 2);
        SetThreadDescription(ti->m_handle, name);
    }
#    if PND_COMPILER_MSVC
#        pragma pack(push, 8)
    struct ThreadName {
        DWORD type;
        LPCSTR name;
        DWORD id;
        DWORD flags;
    };
#        pragma pack(pop)
    ThreadName tn;
    tn.type = 0x1000;
    tn.name = _name;
    tn.id = ti->m_threadId;
    tn.flags = 0;

    __try {
        RaiseException(0x406d1388, 0, sizeof(tn) / 4, reinterpret_cast<ULONG_PTR *>(&tn));
    } __except (EXCEPTION_EXECUTE_HANDLER) {}
#    endif // PND_COMPILER_MSVC
#else
    // TODO: implement for Android
#endif // PND_PLATFORM_
}

int32_t Thread::entry() {
#ifdef PND_PLATFORM_WINDOWS
    ThreadInternal *ti = (ThreadInternal *)m_internal;
    ti->m_threadId = ::GetCurrentThreadId();
#endif // PND_PLATFORM_WINDOWS

    int32_t result = m_fn(this, m_userData);
    return result;
}

} // namespace Panda