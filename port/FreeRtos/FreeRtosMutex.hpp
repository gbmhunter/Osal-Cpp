//!
//! @file				FreeRtosMutex.hpp
//! @author				Geoffrey Hunter <gbmhunter@gmail.com> (www.mbedded.ninja)
//! @created			2014-08-11
//! @last-modified		2014-10-31
//! @brief 				FreeRTOS implementation of a Mutex.
//! @details
//!						

#ifndef __cplusplus
	#error Please build with C++ compiler
#endif

//===============================================================================================//
//======================================== HEADER GUARD =========================================//
//===============================================================================================//

#ifndef MOSAL_FREERTOS_MUTEX_H
#define MOSAL_FREERTOS_MUTEX_H

//===============================================================================================//
//==================================== FORWARD DECLARATION ======================================//
//===============================================================================================//

namespace MbeddedNinja
{
	namespace MOsal
	{
		class FreeRtosMutex;
	}
}

//===============================================================================================//
//========================================= INCLUDES ============================================//
//===============================================================================================//

// System libraries
//#include <cstdint>		// int8_t, int32_t e.t.c
//#include <cstdio>			// snprintf()
//#include <cstdlib>		// realloc(), malloc(), free()
//#include <iostream>		// std::cin, cout, e.t.c

// User libraries

#include "MAssert/api/MAssertApi.h"

// FreeRTOS
#include "FreeRTOS/Source/include/FreeRTOS.h"
#include "FreeRTOS/Source/include/semphr.h"

// User source
#include "../../include/Mutex.h"

//===============================================================================================//
//======================================== NAMESPACE ============================================//
//===============================================================================================//

namespace MbeddedNinja
{
	namespace MOsal
	{

		//! @brief		A FreeRTOS implementation of a Mutex.
		//! @details	A FreeRTOS mutex is a binary semaphore with a priority inheritance mechanism. A FreeRTOS mutex is good for protecting a shared resource. A mutex is designed to be got and released from the same thread.
		//! @note		Inherits from Mutex. See the Mutex class for method descriptions.
		class FreeRtosMutex : public Mutex
		{

		public:

			//============================================================================================//
			//=============================== PUBLIC METHOD DEFINITIONS ==================================//
			//============================================================================================//

			FreeRtosMutex()
			{
				// Create mutex with FreeRTOS call
				this->mutexHandle = xSemaphoreCreateMutex();

				// Make sure the mutex was created
				M_ASSERT(this->mutexHandle);
			}

			~FreeRtosMutex()
			{
				// Handle is not null, so delete.
				vSemaphoreDelete(this->mutexHandle);
			}

			bool Lock(float timeoutPeriodMs)
			{
				// For storing conversion of float into TickType_t
				TickType_t tickTypeTimeoutPeriodInTicks;

				// If it less than 0, the user wants to wait indefinitely
				if(timeoutPeriodMs < 0)
				{
					// Overwrite with special constant
					// (note that the FreeRTOS macro "INCLUDE_vTaskSuspend" has to be set to 1)
					tickTypeTimeoutPeriodInTicks = portMAX_DELAY;
				}
				else
				{
					// User has specified non-negative timeout, so convert this from ms to ticks
					tickTypeTimeoutPeriodInTicks = (TickType_t)(timeoutPeriodMs/(float)portTICK_RATE_MS);

				}

				// Take the semaphore
				if(xSemaphoreTake(this->mutexHandle, tickTypeTimeoutPeriodInTicks) == pdPASS)
				{
					//CyDebugUart_PutString("Mutex obtained.");
					return true;
				}
				else
				{
					//CyDebugUart_PutString("Mutex was not obtained.");
					return false;
				}
			}

			bool Unlock()
			{

				if(xSemaphoreGive(this->mutexHandle) == pdPASS)
					return true;
				else
					return false;
			}

			//============================================================================================//
			//============================== PRIVATE METHOD DEFINITIONS ==================================//
			//============================================================================================//

			//! @brief		Internally saves the mutex handle assigned by the OS.
			void * mutexHandle;

		}; // class FreeRtosMutex : public Mutex
	
	} // namespace MOsalNs
} // namespace MbeddedNinja

#endif // #ifndef MOSAL_FREERTOS_MUTEX_H

// EOF
