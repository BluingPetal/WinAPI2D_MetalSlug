#pragma once
#include "WinAPI.h"

class CCore;
class CEventManager;
class CCollisionManager;
class CUIManager;
class CScene;

class CSceneManager : public SingleTon<CSceneManager>
{
	friend SingleTon<CSceneManager>;
	friend CCore;
	friend CEventManager;
	friend CCollisionManager;
	friend CUIManager;
private:
	CSceneManager();
	virtual ~CSceneManager();

	map<GroupScene, CScene*> m_mapScene;	// ���Ӿ����� �����ϴ� �ڷᱸ��
	CScene* m_pCurScene;					// ���� ���Ӿ�
	int m_credit;

	void Init();
	void Update();
	void Render();
	void Release();

public:
	CScene* GetCurScene();					// ���� ���Ӿ� ��ȯ
	void SetCredit(int credit) { m_credit = credit; }
	int GetCredit() { return m_credit; }
	void ChangeScene(GroupScene scene);		// ���Ӿ� ����
};

#define SCENE				CSceneManager::GetInstance()