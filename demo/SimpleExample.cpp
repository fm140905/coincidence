/*
 * @Description: Simple ROOT example
 * @Author: Ming Fang
 * @Date: 2021-03-26 12:09:32
 * @LastEditors: Ming Fang
 * @LastEditTime: 2021-03-26 12:27:52
 */
#include <TLorentzVector.h>

int main() {
    TLorentzVector v(1,2,3,4);
    v.Print();
    return 0;
}