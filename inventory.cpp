#include "stdafx.h"
#include "inventory.h"


/*
ĳ����������
���� ��ȯ
���� �־��� ����, ���� ��ȯ
*/

/*
â��������
�κ��� �ִ� ���� �ְ� �ޱ�
*/

inventory::inventory()
{
}

inventory::~inventory()
{
}

HRESULT inventory::init()
{
	ITEMMANAGER->init();

	//=================================================������ �ִ� �̹�����=====================================================
	//bkrender()�� ���� �̹�����
	_bgimag = IMAGEMANAGER->findImage("���");								//�ڿ� ���

	//�κ��丮 ��
	_playerinventory._inventoryimg = IMAGEMANAGER->findImage("�κ��丮��");
	_playerinventory.x = WINSIZEX / 2;
	_playerinventory.y = WINSIZEY / 2;
	_playerinventory._inventoryrect = RectMakeCenter(_playerinventory.x, _playerinventory.y, _playerinventory._inventoryimg->getWidth(), _playerinventory._inventoryimg->getHeight());

	//�������� ������ ��ȯ�ϴ� �̹���
	_removeGlass._inventoryimg = IMAGEMANAGER->findImage("������");
	_removeGlass.x = 170;
	_removeGlass.y = 550;
	_removeGlass._inventoryrect = RectMakeCenter(_removeGlass.x, _removeGlass.y, _removeGlass._inventoryimg->getWidth(), _removeGlass._inventoryimg->getHeight());

	//�� �����ִ� ��
	_showitem._showitemimg = IMAGEMANAGER->findImage("�ۺ����ֱ�");					//������ Ŭ���ϸ� �ߴ� �� ������ ���� ����
	_showitem._showitemrc = RectMakeCenter(WINSIZEX - 70, 250, _showitem._showitemimg->getWidth(), _showitem._showitemimg->getHeight());

	_zbutton._inventoryimg = IMAGEMANAGER->findImage("Z��ư");
	_zbutton.x = 660;
	_zbutton.y = 225;

	_weaponiright._inventoryimg = IMAGEMANAGER->findImage("z��Ȱ��ȭ��");
	_weaponiright.x = 637;
	_weaponiright.y = 235;
	_weaponileft._inventoryimg = IMAGEMANAGER->findImage("z����Ȱ��ȭ��");
	_weaponileft.x = 700;
	_weaponileft.y = 235;

	//z����Ȱ��ȭ��,
	_weaponirighting._inventoryimg = IMAGEMANAGER->findImage("z����Ȱ��ȭ��");
	_weaponirighting.x = 700;
	_weaponirighting.y = 235;

	//Ȱ��ȭ��
	_weaponilefting._inventoryimg = IMAGEMANAGER->findImage("zȰ��ȭ��");
	_weaponilefting.x = 637;
	_weaponilefting.y = 235;

	//bkrender()�� ���� �̹�����
	//============================================================================================================
	_inventorybg._inventoryimg = IMAGEMANAGER->findImage("�κ��丮����");
	_inventorybg.x = WINSIZEX / 2 - 410;
	_inventorybg.y = WINSIZEY / 2 - 215;

	//�κ��丮 ��ҵ�
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			_invenotryelement[i + 5 * j]._inventoryimg = IMAGEMANAGER->findImage("���");
			_invenotryelement[i + 5 * j]._inventoryrect = RectMakeCenter(190 + i * 55, 240 + j * 60, 40, 40);	 // ����� ������ 1~2��° �� �κ��丮 ��Ҵ�
			if (j >= 2)   // ����� �� 3~4��° �� �κ��丮 ��Ҵ�
			{
				// �κ��丮 ��� �ҷ����� �̹���  
				_invenotryelement[i + 5 * j]._inventoryrect = RectMakeCenter(190 + i * 55, 240 + j * 60, 40, 40);				//�κ��丮 ��ġ ����
			}
		}
	}
	_invenotryelement[20]._inventoryrect = RectMakeCenter((_removeGlass._inventoryrect.left + _removeGlass._inventoryrect.right) / 2 + 15, (_removeGlass._inventoryrect.top + _removeGlass._inventoryrect.bottom) / 2 - 22, 40, 40);			// �������� ������ ��ȯ�ϴ� �κ��丮 ��� ��������ġ



	//Ŀ�� Ŭ���� �̿��ϱ�
	_cursor = new cursor;
	_cursor->init();
	_cursorNumber = 0;
	_cursorrect = _invenotryelement[_cursorNumber]._inventoryrect;
	_cursor->setRc(_cursorrect);

	//_cursorrect = RectMakeCenter(190, 240, 40, 40);
	//Ŀ�� �׷�
	_grab._grabimg = IMAGEMANAGER->addImage("Ŀ���׷�", "images/shop/grab.bmp", 50, 52, true, RGB(255, 0, 255));

	_openinventorywin = false;

	//�ӽÿ����� ���� 
	_vInven.push_back(ITEMMANAGER->addItem("��������"));// �ӽ÷� �߰��� ���� �� 
	_vInven.push_back(ITEMMANAGER->addItem("��ȭ����"));// �ӽ÷� �߰��� ���� �� 
	_vInven.push_back(ITEMMANAGER->addItem("�踷���"));// �ӽ÷� �߰��� ���� �� 
	_vInven.push_back(ITEMMANAGER->addItem("����"));// �ӽ÷� �߰��� ���� �� 
	_vInven.push_back(ITEMMANAGER->addItem("õ"));// �ӽ÷� �߰��� ���� �� 
	_vInven[4].setItemCnt(5);
	_vInven.push_back(ITEMMANAGER->addItem("õ"));// �ӽ÷� �߰��� ���� �� 
	_vInven[5].setItemCnt(4);

	test = false;
	_storageOpen = false;
	_isweapon = false;			//���� ��ȯ�ϱ� ���� �Ұ�.. �̹����θ�

	return S_OK;
}
void inventory::release()
{
	SAFE_DELETE(_cursor);
}

void inventory::update()
{
	ANIMATIONMANAGER->update();				//Ŀ�� �ִϸ��̼� �����̱� ���� �Ŵ��� �Լ� 
	if (!_openinventorywin)   // �κ��丮 ��Ȱ��ȭ
	{
		if (KEYMANAGER->isOnceKeyDown('6'))
		{
			_openinventorywin = true;
			_cursor->update(_cursorrect);
			_cursorNumber = 0;
			_cursorrect = _invenotryelement[_cursorNumber]._inventoryrect;
			_cursor->setRc(_cursorrect);
		} // �κ��丮Ȱ��ȭ
	}
	else // �κ��丮Ȱ��ȭ
	{
		if (KEYMANAGER->isOnceKeyDown('6')) // �κ��丮Ȱ��ȭ
			_openinventorywin = false;
		cursormove();//Ŀ�� �����̱� 
		inventoryItem();
		grabitemremove();  //J�� ������ �� �������� ����
		isweaponing();
	}
}

void inventory::cursormove()   //Ŀ�� �̵�  �Լ�
{
	_cursor->update(_cursorrect);						//���� �ʱ�ȭ
	if (KEYMANAGER->isOnceKeyDown('A'))		//����
	{
		_cursorNumber--;
		if (_cursorNumber < 0)_cursorNumber = 0;
		_cursorrect = RectMake(_invenotryelement[_cursorNumber]._inventoryrect.left, _invenotryelement[_cursorNumber]._inventoryrect.top, 40, 40);
		_cursor->setRc(_cursorrect);
		_cursor->getAni()->start();
	}
	if (KEYMANAGER->isOnceKeyDown('D'))			//������
	{
		_cursorNumber++;

		if (!_storageOpen)
		{
			if (_cursorNumber > 20);//_cursorNumber = 20;
		}
		else
		{
			if (_cursorNumber > 19)
			{
				test = false;
				_cursorNumber = 0;

			}
		}
		_cursorrect = RectMake(_invenotryelement[_cursorNumber]._inventoryrect.left, _invenotryelement[_cursorNumber]._inventoryrect.top, 40, 40);
		_cursor->setRc(_cursorrect);
		_cursor->getAni()->start();
	}
	if (KEYMANAGER->isOnceKeyDown('W'))			//��
	{
		int temp;
		temp = _cursorNumber;
		_cursorNumber -= 5;
		if (_cursorNumber < 0)  _cursorNumber = temp;
		_cursorrect = RectMake(_invenotryelement[_cursorNumber]._inventoryrect.left, _invenotryelement[_cursorNumber]._inventoryrect.top, 40, 40);
		_cursor->setRc(_cursorrect);
		_cursor->getAni()->start();

	}
	if (KEYMANAGER->isOnceKeyDown('S'))
	{
		int temp;
		temp = _cursorNumber;
		_cursorNumber += 5;
		if (_cursorNumber > 20) _cursorNumber = temp;
		_cursorrect = RectMake(_invenotryelement[_cursorNumber]._inventoryrect.left, _invenotryelement[_cursorNumber]._inventoryrect.top, 40, 40);
		_cursor->setRc(_cursorrect);
		_cursor->getAni()->start();
	}
	grabmove();
}

void inventory::inventoryItem()				//�κ��丮 ���ĭ(_invenotryelement[i])�� ���� (_vInven[i])ĭ�� ��ġ�� ���Ͻ�Ű�� ���� �Լ�
{
	if (_openinventorywin || _storageOpen)
	{
		for (int i = 0; i < 21; i++)
		{
			if (i < _vInven.size())		//���� �κ��丮 ĭ ��ü ũ�� ���� 
				_vInven[i].setRect(_invenotryelement[i]._inventoryrect); // �κ��丮 ĭ ��� ��ġ = ���� �κ��丮 ĭ ��ġ�� ���� ��Ŵ
			for (int j = _vInven.size(); j < 21; j++)
			{
				_vInven.push_back(ITEMMANAGER->addItem("�������"));
			}
		}
	}
}

void inventory::grabmove()
{
	for (int i = 0; i < _vInven.size(); i++)			//���� ����� for������ ����  
	{
		if (_vInven[i].getItemInfo().itemName != "�������")  // �������� ������� ���� ĭ�� Ȯ���Ѵ�.
		{
			if ((_cursorNumber == i) && KEYMANAGER->isOnceKeyDown('J'))   //Ŀ�� ��ȣ(_cursorNumber)�� ���� ��� i�� ��ġ��  J������
			{
				_cursor->getAni()->start();
				if (_vTemp.empty())
				{
					_vTemp.push_back(_vInven[i]); //�ӽ÷� ���� ���Ϳ� �κ��丮�� �ִ� ���� ��Ҹ� ��´�. 
					_vTemp[0].setItemCnt_equal(1);
					_vInven[i].setItemCnt(-1);
					break;
				}
				if (_vTemp[0].getItemInfo().itemName == _vInven[i].getItemInfo().itemName &&
					(_vInven[i].getItemInfo().cnt + _vTemp[0].getItemInfo().cnt) <= _vTemp[0].getItemInfo().maxCnt)
				{
					_vTemp[0].setItemCnt();
					_vInven[i].setItemCnt(-1);
					break;
				}
			}
		}
		if (!_vTemp.empty())     //����
		{
			if (i < 20)
			{
				if (_vInven[i].getItemInfo().itemName == "�������")
				{
					if ((_cursorNumber == i) && KEYMANAGER->isOnceKeyDown('J'))
					{
						_cursor->getAni()->start();

						_vInven.erase(_vInven.begin() + i);
						_vInven.insert(_vInven.begin() + i, _vTemp[0]);
						_vTemp.pop_back();
						break;
					}
				}
			}
			else   // 20 ĭ�̸� ���� ó��
			{
				//��������
				if (_vInven[i].getItemInfo().itemName == "�������")
				{
					if ((_cursorNumber == i) && KEYMANAGER->isOnceKeyDown('J'))
					{
						_cursor->getAni()->start();

						PLAYER->sellplayermoney((_vTemp[0].getItemInfo().cnt*_vTemp[0].getItemInfo().orignalPrice) / 5);
						_vInven.erase(_vInven.begin() + i);
						_vInven.insert(_vInven.begin() + i, ITEMMANAGER->addItem("�������"));
						_vTemp.pop_back();
						break;
					}
				}
			}
		}
	}
}

void inventory::grabitemremove()
{
	for (int i = 0; i < _vInven.size(); i++)
	{
		if (_vInven[i].getItemInfo().cnt == 0)
		{
			_vInven.erase(_vInven.begin() + i);
			_vInven.insert(_vInven.begin() + i, ITEMMANAGER->addItem("�������"));
		}
	}
}

void inventory::isweaponing()
{
	if (!_isweapon)
	{
		if (KEYMANAGER->isOnceKeyDown('O'))					//�ӽ÷� 
		{
			_isweapon = true;
		}
	}
	else
	{
		if (KEYMANAGER->isOnceKeyDown('O'))					//�ӽ÷� 
		{
			_isweapon = false;
		}
	}
}

void inventory::swapItem(item swapItem)
{
	_vTemp.push_back(swapItem);
	_vTemp[0].setRect(_invenotryelement[4]._inventoryrect);
	_cursor->setRc(_invenotryelement[4]._inventoryrect);
}


void inventory::render(HDC hdc)						// ���� ���� -> render>moverender()> bgrender()
{
	if (_openinventorywin)
	{
		bkrender(hdc);
		itemrender(hdc);//������ ����� 
		_cursor->render();				//Ŀ�� Ŭ���� ���� 
	}
	//storageopen �̶� ����� �κ��丮
	if (_storageOpen)
	{
		moverender(hdc);
	}
}				//render()�� ��

void inventory::moverender(HDC hdc)
{
	if (_storageOpen || _openinventorywin)
	{
		char str[128];
		char moneystr[128];					//�÷��̾� �� 
		_inventorybg._inventoryimg->render(hdc, _inventorybg.x, _inventorybg.y);
		IMAGEMANAGER->render("������", hdc, _removeGlass._inventoryrect.left, _removeGlass._inventoryrect.top);

		wsprintf(moneystr, "%d", PLAYER->getMoney());					//���� ������ �ִ� �� 
		SetTextColor(hdc, RGB(41, 41, 41));					// �� ����
		TextOut(hdc, 285, 540, moneystr, strlen(moneystr));			// ��ġ ���� 


		for (int i = 0; i < 21; i++)				//��� ũ�� ����
		{
			IMAGEMANAGER->render("���", hdc, _invenotryelement[i]._inventoryrect.left, _invenotryelement[i]._inventoryrect.top);
		}
		itemrender(hdc);
		for (int i = 0; i < _vInven.size(); i++) // �κ��丮 �ȿ� �������� �����ִ� for�� 
		{
			if (_vInven[i].getItemInfo().itemName != "�������")     // �������� ������
			{
				//if (i==5 && i ==6 && i ==12&& i==19)
				//{
				wsprintf(str, "%d", _vInven[i].getItemInfo().cnt);
				SetTextColor(hdc, RGB(41, 41, 41));
				TextOut(hdc, _vInven[i].getRECT().right, _vInven[i].getRECT().bottom, str, strlen(str));
				SetBkMode(hdc, TRANSPARENT);				//���� �޹�� ó��
			//	}
							//�浹ó��
				if (IntersectRect(&_temp, &_cursorrect, &_vInven[i].getRECT()))
				{
					_vInven[i].getItemInfo().image->render(hdc, _showitem._showitemrc.left + 10, _showitem._showitemrc.top + 10);
				}
			}
		}
		if (_openinventorywin)_cursor->render();					//�÷��̾ ������ �ִ� �κ��丮�ȿ� �ִ� Ŀ���� �� �� �ְԱ�
		if (test) _cursor->render();												//â���� �κ��丮�� ����� �κ�
	}
}

void inventory::bkrender(HDC hdc)									//�̹����� ���̱� ���� �͵� ���� �ʿ���� �͵� 
{
	if (_openinventorywin)
	{
		char str[128];
		_bgimag->alphaRender(hdc, 1000);   // ���ķ��� ó���� ���ȭ��
		_playerinventory._inventoryimg->render(hdc, _playerinventory._inventoryrect.left, _playerinventory._inventoryrect.top);   //�÷��̾� �κ��丮

		IMAGEMANAGER->render("Z��ư", getMemDC(), _zbutton.x, _zbutton.y);
		IMAGEMANAGER->render("z������Ȱ��ȭ��", hdc, _weaponiright.x, _weaponiright.y);
		IMAGEMANAGER->render("z��Ȱ��ȭ��", hdc, _weaponileft.x, _weaponileft.y);
		if (!_isweapon)
		{
			IMAGEMANAGER->render("z����Ȱ��ȭ��", getMemDC(), _weaponilefting.x, _weaponilefting.y);
		}
		if (_isweapon)
		{
			IMAGEMANAGER->render("zȰ��ȭ��", getMemDC(), _weaponirighting.x, _weaponirighting.y);
		}

		_showitem._showitemimg->render(hdc, _showitem._showitemrc.left, _showitem._showitemrc.top, _showitem._showitemimg->getWidth(), _showitem._showitemimg->getHeight());					 //������ �������� �����ִ� �̹���.  
	}

}

void inventory::itemrender(HDC hdc)  //item���͸� ���� ������
{
	for (_viInven = _vInven.begin(); _viInven != _vInven.end(); ++_viInven)
	{
		(*_viInven).render();
	}

	if (_vTemp.size() > 0)					//�ӽ� Ŀ�� �׷� �ߴ� ��
	{
		char str[128];
		_grab._grabimg->render(hdc, _cursorrect.left - 4, _cursorrect.top - 50);
		_vTemp[0].getItemInfo().image->render(hdc, _cursorrect.left - 4, _cursorrect.top - 40);
		wsprintf(str, "%d", _vTemp[0].getItemInfo().cnt);
		SetTextColor(hdc, RGB(41, 41, 41));
		TextOut(hdc, _cursorrect.right, _cursorrect.top - 40, str, strlen(str));
	}
}