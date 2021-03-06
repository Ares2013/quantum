/*
** Copyright 2018 Bloomberg Finance L.P.
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/
#ifndef QUANTUM_CONFIGURATION_H
#define QUANTUM_CONFIGURATION_H

#include <quantum/quantum_thread_traits.h>
#include <chrono>

namespace Bloomberg {
namespace quantum {

//==============================================================================================
//                                 class Configuration
//==============================================================================================
/// @class class Configuration.
/// @brief Configuration parameters for the Quantum library.
class Configuration
{
public:
     enum class BackoffPolicy : int { Linear,        ///< Linear backoff
                                      Exponential }; ///< Exponential backoff (doubles every time)
    
    /// @brief Set the number of threads running coroutines.
    /// @param num The number of threads. Set to -1 to have one coroutine thread per core.
    ///            Default is -1.
    void setNumCoroutineThreads(int num);
    
    /// @brief Set the number of threads running IO tasks.
    /// @param num The number of threads. Default is 5.
    void setNumIoThreads(int num);
    
    /// @brief Indicate if coroutine threads should be pinned to a core.
    /// @param value True or False. Default is False.
    /// @note For best performance, the number of coroutine threads should
    ///       be <= the number of cores in the system.
    void setPinCoroutineThreadsToCores(bool value);
    
    /// @brief Load balancee the shared IO queues.
    /// @param value If set to true, posting to the 'any' IO queue will result in
    ///              the load being spread among N queues. This mode can provide higher
    ///              throughput if dealing with high task loads. Default is false.
    /// @note To achieve higher performance, the threads run in polling mode which
    ///       increases CPU usage even when idle.
    void setLoadBalanceSharedIoQueues(bool value);
    
    /// @brief Set the interval between IO thread polls.
    /// @param interval Interval in milliseconds. Default is 100ms.
    /// @note Setting this to a higher value means it may take longer to react to the first
    ///       IO task posted, and vice-versa if the interval is lower.
    void setLoadBalancePollIntervalMs(std::chrono::milliseconds interval);
    
    /// @brief Set a backoff policy for the shared queue polling interval.
    /// @param policy The backoff policy to use. Default is 'Linear'.
    void setLoadBalancePollIntervalBackoffPolicy(BackoffPolicy policy);
    
    /// @brief Set the number of backoffs.
    /// @param numBackoffs The number of backoff increments. Default is 0.
    ///                    When the number of backoffs is reached, the poll interval remains unchanged thereafter.
    void setLoadBalancePollIntervalNumBackoffs(size_t numBackoffs);
    
    /// @brief Get the number of coroutine threads.
    /// @return The number of threads.
    int getNumCoroutineThreads() const;
    
    /// @brief Get the number of IO threads.
    /// @return The number of threads.
    int getNumIoThreads() const;
    
    /// @brief Check to see if coroutine threads are pinned to cores or not.
    /// @return True or False.
    bool getPinCoroutineThreadsToCores() const;
    
    /// @brief Check if IO shared queues are load balanced or not.
    /// @return True or False.
    bool getLoadBalanceSharedIoQueues() const;
    
    /// @brief Get load balance shared queue poll interval.
    /// @return The number of milliseconds.
    std::chrono::milliseconds getLoadBalancePollIntervalMs() const;
    
    /// @brief Get the backoff policy in load balance mode.
    /// @return The backoff policy used.
    BackoffPolicy getLoadBalancePollIntervalBackoffPolicy() const;
    
    /// @brief Get the number of backoffs used.
    /// @return The number of backoffs.
    size_t getLoadBalancePollIntervalNumBackoffs() const;
    
private:
    int                         _numCoroutineThreads{-1};
    int                         _numIoThreads{5};
    bool                        _pinCoroutineThreadsToCores{false};
    bool                        _loadBalanceSharedIoQueues{false};
    std::chrono::milliseconds   _loadBalancePollIntervalMs{100};
    BackoffPolicy               _loadBalancePollIntervalBackoffPolicy{BackoffPolicy::Linear};
    size_t                      _loadBalancePollIntervalNumBackoffs{0};
};

}}

#include <quantum/impl/quantum_configuration_impl.h>

#endif //QUANTUM_CONFIGURATION_H
