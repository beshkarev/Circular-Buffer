#pragma once
#include <vector>
#include <cstdint>
#include <cstring>
#include <mutex>

/** A size of circular buffer */
constexpr const uint32_t CIRCULAR_BUFFER_SIZE = 2;

/*! The class for message, which will be saved in a circular buffer  */
struct Message {
    /**
     * A constructor.
     * Initialization a vector with a message and a message size, which was calculated from message length.
     * @param message the pointer to a message
    */
    Message(char *message);

    /** A default constructor */
    Message() = default;

    /**
      * The method return stored message.
      * @return a message, which was stored in vector during creating an instance of this class
    */
    const char* getMessage() const;

private:
    uint32_t m_messageSize; /** A size of stored message */
    std::vector<char> m_message; /** A stored message */
};

/*! The class of circular buffer. */
class CircularBuffer {
public:
    /**
     * A constructor. Without parameters.
     * Performs initialization of members.
    */
    CircularBuffer();

    /**
      * The method for pushing message to a circular buffer.
      * New messages will remove old messages, if buffer is exhausted.
      * @param message the pointer to the variable - length array of bytes
    */
    void pushMessage(char *message);

    /**
      * The method for poping message from a circular buffer.
      * If the buffer is empty, will returned error message.
      * @return A message. A char pointer to the constant.
    */
    const char* popMessage();

private:
    /**
      * The private method, which save the order of messages in a circular buffer.
      * @param index The index of message in a circular buffer.
    */
    void _pushIndexOfMessageToQueue(uint32_t index);

    /**
      * The private method, which return a index of oldest message in buffer.
      * Can generated exception.
      * @throw std::runtime_error If a circular buffer is empty, will be generated exception.
    */
    int32_t _popMessageFromQueue();

    /** The private method for initialization a queue. */
    void _initQueue();

    int32_t _indexOfEmptyBeforeCurr(int32_t currIndx) const;

    void _compressQueue(uint32_t begin, uint32_t end);

    void _moveQueueIndxToEmptyPos();

    Message m_buffer[CIRCULAR_BUFFER_SIZE]; /** The circular buffer */

    int32_t m_queue[CIRCULAR_BUFFER_SIZE]; /** The queue, which store the order of message in a circular buffer */

    uint32_t m_queue_indx; /** The index for iteration on queue */

    uint32_t m_begin; /** The circular buffer begin index */

    uint32_t m_end; /** The circular buffer end index */

    std::mutex m_buffer_mtx; /** The mutex for thread safety communication with buffer and queue */
};
