//#include "sinbaram\sinLinkHeader.h"
#include "CManufactureWindow.h"
//#include "Utils/String.h"
//#pragma editandcontinue("Full")
#include "Shop/NewShop.h"

CManufactureWindow cManufactureWindow;

CManufactureWindow* CManufactureWindow::getInstance()
{
	return &cManufactureWindow;
}

void CManufactureWindow::Init()
{
	BuildWindow1();
}

void CManufactureWindow::Render()
{
	if (!pWindow1)
		return;
	
	if (pWindow1->isVisible())
		Resize();

	pWindow1->Render(0, 0, WinSizeX, WinSizeY, 0, 0);
}

void CManufactureWindow::Resize()
{
	if (!pWindow1)
		return;

	pWindow1->setX((WinSizeX - pWindow1->getWidth()) / 2);
	pWindow1->setY((WinSizeY - pWindow1->getHeight()) / 2);
}

bool CManufactureWindow::OnMouseClick(bool Type)
{
	if (!pWindow1)
		return false;
	return pWindow1->OnMouseClick(Type ? Down : Up, 0, 0, WinSizeX, WinSizeY, 0, 0);

}

void CManufactureWindow::OnMouseMove()
{
	if (!pWindow1)
		return;

	pWindow1->OnMouseMove(0, 0, WinSizeX, WinSizeY, 0, 0);
}

void CManufactureWindow::HandlePacket(PacketOpenManufactureWindow* psPacket)
{
	pWindow1->show();
}

void CManufactureWindow::HandlePacket(PacketRecipeInfo* psPacket)
{

	auto pChanceText = pWindow1->GetElement<UINewText>(Craft_ChanceCraftar);
	auto pPriceText = pWindow1->GetElement<UINewText>(Craft_Preco);

	if (pChanceText && pPriceText)
	{

		vResultItems.clear();

		pChanceText->setText((char*)FormatString("Chance de sucesso: %d%%", psPacket->Chance));
		pChanceText->show();

		char buff[255] = {};
		NumLineComa(psPacket->Price, buff);
		pPriceText->setText((char*)FormatString("Taxa: %dkc", psPacket->Price));
		pPriceText->show();

		Price = psPacket->Price;

		memcpy(StoneCode, psPacket->StoneCode, sizeof(psPacket->StoneCode));

		for (int i = 0; i < psPacket->Count; i++)
		{
			for (int cnt = 0; cnt < MAX_ITEM; cnt++)
			{
				if (sItem[cnt].CODE == psPacket->ResultItem[i].CODE)
				{
					sITEM Item;
					memcpy(&Item, &sItem[cnt], sizeof(sITEM));
					memcpy(&Item.sItemInfo, &psPacket->ResultItem[i], sizeof(sITEMINFO));

					bool canPut = true;

					for (int cnty = 0; cnty < 6 * 22; cnty += 22)
					{
						for (int cntx = 0; cntx < 12 * 22; cntx += 22)
						{
							canPut = true;
							for (auto StoredItem : vResultItems)
							{
								if (cntx > StoredItem.x - Item.w && cntx < StoredItem.x + StoredItem.w &&
									cnty > StoredItem.y - Item.h && cnty < StoredItem.y + StoredItem.h)
								{
									canPut = false;
									break;
								}
							}


						}

						if (canPut)
							break;
					}
					break;
				}
			}
		}
	}
}

void CManufactureWindow::BuildWindow1()
{
	pWindow1 = new UINewWindow(0, "game\\images\\UID\\ManufactureMain.bmp", 0, 0);
	pWindow1->setHeight(pWindow1->getHeight() + 50);
	pWindow1->setX((WinSizeX - pWindow1->getWidth()) / 2);
	pWindow1->setY((WinSizeY - pWindow1->getHeight()) / 2);

	pWindow1->AddElement(new UINewImage(0, "game\\images\\Craft\\ManufactureTitle.bmp", 96, -5));
	//pWindow1->AddElement(new UIImage(0, "game\\craft\\Craft\\my_money.bmp", 178, 179));
	pWindow1->AddElement(new UINewButton(0, "game\\images\\Craft\\xxx.png", "game\\craft\\Craft\\ButtonExit.bmp", 304, 207, true, false, std::bind(&CManufactureWindow::Close, this)));
//	pWindow1->AddElement(new UIButton(0, "image\\sinimage\\help\\help_icon01_.tga", "image\\sinimage\\help\\help_icon01.tga", 303, 19, true, false, std::bind(&CManufactureWindow::Help, this)));
	pWindow1->AddElement(new UINewButton(0, "game\\images\\Craft\\xxx.png", "game\\craft\\Craft\\Button.bmp", 170, 133, true, false, std::bind(&CManufactureWindow::OnCraftButtonClick, this)));//

	vector<POINT> FramePos = {
		{249, 115},  {224, 146}, {274, 146},
	};

	int count = 0;
	for (auto pos : FramePos)
	{
		auto pItemFrame = new UINewItemFrame(Craft_Slots + count++, pos.x, pos.y, 24, 24);
		pItemFrame->setHover(std::bind(&CManufactureWindow::OnFrameHover, this, pItemFrame, &MouseItem));
		pWindow1->AddElement(pItemFrame);
	}

	auto pMainFrame = new UINewItemFrame(Craft_Receita, 68, 99, 67, 89);
	pMainFrame->setHover(std::bind(&CManufactureWindow::OnMainFrameHover, this, pMainFrame, &MouseItem));
	pMainFrame->setPutFunc(std::bind(&CManufactureWindow::OnPutRecipe, this, pMainFrame));
	pMainFrame->setPickFunc(std::bind(&CManufactureWindow::OnPickRecipe, this));
	pWindow1->AddElement(pMainFrame);


	auto pChanceText = new UINewText(Craft_ChanceCraftar, "Chance: 0%", 30, 209, false);
	pChanceText->hide();
	pWindow1->AddElement(pChanceText);

	auto pPriceText = new UINewText(Craft_Preco, "0", 228, 209);
	pPriceText->hide();
	pWindow1->AddElement(pPriceText);

	pWindow1->hide();
}

void CManufactureWindow::Close()
{
	if (MouseItem.Flag)
		return;

	auto pMainFrame = pWindow1->GetElement<UINewItemFrame>(Craft_Receita);
	if (pMainFrame)
	{
		if (pMainFrame->getItem()->Flag)
		{
			pMainFrame->pickItem(&MouseItem);
			cInvenTory.AutoSetInvenItem(&MouseItem);
		}
	}

	

	auto pChanceText = pWindow1->GetElement<UINewText>(Craft_ChanceCraftar);
	auto pPriceText = pWindow1->GetElement<UINewText>(Craft_Preco);

	if (pChanceText)
		pChanceText->hide();

	if (pPriceText)
		pPriceText->hide();

	pWindow1->hide();
}

void CManufactureWindow::Help()
{
}

void CManufactureWindow::OnCraftButtonClick()
{
	if (NewShop::GetInstance()->GetCoin() < Price)
	{
		cMessageBox.ShowMessageEvent("Você não tem Coins suficiente.");
		
		return;
	}

	auto pMainFrame = pWindow1->GetElement<UINewItemFrame>(Craft_Receita);
	PacketManufactureRecipe sPacket = {};
	sITEM* pRecipe = pMainFrame->getItem();
	vector<sITEM*> vRunes;

	sPacket.code = smTRANSCODE_MANUFACTURE_RECIPE;
	sPacket.size = sizeof(sPacket);

	if (pRecipe->Flag)
	{
		bool bValid = true;
		lstrcpy(sPacket.Recipe.ItemCode, pRecipe->LastCategory);
		sPacket.Recipe.Code = pRecipe->CODE;
		sPacket.Recipe.Head = pRecipe->sItemInfo.ItemHeader.Head;
		sPacket.Recipe.ChkSum = pRecipe->sItemInfo.ItemHeader.dwChkSum;

		for (int i = 0; i < 3; i++)
		{
			auto pItemFrame = pWindow1->GetElement<UINewItemFrame>(Craft_Slots + i);
			if (pItemFrame)
			{
				auto pItem = pItemFrame->getItem();
				if (StoneCode[i][0] && pItem->Flag && !lstrcmpi(StoneCode[i], pItem->LastCategory))
				{
					lstrcpy(sPacket.Runes[i].ItemCode, pItem->LastCategory);
					sPacket.Runes[i].Code = pItem->CODE;
					sPacket.Runes[i].Head = pItem->sItemInfo.ItemHeader.Head;
					sPacket.Runes[i].ChkSum = pItem->sItemInfo.ItemHeader.dwChkSum;
					vRunes.push_back(pItem);
				}
				else if (StoneCode[i][0])
				{
					bValid = false;
					break;
				}
				else break;
			}
		}

		if (bValid)
		{
			ZeroMemory(pRecipe, sizeof(sITEM));

			for (auto pItem : vRunes)
				ZeroMemory(pItem, sizeof(sITEM));

			//sinMinusMoney(Price);

			


			smWsockServer->Send((char*)&sPacket, sPacket.size, TRUE);

			Close();
		}
		else
		{
			cMessageBox.ShowMessageEvent("Runas incorretas.");
			
			return;
		}
	}
}

void CManufactureWindow::OnFrameHover(UINewItemFrame* pItemFrame, sITEM* pItem)
{
if (!pItemFrame || !pItem)
		return;

	bool canPut = false;
	if (pItem->Flag)
	{
		DWORD itemCode = (pItem->CODE & sinITEM_MASK2);

		if (itemCode == sinPR1 || itemCode == sinPR2 || itemCode == sinPR3)
		{
			int frameID = pItemFrame->getID() - 0x100;
			if (!lstrcmpi(pItem->LastCategory, StoneCode[frameID]))
				canPut = true;
		}
	}
	else if (pItemFrame->getItem()->Flag)
		canPut = true;

	pItemFrame->setPutItem(canPut);
}
// por verificação 
void CManufactureWindow::OnMainFrameHover(UINewItemFrame* pItemFrame, sITEM* pItem)
{
	if (!pItemFrame || !pItem)
		return;

	if (pItem->Flag)
	{
		DWORD itemCode = (pItem->CODE & sinITEM_MASK2);

		if (itemCode == sinDR1 || itemCode == sinOA2 || itemCode == sinDB1 || itemCode == sinDG1)
			pItemFrame->setPutItem(true);
		else
			pItemFrame->setPutItem(false);

	}
	else if (pItemFrame->getItem()->Flag)
		pItemFrame->setPutItem(true);
	else
		pItemFrame->setPutItem(false);
}

void CManufactureWindow::OnPutRecipe(UINewItemFrame* pItemFrame)
{
	ResetRuneFrames();

	sITEM* pItem = pItemFrame->getItem();
	PacketGetRecipe sPacket;
	sPacket.size = sizeof(sPacket);
	sPacket.code = smTRANSCODE_GET_RECIPE;
	lstrcpy(sPacket.RecipeCode, pItem->LastCategory);
	smWsockServer->Send((char*)&sPacket, sPacket.size, TRUE);
}

void CManufactureWindow::OnPreviewHover(sITEM* pItem)
{
	//int bkpx = pItem->x;
	//int bkpy = pItem->y;
	//pItem->x = pCursorPos.x;
	//pItem->y = pCursorPos.y;
	//cItem.ShowItemInfo(pItem, 1);
	//sinShowItemInfoFlag = 1;
	//pItem->x = bkpx;
	//pItem->y = bkpy;
}

void CManufactureWindow::OnPickRecipe()
{

	auto pChanceText = pWindow1->GetElement<UINewText>(Craft_ChanceCraftar);
	auto pPriceText = pWindow1->GetElement<UINewText>(Craft_Preco);

	if (pChanceText && pPriceText)
	{
		vResultItems.clear();
		pChanceText->hide();
		pPriceText->hide();

		ResetRuneFrames();

		ZeroMemory(StoneCode, sizeof(StoneCode));
	}
}

void CManufactureWindow::ResetRuneFrames()
{
	for (int i = 0; i < 12; i++)
	{
		auto pItemFrame = pWindow1->GetElement<UINewItemFrame>(Craft_Slots + i);
		if (pItemFrame)
		{
			if (pItemFrame->getItem()->Flag)
			{
				sITEM tempItem;
				pItemFrame->pickItem(&tempItem);
				cInvenTory.AutoSetInvenItem(&tempItem);
			}
		}
	}
}