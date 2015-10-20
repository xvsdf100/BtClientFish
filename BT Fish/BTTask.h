#ifndef BTTASK_H
#define BTTASK_H

class CBTask
{
public:
	CBTask();
	~CBTask();

	void Start();
	void Stop();

private:
	std::string torrentPath;
	std::string infoHash;
	int			fileSize;
	int			pieceSize;

};
#endif