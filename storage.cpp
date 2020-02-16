#include "stdafx.h"
#include "storage.h"

storage::storage()
{
}

storage::~storage()
{
}

HRESULT storage::init(string storageName, POINT xy)
{
	//â�� �ʱ�ȭ
	_name = storageName;
	_boxImg = IMAGEMANAGER->findImage(storageName);
	_boxRc = RectMakeCenter(xy.x,xy.y , _boxImg->getFrameWidth(), _boxImg->getFrameHeight());

	if(storageName=="â��1")_storageAni = ANIMATIONMANAGER->findAnimation("â��1����");
	if(storageName=="â��2")_storageAni = ANIMATIONMANAGER->findAnimation("â��2����");

	_mainImage = IMAGEMANAGER->findImage("â������");

	_showItem = IMAGEMANAGER->findImage("�ۺ����ֱ�");
	_showItemRc = RectMakeCenter(WINSIZEX - 70, 250, _showItem->getWidth(), _showItem->getHeight());

	//â���κ��丮 ���� �ʱ�ȭ
	for (int i = 0; i < SLOTNUM; i++)
	{
		int k, j;
		k = i % 7;
		j = i / 7;
		_slot[i] = RectMakeCenter(WINSIZEX / 2 + 20 + (k * 60), WINSIZEY / 2 - 60 + (j * 60), 40, 40);
		_slotImage[i] = IMAGEMANAGER->findImage("�����̹���");
	}

	//Ŀ�� �ʱ�ȭ
	_cursorNum = 0;
	_cursorSlot = _slot[_cursorNum];
	_cursor = new cursor;
	_cursor->init();
	_grab = new image;
	_grab = IMAGEMANAGER->findImage("Ŀ���׷�");


	//����ִ� ���·� �ʱ�ȭ
	for (int i = 0; i < 28; i++)
	{
		_vStorage.push_back(ITEMMANAGER->addItem("�������"));
	}

	return S_OK;
}

void storage::release()
{
	SAFE_DELETE(_cursor);
}

void storage::update()
{
	if (KEYMANAGER->isOnceKeyDown('5'))
	{
		for (int k = 0; k < _vStorage.size(); k++)
		{
			if (_vStorage[k].getItemInfo().itemName == "�������" || _vStorage[k].getItemInfo().cnt <= 0)
			{
				_vStorage.erase(_vStorage.begin() + k);
				_vStorage.insert(_vStorage.begin() + k, ITEMMANAGER->addItem("��������"));
				_vStorage[k].setItemCnt_equal(10);
				break;
			}
		}
		for (int k = 0; k < _vStorage.size(); k++)
		{
			if (_vStorage[k].getItemInfo().itemName == "�������" || _vStorage[k].getItemInfo().cnt <= 0)
			{
				_vStorage.erase(_vStorage.begin() + k);
				_vStorage.insert(_vStorage.begin() + k, ITEMMANAGER->addItem("����"));
				_vStorage[k].setItemCnt_equal(10);
				break;
			}
		}
		for (int k = 0; k < _vStorage.size(); k++)
		{
			if (_vStorage[k].getItemInfo().itemName == "�������" || _vStorage[k].getItemInfo().cnt <= 0)
			{
				_vStorage.erase(_vStorage.begin() + k);
				_vStorage.insert(_vStorage.begin() + k, ITEMMANAGER->addItem("����ȭ �� ������"));
				_vStorage[k].setItemCnt_equal(10);
				break;
			}
		}
		for (int k = 0; k < _vStorage.size(); k++)
		{
			if (_vStorage[k].getItemInfo().itemName == "�������" || _vStorage[k].getItemInfo().cnt <= 0)
			{
				_vStorage.erase(_vStorage.begin() + k);
				_vStorage.insert(_vStorage.begin() + k, ITEMMANAGER->addItem("õ"));
				_vStorage[k].setItemCnt_equal(10);
				break;
			}
		}
		for (int k = 0; k < _vStorage.size(); k++)
		{
			if (_vStorage[k].getItemInfo().itemName == "�������" || _vStorage[k].getItemInfo().cnt <= 0)
			{
				_vStorage.erase(_vStorage.begin() + k);
				_vStorage.insert(_vStorage.begin() + k, ITEMMANAGER->addItem("���ھ�"));
				_vStorage[k].setItemCnt_equal(10);
				break;
			}
		}
		for (int k = 0; k < _vStorage.size(); k++)
		{
			if (_vStorage[k].getItemInfo().itemName == "�������" || _vStorage[k].getItemInfo().cnt <= 0)
			{
				_vStorage.erase(_vStorage.begin() + k);
				_vStorage.insert(_vStorage.begin() + k, ITEMMANAGER->addItem("�̻���"));
				_vStorage[k].setItemCnt_equal(10);
				break;
			}
		}
	}
	playerCollision(); //����ݱ�
	//itemArrange();	   //Z��ư�� ������ â���� ����ִ� �͵��� �ڵ����� �������ִ� �Լ�...�̿ϼ�
	if (!PLAYER->getinventory()->getTest()) cursorControl();   //-->Ŀ����Ʈ�� WASD��ư
	else PLAYER->getinventory()->cursormove();				//-->Ŀ����Ʈ�� WASD��ư
	cursorControl();   //Ŀ����Ʈ�� WASD��ư
	setStorageItem();  //â������ ������ ��ġ�� ������Ʈ
	removeItem();	   //ī��Ʈ�� 0���� ������ �����۵��� ��� �����ش�. ������ �������� �ٽ� '�������'�� �־���
	PLAYER->getinventory()->inventoryItem();						//			---> â������ �κ��丮�� ���¸� ��������ϸ� 
	PLAYER->getinventory()->grabitemremove();					//			--->  â������ (â����)�κ��丮 �������� ������ ���¸� �˾ƾ��Ѵ�. 
	//invenplayer();				//    -->â���� �κ��丮�� �ְ� �޴� �Լ�
	//invenplayermove(); //-->�κ��丮 �������� â���� �Ѿ�� �� ������� �ڸ���  ä������ �Լ�
}

void storage::render()
{
	PLAYER->getinventory()->moverender(getMemDC());
	storageRender();
	//Rectangle(getMemDC(), _cursorSlot.left, _cursorSlot.top, _cursorSlot.right, _cursorSlot.bottom);
	if (!_showWindow)
	{
		if (IntersectRect(&temp, &PLAYER->getPlayercollision(), &_boxRc))
		{
			IMAGEMANAGER->render("����", getMemDC(), _boxRc.right, _boxRc.top - 20);
		}
	}
	
	if (_vTemp.size() > 0)	//������ ������� �ʴٸ�(Ŀ���� ���� ��� �ִٸ�)
	{
		char str[128];
		IMAGEMANAGER->render("Ŀ���׷�", getMemDC(), _cursorSlot.left, _cursorSlot.top - 60);
		_vTemp[0].getItemInfo().image->render(getMemDC(), _cursorSlot.left, _cursorSlot.top - 50);
		wsprintf(str, "%d", _vTemp[0].getItemInfo().cnt);
		TextOut(getMemDC(), _cursorSlot.right, _cursorSlot.top - 40, str, strlen(str));
	}
}


void storage::setStorageItem()
{
	if (_showWindow)
	{
		for (int i = 0; i < SLOTNUM; i++)
		{
			//â���� ������ �������� ��Ʈ�� ���Կ� �°� �ʱ�ȭ
			if (i < _vStorage.size())
			{
				_vStorage[i].setRect(_slot[i]);
			}
		}
	}
}

void storage::storageRender()
{
	_boxImg->aniRender(getMemDC(), _boxRc.left, _boxRc.top, _storageAni);

	if (_showWindow)//����������
	{
		char str[128];

		//â������
		_mainImage->render(getMemDC(), WINSIZEX / 2 - 70, 100, _mainImage->getWidth(), _mainImage->getHeight());
		_showItem->render(getMemDC(), _showItemRc.left, _showItemRc.top, _showItem->getWidth(), _showItem->getHeight());

		//������ĭ ����. [] <<�̷��Ի����
		for (int i = 0; i < SLOTNUM; i++)
		{
			_slotImage[i]->render(getMemDC(), _slot[i].left, _slot[i].top);
		}

		//���� â�� �ȿ� �������� ������ ���Կ� �������� ������.
		for (int i = 0; i < _vStorage.size(); i++)
		{
			if (_vStorage[i].getItemInfo().itemName != "�������") //�������� ������ ����
			{
				_vStorage[i].render();

				wsprintf(str, "%d", _vStorage[i].getItemInfo().cnt);
				TextOut(getMemDC(), _vStorage[i].getRECT().right, _vStorage[i].getRECT().bottom, str, strlen(str));
				// Ŀ���� �������� �浹�� ���¶�� �߰��� ��ĭ�� �̹����� �����
				if (IntersectRect(&temp, &_cursorSlot, &_vStorage[i].getRECT()))
				{
					_vStorage[i].getItemInfo().image->render(getMemDC(), _showItemRc.left+10, _showItemRc.top+10);
				}
			}
		}
		if(!PLAYER->getinventory()->getTest()) _cursor->render();
	}
}

void storage::cursorControl()
{
	if (_showWindow)
	{
		if (KEYMANAGER->isOnceKeyDown('A'))
		{
			//Ŀ���� ������ ��������
			_cursorNum--;
			if (_cursorNum < 0)
			{
				_cursorNum = 0;
				PLAYER->getinventory()->setTest(true); //������ �κ��丮�� �ű��
				if(_vTemp.size()!=0) PLAYER->getinventory()->swapItem(_vTemp[0]);
			//	PLAYER->getinventory()->getvTemp()[0];
			}
			/*	if (_cursorNum = 7 && KEYMANAGER->isOnceKeyDown('A'))
				{
					PLAYER->getinventory()->setTest(true);
				}*/

			_cursorSlot = RectMake(_slot[_cursorNum].left, _slot[_cursorNum].top, 40, 40);
			_cursor->getAni()->start();
		}
		if (KEYMANAGER->isOnceKeyDown('D'))
		{
			//Ŀ���� ������ ��������
			_cursorNum++;
			if (_cursorNum > SLOTNUM) _cursorNum = SLOTNUM;

			_cursorSlot = RectMake(_slot[_cursorNum].left, _slot[_cursorNum].top, 40, 40);
			_cursor->getAni()->start();
		}
		if (KEYMANAGER->isOnceKeyDown('W'))
		{
			//Ŀ���� ���θ�������
			int temp;
			temp = _cursorNum;
			_cursorNum-=7;
			if (_cursorNum < 0)_cursorNum = temp;

			_cursorSlot = RectMake(_slot[_cursorNum].left, _slot[_cursorNum].top, 40, 40);
			_cursor->getAni()->start();
		}
		if (KEYMANAGER->isOnceKeyDown('S'))
		{
			//Ŀ���� �Ʒ��� ��������
			int temp;
			temp = _cursorNum;
			_cursorNum+=7;
			if (_cursorNum > SLOTNUM)_cursorNum = temp;

			_cursorSlot = RectMake(_slot[_cursorNum].left, _slot[_cursorNum].top, 40, 40);
			_cursor->getAni()->start();
		}
		_cursor->setRc(_cursorSlot);

		if (KEYMANAGER->isOnceKeyDown('I'))
		{
			_cursor->getAni()->start();
		}
		grab();
	}
}

void storage::itemArrange() //�̿ϼ�
{
	if (KEYMANAGER->isOnceKeyDown('1'))
	{
		for (int k = 0; k < _vStorage.size(); k++)//������ ������ ������ ���Ƽ�
		{
			if (_vStorage[k].maxItem() == false) // ���� �������� �ִ밹���� �ƴ϶�� 
			{
				for (int j = k + 1; j < _vStorage.size(); j++) //�� �����ۺ��� �����ؼ� ������ ���Ƽ� 
				{
					if (_vStorage[k].getItemInfo().itemName == _vStorage[j].getItemInfo().itemName)
					{
						//���� �������� ������ ��������
						_vStorage[k].setItemCnt();
						_vStorage.erase(_vStorage.begin() + j);
						if (_vStorage[k].getItemInfo().cnt >= _vStorage[k].getItemInfo().maxCnt)
						{
							//�׷��� �ִ밹���� �����ϸ� �ִ밹���� �ٲ��־� ��������
							_vStorage[k].maxItem();
						}
						//break;
					}
				}
			}
		}
	}
}

void storage::resetCursor()
{
	//â�� â�� ���������� Ŀ����ġ �ʱ�ȭ
	_cursorNum = 0; 
	_cursorSlot = _slot[_cursorNum];
}

void storage::removeItem()
{
	for (int i = 0; i < _vStorage.size(); i++)
	{
		if(_vStorage[i].getItemInfo().cnt == 0)
		{
			_vStorage.erase(_vStorage.begin() + i);
			_vStorage.insert(_vStorage.begin() + i , ITEMMANAGER->addItem("�������"));
		}
	}
}

void storage::grab()
{
	for (int i = 0; i < _vStorage.size(); i++)
	{
		if (_vStorage[i].getItemInfo().itemName != "�������") //â�� ���� ������� �ʰ�
		{
			if ((_cursorNum == i )&& KEYMANAGER->isOnceKeyDown('J'))// Ŀ���� �������� �浹�� ���¶��
			{
				_cursor->getAni()->start();
	
				//�ӽú��Ͱ� ��������� ���Ϳ� �߰�
				if (_vTemp.size() <= 0) 
				{
					_vTemp.push_back(_vStorage[i]);
					_vTemp[0].setItemCnt_equal(1);
					_vStorage[i].setItemCnt(-1);
					//_vStorage.erase(_vStorage.begin() + i);
					//_vStorage.insert(_vStorage.begin() + i, ITEMMANAGER->addItem("�������"));
					break;
				}

				//�ӽú��Ϳ� �̹� ���� �̸��� �������� ������ ī��Ʈ�� �÷���
				if (_vTemp[0].getItemInfo().itemName == _vStorage[i].getItemInfo().itemName &&
					(_vStorage[i].getItemInfo().cnt+_vTemp[0].getItemInfo().cnt)<= _vTemp[0].getItemInfo().maxCnt) 
				{
					_vTemp[0].setItemCnt();
					_vStorage[i].setItemCnt(-1);
					break;
				}
			}
			//if ((_cursorNum == i) && KEYMANAGER->isStayKeyDown('J'))
			//{
			//	allGrab = true;
			//	if (allGrab)
			//	{
			//		_cursor->getAni()->start();

			//		//�ӽú��Ͱ� ��������� ���Ϳ� �߰�
			//		if (_vTemp.size() <= 0)
			//		{
			//			_vTemp.push_back(_vStorage[i]);
			//			_vStorage.erase(_vStorage.begin() + i);
			//			_vStorage.insert(_vStorage.begin() + i, ITEMMANAGER->addItem("�������"));
			//			break;
			//		}

			//		//�ӽú��Ϳ� �̹� ���� �̸��� �������� ������ ī��Ʈ�� �÷���
			//		if (_vTemp[0].getItemInfo().itemName == _vStorage[i].getItemInfo().itemName &&
			//			(_vStorage[i].getItemInfo().cnt + _vTemp[0].getItemInfo().cnt) <= _vTemp[0].getItemInfo().maxCnt)
			//		{
			//			_vTemp[0].setItemCnt(_vStorage[i].getItemInfo().cnt);
			//			_vStorage.erase(_vStorage.begin() + i);
			//			_vStorage.insert(_vStorage.begin() + i, ITEMMANAGER->addItem("�������"));
			//			break;
			//		}
			//		allGrab = false;
			//	}
			//}
		}
		
		if (_vTemp.size() > 0)	//������ ������� �ʴٸ�(�������� ��� �ִٸ�)
		{
			if (_vStorage[i].getItemInfo().itemName == "�������") //����ִ� ����
			{
				if ((_cursorNum == i) && KEYMANAGER->isOnceKeyDown('J'))//j�� ������ �������� �� �ڸ��� ����
				{
					_cursor->getAni()->start();

					_vStorage.erase(_vStorage.begin() + i);
					_vStorage.insert(_vStorage.begin() + i, _vTemp[0]);
					_vTemp.pop_back();
					break;
				}
			}
		}

	}
}


void storage::invenplayer()   //-->
{
	if (PLAYER->getinventory()->getTest() == true)						//�κ������� �Ѿ�� ���¿���
	{
		for (int i = 0; i < PLAYER->getinventory()->getvInven().size(); i++)				//��ü ���� Ȯ���ϱ�
		{
			if (PLAYER->getinventory()->getvInven()[i].getItemInfo().itemName != "�������")
			{
				if ((PLAYER->getinventory()->getcusornumber() == i) && KEYMANAGER->isOnceKeyDown('U'))
				{
					_cursor->getAni()->start();

					if (_vTemp.empty())
					{
						_vTemp.push_back(PLAYER->getinventory()->getvInven()[i]);
						_vTemp[0].setItemCnt_equal(1);
						PLAYER->getinventory()->getvInven()[i].setItemCnt(-1);
						break;
					}
					if ((_vTemp[0].getItemInfo().itemName == PLAYER->getinventory()->getvInven()[i].getItemInfo().itemName))
					{
						_vTemp[0].setItemCnt();
						PLAYER->getinventory()->getvInven()[i].setItemCnt(-1);
						break;
					}
				}
			}
			if (!_vTemp.empty())
			{
				if (i < 20)
				{
					if (PLAYER->getinventory()->getvInven()[i].getItemInfo().itemName == "�������")
					{
						if ((PLAYER->getinventory()->getcusornumber() == i) && KEYMANAGER->isOnceKeyDown('U'))
						{
							_cursor->getAni()->start();

							PLAYER->getinventory()->getvInven().erase(PLAYER->getinventory()->getvInven().begin() + i);
							PLAYER->getinventory()->getvInven().insert(PLAYER->getinventory()->getvInven().begin() + i, _vTemp[0]);
							_vTemp.pop_back();
							break;
						}
					}
				}
			}
		}
	}
}

void storage::invenplayermove()
{
	for (int i = 0; i < PLAYER->getinventory()->getvInven().size(); i++)
	{
		if (PLAYER->getinventory()->getvInven()[i].getItemInfo().cnt == 0)
		{
			PLAYER->getinventory()->getvInven().erase(PLAYER->getinventory()->getvInven().begin() + i);
			PLAYER->getinventory()->getvInven().insert(PLAYER->getinventory()->getvInven().begin() + i, ITEMMANAGER->addItem("�������"));
		}
	}
}


void storage::playerCollision()//����ݱ�
{
	if (IntersectRect(&temp, &PLAYER->getPlayercollision(), &_boxRc))
	{
		if (!_showWindow)
		{
			if (KEYMANAGER->isOnceKeyDown('J'))
			{
				if (_name == "â��1")_storageAni = ANIMATIONMANAGER->findAnimation("â��1����");
				if (_name == "â��2")_storageAni = ANIMATIONMANAGER->findAnimation("â��2����");
				resetCursor();
				_storageAni->start();
				PLAYER->getinventory()->setStprageOpen(true);  //--->������ â(�κ�) ���� 
				_showWindow = true;
			}
		}
		if (_showWindow)
		{
			if (KEYMANAGER->isOnceKeyDown('I'))
			{
				if (_name == "â��1")_storageAni = ANIMATIONMANAGER->findAnimation("â��1Ŭ����");
				if (_name == "â��2")_storageAni = ANIMATIONMANAGER->findAnimation("â��2Ŭ����");
				
				_storageAni->start();
				PLAYER->getinventory()->setStprageOpen(false);			//--> ������â(�κ�) �ݱ�
				_showWindow = false;
			}
		}
	}
}