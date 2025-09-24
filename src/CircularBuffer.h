#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <vector>
#include <format>

#include "LoggerHelper.h"

template <typename T>
class CircularBuffer
{
    private:
        std::vector<T> m_Buffer;
        size_t m_ReadIdx; // Points to the oldest element
        size_t m_WriteIdx; // Points to the next insertion point
        bool m_Full;

        inline void IncrementReadIdx()
        {
            if (m_Buffer.capacity() > 0)
            {
                m_ReadIdx = (m_ReadIdx + 1) % m_Buffer.capacity(); 
            }
            else
            {
                m_ReadIdx++;
            }
        }

        inline void IncrementWriteIdx()
        {
            if (m_Buffer.capacity() > 0)
            {
                m_WriteIdx = (m_WriteIdx + 1) % m_Buffer.capacity();
            }
            else
            {
                m_WriteIdx++;
            }
        }

        inline void ExtendBuffer()
        {
            const size_t oldCapacity = m_Buffer.capacity();
            size_t newCapacity = oldCapacity * 2;
            if (newCapacity == 0)
            {
                newCapacity = 1; // Start with a capacity of 1 if the buffer was empty
            }
            else if (newCapacity > m_Buffer.max_size())
            {
                PrintMessageNow(LogLevel::ERROR, __FILE__, __LINE__, 
                    std::format("Cannot resize circular buffer beyond its max size of {} elements", m_Buffer.max_size()));
                return;
            }

            m_Buffer.reserve(oldCapacity * 2); // Double the size of the buffer
        }

    public:
        CircularBuffer(size_t size = 8192) // Default size is 8192
        {
            if (size > m_Buffer.max_size())
            {
                PrintMessageNow(LogLevel::ERROR, __FILE__, __LINE__, 
                    std::format("Cannot create circular buffer larger than its max size of {} elements", m_Buffer.max_size()));
                size = m_Buffer.max_size();
            }
            else if (size < 2)
            {
                size = 2; // Minimum size of 2
            }

            m_Buffer.reserve(size);
            m_ReadIdx = 0;
            m_WriteIdx = 0;
            m_Full = false;
        }

        ~CircularBuffer()
        {
        }

        bool empty() const
        {
            return (!m_Full && (m_ReadIdx == m_WriteIdx));
        }

        size_t size() const
        {
            int size = m_WriteIdx - m_ReadIdx;
            if (size < 0)
            {
                size += m_Buffer.capacity();
            }

            return static_cast<size_t>(size);
        }

        T front()
        {
            return m_Buffer[m_ReadIdx];
        }

        void pop()
        {
            if (empty())
            {
                PrintMessageNow(LogLevel::ERROR, __FILE__, __LINE__, "Attempted to pop from an empty circular buffer");
                return;
            }

            IncrementReadIdx();
            m_Full = false;
        }

        template <typename... Args>
        void emplace(Args&&... args)
        {
            const bool overwriteOldest = false; // Change to true to allow overwriting the oldest data for performance

            if (m_Full)
            {
                // Log immediately using the Logger class's formatting and bypass the queue
                PrintMessageNow(LogLevel::ERROR, __FILE__, __LINE__, 
                    std::format("Attempted to emplace into a full circular buffer. Current Buffer Size: {}", m_Buffer.capacity()));
                
                if (!overwriteOldest)
                {
                    // In the case of not overwriting, extend buffer size
                    ExtendBuffer();
                }
                else
                {
                    // Move read index forward to overwrite the oldest data
                    IncrementReadIdx();
                }
            }

            // Place the new item at the write position
            new (&m_Buffer[m_WriteIdx]) T(std::forward<Args>(args)...); // Placement new
            IncrementWriteIdx(); // Move write index forward
            m_Full = (m_ReadIdx == m_WriteIdx);
        }
};

#endif // !CIRCULAR_BUFFER_H
