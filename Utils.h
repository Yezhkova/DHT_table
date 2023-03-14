#pragma once
#include <cstddef>
#include <cstdint>
#include <map>
#include <iostream>
#include <mutex>

inline std::recursive_mutex gLogMutex;

#define HIDE_LOGS
// LOG
#ifndef QDBG 
    #ifndef HIDE_LOGS 
        #define LOG(expr) { \
		    const std::lock_guard<std::recursive_mutex> autolock(gLogMutex); \
		    std::cout << std::fixed << expr << std::endl << std::flush; \
        }
    #else 
        #define LOG(expr) {}
    #endif 

#else
#define LOG(expr) { \
            std::ostringstream out; \
            out << m_dbgOurPeerName << ": " << expr; \
            ??? \
    }
#endif

#ifndef QDBG
#define EX_LOG(expr) { \
            const std::lock_guard<std::recursive_mutex> autolock( gLogMutex ); \
            std::cout << std::fixed << expr << std::endl << std::flush; \
        }
#else
#define EX_LOG(expr) { \
            std::ostringstream out; \
            out << m_dbgOurPeerName << ": " << expr; \
            ??? \
    }
#endif



namespace Utils
{

	size_t pow2(size_t power);

	char hex(int symbol);

}
