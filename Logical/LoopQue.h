/* Queue.h */
#ifndef __QUEUE_H__
#define __QUEUE_H__
#include <iostream.h>
extern "C" { void exit(int); }

const int nDefaultQueueSize = 50;
template <class T>
class Queue {
private:
	T *qlist; //存放队列元素的指针(数组)
	int size; //队列大小(容量)
	int front; //队首位置
	int rear; //队尾位置(最后一个元素的下一位置)
	int count; //队列中元素的个数
public:
//构造函数
Queue(int initSize=nDefaultQueueSize) 
{
	if (initSize < 1)
	initSize = nDefaultQueueSize;
	qlist = new T[initSize];
	if (!qlist)
	{
		exit(1);
	}
	front = 0;
	rear = 0;
	count = 0;
	size = initSize;
}
//析构函数
~Queue() 
{
  if (qlist) delete [] qlist;
	front = 0;
	rear = 0;
	count = 0;
	size = 0;
}
	//判断队列是否为空
	int QEmpty() {
	return front == rear;
	//return count == 0;
	}
	//判断队列是否已满
	int QFull() {
	return (rear+1) % size == front;
	//return count == size;
	}
	//队列长度
	int QLength() {
	return count;
	//return (rear - front + size) % size;
	}
	//队尾插入(追加)元素
	void QInsert(const T &item) 
	{
	if (count == size) 
	{
		return;
	}
	count ++;
	qlist[rear] = item;
	rear = (rear + 1) % size; //rear始终指向最后一个元素的下一个位置
	}
	//队首删除元素
	T QDelete(T &data) {
	 (count > 0) {
	data = qlist[front];
	count --;
	front = (front + 1) % size; //front移向下一位置
	}
	else
	return data;
	}
	//读取队首元素
	T QFront(T &data) {
		if (count > 0)
		data = qlist[front];
		else 
		return data;
	}
	//清空队列
	void ClearQueue()
	 {
		front = 0;
		rear = 0;
		count = 0;
	}
};
#endif /* !__QUEUE_H__ */