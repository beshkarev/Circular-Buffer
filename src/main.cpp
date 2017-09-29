#include <iostream>
#include "circularBuffer.h"

int main() {
    CircularBuffer buff;
    char arr[] = "Hello\0";
//    buff.pushMessage(arr);
    buff.pushMessage(arr);

    char arr1[] = "Hello 1\0";
    buff.pushMessage(arr1);

    char arr2[] = "Hello2";
//    buff.pushMessage(arr2);

    char arr3[] = "Hello 3";

    std::cout << buff.popMessage() << '\n';
    buff.pushMessage(arr2);
    std::cout << buff.popMessage() << '\n';
    buff.pushMessage(arr3);
    std::cout << buff.popMessage() << '\n';
    std::cout << buff.popMessage() << '\n';
    std::cout << buff.popMessage() << '\n';

    return 0;
}
