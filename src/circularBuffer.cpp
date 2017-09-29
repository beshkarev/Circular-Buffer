#include "circularBuffer.h"
#include <stdexcept>

Message::Message(char *message) : m_messageSize(strlen(message)) {
    m_message.reserve(m_messageSize + 1);
    m_message.resize(m_messageSize + 1);
    memcpy(m_message.data(), message, m_messageSize + 1);
}

const char *Message::getMessage() const {
    return m_message.data();
}

CircularBuffer::CircularBuffer() :
    m_queue_indx(0),
    m_begin(0),
    m_end(0)
{
    _initQueue();
}

void CircularBuffer::pushMessage(char *message) {
    Message mess(message);

    std::lock_guard<std::mutex> lock(m_buffer_mtx);
    m_buffer[m_end] = mess;
    _pushIndexOfMessageToQueue(m_end);

    if (m_end + 1 == CIRCULAR_BUFFER_SIZE) m_end = 0;
    else ++m_end;
}

const char *CircularBuffer::popMessage() {
    try {
        std::lock_guard<std::mutex> lck(m_buffer_mtx);
        const auto index { _popMessageFromQueue() };
        return m_buffer[index].getMessage();
    } catch (const std::runtime_error &err) {
        return err.what();
    }
}

void CircularBuffer::_pushIndexOfMessageToQueue(uint32_t index) {
    _moveQueueIndxToEmptyPos();
    if (m_queue_indx + 1 == CIRCULAR_BUFFER_SIZE) {
        /* Determinating if the queue should be compressed
         * Example: [-1, 0] -> need move [0] to the first position
         *          [1, 0] -> not need moving anything
         *          before compressing [-1, 0] -> after [0, -1]
         * If the queue will be [0, -1] -> this method decides that nothing needs to be moved
        */
        auto indexOfEmpty { _indexOfEmptyBeforeCurr(m_queue_indx) };
        if (indexOfEmpty != -1) {
            _compressQueue(indexOfEmpty, m_queue_indx);
            /* When we compress the queue we writing current index of element in buffer at the end of the queue */
            m_queue[m_queue_indx] = index;
        } else if (m_queue[m_queue_indx] == -1) {
            /* If we don't have any empty position before current, we save the index to the current position.
             * And go to the beginning of the queue.
             */
            m_queue[m_queue_indx] = index;
            m_queue_indx = 0;
        }
    } else {
        /* Just fills the queue and increments position */
        m_queue[m_queue_indx] = index;
        ++m_queue_indx;
    }
}

int32_t CircularBuffer::_popMessageFromQueue() {
    int32_t index{-1};
    for (uint32_t i{0}; i < CIRCULAR_BUFFER_SIZE; ++i) {
        if (m_queue[i] != -1) {
            index = m_queue[i];
            m_queue[i] = -1;
            break;
        }
    }

    if (index == -1) throw std::runtime_error("ERROR. The buffer is empty.");

    /* Compress the queue after poping element from buffer. [-1, 0] -> [0, -1]
     * Required to preserve the order in which items arrive in the queue.
    */
    _compressQueue(index, CIRCULAR_BUFFER_SIZE - 1);
    return index;
}

void CircularBuffer::_initQueue() {
    for (uint32_t i{0}; i < CIRCULAR_BUFFER_SIZE; ++i) m_queue[i] = -1;
}

int32_t CircularBuffer::_indexOfEmptyBeforeCurr(int32_t currIndx) const {
    for (uint32_t i {0}; i < currIndx; ++i) {
        if (m_queue[i] == -1) return i;
    }

    return -1;
}

void CircularBuffer::_compressQueue(uint32_t begin, uint32_t end) {
    if (begin == 0) ++begin;
    for (uint32_t i{begin}; i <= end; ++i) {
        std::swap(m_queue[i], m_queue[i - 1]);
    }
}

void CircularBuffer::_moveQueueIndxToEmptyPos() {
    if (m_queue[m_queue_indx] != -1 && m_queue_indx != CIRCULAR_BUFFER_SIZE - 1) {
        ++m_queue_indx;
        _moveQueueIndxToEmptyPos();
    } else return;
}
