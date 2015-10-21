#ifndef BTTASK_H
#define BTTASK_H
#include <string>
#include <vector>
#include "Utility.h"
#include "BTClient.h"
#include "BTDataManager.h"

class CBTTask
{
	typedef std::vector<CBTClientChannel> BTClientChannelArray;

	typedef struct tagBTInfo
	{
		string	TorrentPath;
		string	InfoHash;
		int_64	FileSize;
		int		PieceSize;
	}BTInfo;

public:
	CBTTask(const BTInfo& Info);
	~CBTTask();

	bool Start();
	void Stop();

private:
	void Init();

private:
	BTInfo					m_BTInfo;
	int						m_Status;		//ÁÙÊ±±£´æ×´Ì¬
	BTClientChannelArray	m_ChannelArray;	
	CBTDataManager*			m_DataManager;
};
#endif