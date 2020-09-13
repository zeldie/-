#include "Subject.h"
#include "Observer.h"

USING(Engine)

CSubject::CSubject()
{
}


CSubject::~CSubject()
{
}

void CSubject::Subscribe(CObserver * pObserver)
{
	if (pObserver == nullptr)
		return;
	m_Observers.push_back(pObserver);
}

void CSubject::UnSubscribe(CObserver * pObserver)
{
	if (pObserver == nullptr)
		return;
	
	auto iter_find = find(m_Observers.begin(), m_Observers.end(), pObserver);

	if (m_Observers.end() == iter_find)
		return;

	m_Observers.erase(iter_find);
}

void CSubject::Notify(int iMessage, void * pData)
{
	for (auto& pObserver : m_Observers)
		pObserver->Update(iMessage, pData);
}

CSubject* CSubject::Create()
{
	return new CSubject;
}

void CSubject::Free(void)
{
	m_Observers.clear();
}
