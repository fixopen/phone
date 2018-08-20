/* Queue.h */
#ifndef __QUEUE_H__
#define __QUEUE_H__
#include <iostream.h>
extern "C" { void exit(int); }

const int nDefaultQueueSize = 50;
template <class T>
class Queue {
private:
	T *qlist; //��Ŷ���Ԫ�ص�ָ��(����)
	int size; //���д�С(����)
	int front; //����λ��
	int rear; //��βλ��(���һ��Ԫ�ص���һλ��)
	int count; //������Ԫ�صĸ���
public:
//���캯��
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
//��������
~Queue() 
{
  if (qlist) delete [] qlist;
	front = 0;
	rear = 0;
	count = 0;
	size = 0;
}
	//�ж϶����Ƿ�Ϊ��
	int QEmpty() {
	return front == rear;
	//return count == 0;
	}
	//�ж϶����Ƿ�����
	int QFull() {
	return (rear+1) % size == front;
	//return count == size;
	}
	//���г���
	int QLength() {
	return count;
	//return (rear - front + size) % size;
	}
	//��β����(׷��)Ԫ��
	void QInsert(const T &item) 
	{
	if (count == size) 
	{
		return;
	}
	count ++;
	qlist[rear] = item;
	rear = (rear + 1) % size; //rearʼ��ָ�����һ��Ԫ�ص���һ��λ��
	}
	//����ɾ��Ԫ��
	T QDelete(T &data) {
	 (count > 0) {
	data = qlist[front];
	count --;
	front = (front + 1) % size; //front������һλ��
	}
	else
	return data;
	}
	//��ȡ����Ԫ��
	T QFront(T &data) {
		if (count > 0)
		data = qlist[front];
		else 
		return data;
	}
	//��ն���
	void ClearQueue()
	 {
		front = 0;
		rear = 0;
		count = 0;
	}
};
#endif /* !__QUEUE_H__ */