#ifndef _Lock_H
#define _Lock_H

#include <windows.h>


//���ӿ���
class ILock
{
public:
	virtual ~ILock() {}
	virtual void Lock() const = 0;
	virtual bool IsLock() const = 0;
	virtual void Unlock() const = 0;
};

//�����������
class Mutex : public ILock
{
public:
	Mutex();
	Mutex(LPCTSTR);
	~Mutex();

	virtual void Lock() const;
	virtual bool IsLock() const;
	virtual void Unlock() const;

private:
	HANDLE m_mutex;
};

//�ٽ�������
class CriSection : public ILock
{
public:
	CriSection();
	~CriSection();

	virtual void Lock() const;
	virtual bool IsLock() const;
	virtual void Unlock() const;

private:
	CRITICAL_SECTION m_critclSection;
};


//��
class CLock
{
public:
	CLock(const ILock&);
	CLock(const ILock *);
	~CLock();

private:
	const ILock& m_lock;
};


#endif