//
// Created by xiaotian on 2021/10/31.
//

#ifndef PIO_IN_CLION_ARRAYSTACK_H
#define PIO_IN_CLION_ARRAYSTACK_H
#include <iostream>
using namespace std;
//模板类
template<class T>
class ArrayStack{
public:
    ArrayStack();
    ~ArrayStack();
    void push(T t);//向栈中添加一个t元素
    T peek();//向栈中取出一个元素
    T pop();//在栈中删除一个元素
    int size();//大小
    int isEmpty();//判断是否为空
private:
    T *arr;//数组？
    int count;
};
// 创建“栈”，默认大小是1024
template<class T>
ArrayStack<T>::ArrayStack() //这里面就是比普通的
{
    arr = new T[1024];
    if (!arr)
    {
        cout<<"arr malloc error!"<<endl;
    }
}
// 销毁“栈”
template<class T>
ArrayStack<T>::~ArrayStack()
{
    if (arr)
    {
        delete[] arr;
        arr = NULL;
    }
}
// 将val添加到栈中
//向栈中添加一个元素
template<class T>
void ArrayStack<T>::push(T t)
{
    //arr[count++] = val;
    arr[count++] = t;
}
// 返回“栈顶元素值”
template<class T>
T ArrayStack<T>::peek()
{
    return arr[count-1];
}
// 返回“栈顶元素值”，并删除“栈顶元素”
template<class T>
T ArrayStack<T>::pop()
{
    int ret = arr[count-1];
    count--;
    return ret;
}
// 返回“栈”的大小
template<class T>
int ArrayStack<T>::size()
{
    return count;
}
// 返回“栈”是否为空
template<class T>
int ArrayStack<T>::isEmpty() {
    return size() == 0;
}
#endif //PIO_IN_CLION_ARRAYSTACK_H
