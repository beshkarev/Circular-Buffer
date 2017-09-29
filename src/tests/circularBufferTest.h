#include <cxxtest/TestSuite.h>
#include "circularBuffer.h"

class CircularBufferTest : public CxxTest::TestSuite
{
public:
    void testPushMessage()  {
        CircularBuffer buff;
        char arr[] = "Hello";
        buff.pushMessage(arr);

        char arr1[] = "Hello 1";
        buff.pushMessage(arr1);

        char arr2[] = "Hello 2";

        char arr3[] = "Hello 3";

        TS_ASSERT_EQUALS(buff.popMessage(), "Hello");
        buff.pushMessage(arr2);
        TS_ASSERT_EQUALS(buff.popMessage(), "Hello 1");
        buff.pushMessage(arr3);
        TS_ASSERT_EQUALS(buff.popMessage(), "Hello 2");
        TS_ASSERT_EQUALS(buff.popMessage(), "Hello 3");
        TS_ASSERT_EQUALS(buff.popMessage(), "ERROR. The buffer is empty.");
    }

    void testPopMessage() {
        CircularBuffer buff;
        char arr[] = "Hello";

        buff.pushMessage(arr);
        TS_ASSERT_EQUALS(buff.popMessage(), "Hello");
        TS_ASSERT_EQUALS(buff.popMessage(), "ERROR. The buffer is empty.");
    }
};
