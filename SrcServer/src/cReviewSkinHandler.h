#pragma once


struct PacketReviewSkin
{
	int size, code;
	int Count;
	int Coins;
	bool bFirst;
	int  Sections;
	struct Info
	{
		sITEMINFO ResultItem;
		int Section;
		int Hand;

	} InfoReview[10];


};

class cReviewSkinHandler
{
public:
	static cReviewSkinHandler* getInstance() { static cReviewSkinHandler instance; return &instance; };

	void SendOpenReviewSkin(rsPLAYINFO* lpPlayInfo);

	void HandlePacket(rsPLAYINFO* lpPlayInfo, PacketReviewSkin* psPacket);
};

#define REVIEWSKINHANDLER cReviewSkinHandler::getInstance()