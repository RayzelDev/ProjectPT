#include "cReviewSkinWindow.h"

cReviewSkinWindow::~cReviewSkinWindow()
{
	if (pWindow1)
		delete pWindow1;
}

void cReviewSkinWindow::Init()
{
	ClearLoadedImages();
	if (pWindow1)
	{
		delete pWindow1;
		pWindow1 = 0;
	}

	BuildImages();


}

bool cReviewSkinWindow::OnMouseScroll(int zDelta)
{
	if (!pWindow1 || !pWindow1->isVisible())
		return false;
	return pWindow1->OnMouseScroll(zDelta, 0, 0, WinSizeX, WinSizeY, 0, 0);
}

bool cReviewSkinWindow::OnMouseClick(bool flag)
{
	if (!pWindow1 || !pWindow1->isVisible())
		return false;

	return pWindow1->OnMouseClick(flag ? Down : Up, 0, 0, WinSizeX, WinSizeY, 0, 0);
}

void cReviewSkinWindow::OnMouseMove()
{
	if (!pWindow1 || !pWindow1->isVisible())
		return;

	pWindow1->OnMouseMove(0, 0, WinSizeX, WinSizeY, 0, 0);
}

void cReviewSkinWindow::Resize()
{
	if (!pWindow1)
		return;

	pWindow1->setX((WinSizeX - pWindow1->getWidth()) / 2 + 300);
	pWindow1->setY((WinSizeY - pWindow1->getHeight()) / 2);
}

void cReviewSkinWindow::Draw()
{
	if (!pWindow1 || !pWindow1->isVisible())
		return;

	if (pOpen)
	{
		auto pMenuWindow = new UINewDropdown(Review_Menu, 254, 50, "game\\menu\\main.png", "game\\menu\\topleft.png", "game\\menu\\middle.png", "game\\menu\\bottom.png");
		pMenuWindow->AddOption("Armaduras", 1001);
		pMenuWindow->AddOption("Roupão", 1002);
		pMenuWindow->AddOption("Escudos", 1003);
		pMenuWindow->AddOption("Axe", 1004);
		pMenuWindow->AddOption("Garra", 1005);
		pMenuWindow->AddOption("Hammer", 1006);
		pMenuWindow->AddOption("Cajado", 1007);
		pMenuWindow->AddOption("Varinha", 1008);
		pMenuWindow->AddOption("Foice", 1009);
		pMenuWindow->AddOption("Arco", 1010);
		pMenuWindow->AddOption("Espada", 1011);
		pMenuWindow->AddOption("Lança", 1012);
		pMenuWindow->SelectItem(1001);
		pWindow1->AddElement(pMenuWindow);

		pOpen = false;
	}

	auto pMenuWindow = pWindow1->GetElement<UINewDropdown>(Review_Menu);

	if (pMenuWindow && SelectItem != pMenuWindow->returnSelected())
	{
		ClearItems();

		PacketReviewSkin sPacket;
		sPacket.size = sizeof(sPacket);
		sPacket.code = OPCODE_GET_REVIEWSKIN;
		sPacket.Sections = pMenuWindow->returnSelected();
		smWsockServer->Send((char*)&sPacket, sPacket.size, TRUE);
	}

	if (pMenuWindow)
		SelectItem = pMenuWindow->returnSelected();

	pWindow1->Render(0, 0, WinSizeX, WinSizeY, 0, 0);
}

void cReviewSkinWindow::Close()
{
	pWindow1->hide();
	pOpen = false;

	sinSetCharItem(cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE, INVENTORY_POS_LHAND, FALSE);
	sinSetCharItem(cInvenTory.InvenItem[sInven[1].ItemIndex - 1].CODE, INVENTORY_POS_RHAND, FALSE);

	int sArrowPosi = 0;
	if (ArrowState[1] == 0)sArrowPosi = 4;
	else sArrowPosi = 3;
	sinButtonFlag = sArrowPosi;

	sinChangeSetFlag = sArrowPosi;

	sinSetCharItem(cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE, INVENTORY_POS_LHAND, FALSE);
	sinSetCharItem(cInvenTory.InvenItem[sInven[1].ItemIndex - 1].CODE, INVENTORY_POS_RHAND, FALSE);

	if (sInven[2].ItemIndex)
		sinSetCharItem(cInvenTory.InvenItem[sInven[2].ItemIndex - 1].CODE, 8, TRUE);
	else
		sinSetCharItem(NULL, 8, TRUE);

	cInvenTory.SetItemToChar();

}

map<int, int> rCounts;
void cReviewSkinWindow::HandlePacket(PacketReviewSkin* psPacket)
{
	auto pPreviewWindow = pWindow1->GetElement<UINewWindow>(Review_ReviewItens);

	if (psPacket->bFirst)
	{
		rCounts.clear();
		ClearItems();
	}

	if (pPreviewWindow)
	{
		for (int i = 0; i < psPacket->Count; i++)
		{
			if (psPacket->InfoReview[i].Section == SelectItem)
			{
				int count = rCounts[psPacket->InfoReview[i].Section];

				ReviewSkin* pItem = new ReviewSkin;
				pItem->Section = psPacket->InfoReview[i].Section;
				pItem->ID = 0x1000 + count;

				for (int cnt = 0; cnt < MAX_ITEM; cnt++)
				{
					if (sItem[cnt].CODE == psPacket->InfoReview[i].ResultItem.CODE)
					{
						memcpy(&pItem->Item, &sItem[cnt], sizeof(sITEM));
						memcpy(&pItem->Item.sItemInfo, &psPacket->InfoReview[i].ResultItem, sizeof(sITEMINFO));
						break;
					}
				}

				int w = (pItem->Item.w * 21) / 100;
				int h = (pItem->Item.h * 21) / 100;

				auto pTitle = new UINewText(pItem->ID, pItem->Item.sItemInfo.ItemName, strlen(pItem->Item.sItemInfo.ItemName) < 20 ? 30 + (168 * (count % 2)) : 14 + (168 * (count % 2)), 18 + (145 * (count / 2)), true, RGB(255, 255, 255));
				auto pBgItem = new UINewImage(pItem->ID, "Game\\Images\\ReviewSkins\\boxitem.png", 5 + (168 * (count % 2)), 15 + (145 * (count / 2)));

				auto pImage = new UINewImage(pItem->ID, (char*)FormatString("image\\Sinimage\\Items\\%s\\it%s.bmp", pItem->Item.ItemFilePath, pItem->Item.DorpItem), 86 + (168 * (count % 2)), 20 + (145 * (count / 2)), pItem->Item.w - w, pItem->Item.h - h/*, std::bind(&cChangeItem::OnPreviewHover, this, pItem)*/);

				auto pButton = new UINewButton(pItem->ID, "Game\\Images\\ReviewSkins\\bnt_visualizar_.png", "Game\\Images\\ReviewSkins\\bnt_visualizar_.png", 48 + (168 * (count % 2)), 124 + (145 * (count / 2)), true, false, std::bind(&cReviewSkinWindow::OnButtonGetClick, this, pItem->Item, psPacket->InfoReview->Hand));

				pImage->setX(pImage->getX() + ((0 - pImage->getWidth()) / 2));
				pImage->setY(pImage->getY() + ((96 - pImage->getHeight()) / 2));

				pPreviewWindow->AddElement(pBgItem);
				pPreviewWindow->AddElement(pImage);
				pPreviewWindow->AddElement(pButton);
				pPreviewWindow->AddElement(pTitle);

				rCounts[psPacket->InfoReview[i].Section]++;
				vResultSkins.push_back(pItem);
			}
		}
	}
}

void cReviewSkinWindow::BuildImages()
{
	pWindow1 = new UINewWindow(Review_Window, "game\\Images\\ReviewSkins\\background.png", 0, 0, false);
	pWindow1->setHeight(pWindow1->getHeight() + 50);
	pWindow1->setX((WinSizeX - pWindow1->getWidth()) / 2 + 300);
	pWindow1->setY((WinSizeY - pWindow1->getHeight()) / 2);

	pWindow1->AddElement(new UINewButton(Review_Close, "game\\Images\\ReviewSkins\\fechar.png", "game\\Images\\ReviewSkins\\fechar_.png", 342, 6, true, false, std::bind(&cReviewSkinWindow::Close, this)));

	auto pPreviewWindow = new UINewWindow(Review_ReviewItens, 12, 70, 370, 350);
	auto pScroll = new UINewVerticalScroll(Review_Scroll, 350, 5, 0, 250, 0, 0);

	pScroll->hide();
	pPreviewWindow->SetScrollBar(pScroll);
	pWindow1->AddElement(pPreviewWindow);

	pWindow1->hide();
}

void cReviewSkinWindow::OnButtonGetClick(sITEM pItem, int Hand)
{
	lpCurPlayer->SetTool(0, hvPOSI_LHAND);
	lpCurPlayer->SetTool(0, hvPOSI_RHAND);

	DWORD itemCode = (pItem.CODE & sinITEM_MASK2);
	auto Class = lpCurPlayer->smCharInfo.JOB_CODE;

	switch (Class)
	{
	
	case JOBCODE_ARCHER:
	case JOBCODE_ATALANTA:
	case JOBCODE_FIGHTER:
	case JOBCODE_MECHANICIAN:
	case JOBCODE_PIKEMAN:
	case JOBCODE_KNIGHT:
		if (itemCode == sinDA2 )
			cMessageBox.ShowMessageEvent("Item não permitido para sua classe!");
		else
			sinSetCharItem(pItem.CODE, Hand, TRUE);
		break;

	case JOBCODE_MAGICIAN:
	case JOBCODE_PRIESTESS:
		if (itemCode == sinDA1 )
			cMessageBox.ShowMessageEvent("Item não permitido para sua classe!");
		else
			sinSetCharItem(pItem.CODE, Hand, TRUE);
		break;
	}
}

void cReviewSkinWindow::ClearItems()
{
	auto pList = pWindow1->GetElement<UINewWindow>(Review_ReviewItens);
	if (pList)
	{
		for (auto Item : vResultSkins)
		{
			pList->RemoveElementByID(Item->ID);
			delete Item;
		}
	}

	vResultSkins.clear();
}