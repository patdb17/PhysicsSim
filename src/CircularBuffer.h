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
        size_t m_Head; // Points to the oldest element
        size_t m_Tail; // Points to the next insertion point, hence the buffer grows at the tail when elements are added
        bool m_Full;

        // Move head forward
        inline void IncrementHead()
        {
            if (m_Buffer.capacity() > 0)
            {
                m_Head = (m_Head + 1) % m_Buffer.capacity(); 
            }
            else
            {
                m_Head++;
            }
        }

        // Move tail forward
        inline void IncrementTail()
        {
            if (m_Buffer.capacity() > 0)
            {
                m_Tail = (m_Tail + 1) % m_Buffer.capacity();
            }
            else
            {
                m_Tail++;
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
            m_Head = 0;
            m_Tail = 0;
            m_Full = false;
        }

        ~CircularBuffer()
        {
        }

        bool empty() const
        {
            return (!m_Full && (m_Head == m_Tail));
        }

        size_t size() const
        {
            int size = m_Tail - m_Head;
            if (size < 0)
            {
                size += m_Buffer.capacity();
            }

            return static_cast<size_t>(size);
        }

        T front()
        {
            return m_Buffer[m_Head];
        }

        void pop()
        {
            if (empty())
            {
                PrintMessageNow(LogLevel::ERROR, __FILE__, __LINE__, "Attempted to pop from an empty circular buffer");
                return;
            }

            IncrementHead();
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
                    // Move head forward to overwrite the oldest data
                    IncrementHead();
                }
            }

            // Place the new item at the tail position
            new (&m_Buffer[m_Tail]) T(std::forward<Args>(args)...); // Placement new
            IncrementTail(); // Move tail forward
            m_Full = (m_Head == m_Tail);
        }
};

#endif // !CIRCULAR_BUFFER_H
