#include "test_kdvmedianet_func.h"

#define MAX_FRAME_SIZE	(1024)
#define NETBAND			(100*1024)
#define FRAMERATE		(20)
#define MEDIA_TYPE		(MEDIA_TYPE_H264)
#define SSRC			(123456)
#define REMOTE_PAYLOAD	(323)

/*===============error number===============*/
#define ERROR_BASE
#define OPEN_FILE_ERROR		(ERROR_BASE+1)
#define MALLOC_BUF_ERROR	(ERROR_BASE+2)
/*==========================================*/

void CTestKdvmedianetFunc::SetUpTestCase()
{

}

void CTestKdvmedianetFunc::TearDownTestCase()
{

}

void CTestKdvmedianetFunc::SetUp()
{

}

void CTestKdvmedianetFunc::TearDown()
{

}

//初始化snd
CKdvMediaSnd* CKdvMediaSndInit()
{
	CKdvMediaSnd *pcSnd = new CKdvMediaSnd;
	if(NULL == pcSnd)
	{
		printf("pcSnd = NULL , %s(%d)\n", __FUNCTION__, __LINE__);
		return NULL;
	}

	return pcSnd;
}

//反初始化snd
void CKdvMediaSndUnInit(CKdvMediaSnd* pcSnd)
{
	delete pcSnd;
}

//初始化rcv
CKdvMediaRcv* CKdvMediaRcvInit()
{
	CKdvMediaRcv *pcRcv = new CKdvMediaRcv;
	assert(pcRcv != NULL);
	return pcRcv;
}

//反初始化rcv
void CKdvMediaRcvUnInit(CKdvMediaRcv * pcRcv)
{
	delete pcRcv;
}

CKdvMediaSnd *g_pcSnd = NULL;
CKdvMediaRcv *g_pcRcv = NULL;

/*============================rcv===================================*/

//rcv 回调函数, 单位帧
void CallBack(PFRAMEHDR pFrmHdr, KD_PTR pContext)
{
	printf("rcv ...\n");
}

//rtcp回调函数
void RtcpCallBack(TRtcpInfo *pRtcpInfo, KD_PTR pContext)
{
	printf("rtcp call back ...\n");
}

//ps 回调
void PsCallBack(PFRAMEHDR pFrmHdr, KD_PTR pContext)
{
	printf("ps call back ...\n");
}

//create rcv
TEST_F(CTestKdvmedianetFunc, RcvCreate)
{
	//osp init
	BOOL32 bRet = OspInit(true, 2500);
	if(bRet != TRUE)
	{
		printf("osp init failure\n");
	}

	//调用socket start up()
	u16 wRet = KdvSocketStartup();

	if(wRet != 0)
	{
		printf("kdvsocketstartup failure\n");
	}
	
	g_pcRcv = CKdvMediaRcvInit();
	assert(g_pcRcv != NULL);

	wRet = g_pcRcv->Create(SSRC, CallBack, NULL, SSRC);

	EXPECT_EQ(MEDIANET_NO_ERROR, wRet);
}

// set rcv param
TEST_F(CTestKdvmedianetFunc, RcvSetRcvLocalParam)
{
	TLocalNetParam tRcvParam;
	memset(&tRcvParam, 0, sizeof(tRcvParam));
	TNetSession tRcvSession ;
	memset(&tRcvSession, 0, sizeof(tRcvSession));
	tRcvSession.m_dwRTPAddr  = inet_addr("127.0.0.1");
	tRcvSession.m_dwRTCPAddr = inet_addr("127.0.0.1");
	tRcvSession.m_wRTPPort   = htonl(8005);
	tRcvSession.m_wRTCPPort  = htonl(8006);
	tRcvParam.m_tLocalNet = tRcvSession;

	u16 wRet = g_pcRcv->SetNetRcvLocalParam(tRcvParam);

	EXPECT_EQ(MEDIANET_NO_ERROR, wRet);
}

//set ActivePT
TEST_F(CTestKdvmedianetFunc, RcvSetActivePT)
{
	u16 wRet = g_pcRcv->SetActivePT(REMOTE_PAYLOAD, MEDIA_TYPE_H264);

	EXPECT_EQ(MEDIANET_NO_ERROR, wRet);
}

//set rtcp callback info
TEST_F(CTestKdvmedianetFunc, RcvSetRtcpCallBackInfo)
{
	u16 wRet = g_pcRcv->SetRtcpInfoCallback(RtcpCallBack, 0);

	EXPECT_EQ(MEDIANET_NO_ERROR, wRet);
}

//get Status
TEST_F(CTestKdvmedianetFunc, RcvGetStatus)
{
	TKdvRcvStatus tStatus = {0};

	u16 wRet = g_pcRcv->GetStatus(tStatus);

	EXPECT_EQ(MEDIANET_NO_ERROR, wRet);
}

//get statistics
TEST_F(CTestKdvmedianetFunc, RcvGetStatistics)
{
	TKdvRcvStatus tStatistics = {0};

	u16 wRet = g_pcRcv->GetStatus(tStatistics);

	EXPECT_EQ(MEDIANET_NO_ERROR, wRet);
}

//get max frame size
TEST_F(CTestKdvmedianetFunc, RcvGetMaxFrameSize)
{
	u32 dwMaxFramseSize = 0;
	u16 wRet = g_pcRcv->GetMaxFrameSize(dwMaxFramseSize);

	EXPECT_EQ(MEDIANET_NO_ERROR, wRet);
}

//get max frame size
TEST_F(CTestKdvmedianetFunc, RcvGetLocalMediaType)
{
	u8 cMediaType = 0;
	u16 wRet = g_pcRcv->GetLocalMediaType(cMediaType);

	EXPECT_EQ(MEDIANET_NO_ERROR, wRet);
}

//set times stampSample
TEST_F(CTestKdvmedianetFunc, SetTsSample)
{
	u32 dwSample = 9600;
	u16 wRet = g_pcRcv->SetTimestampSample(dwSample);

	EXPECT_EQ(MEDIANET_NO_ERROR, wRet);
}

//ps call back
TEST_F(CTestKdvmedianetFunc, RcvpsCallBack)
{
	BOOL32 bPsCallBackWithoutHead = false;
	u16 wRet = g_pcRcv->AddPsFrameCallBack(PsCallBack, bPsCallBackWithoutHead, NULL);

	EXPECT_EQ(MEDIANET_NO_ERROR, wRet);
}

//rcv 4k 
TEST_F(CTestKdvmedianetFunc, RcvSetIs4k)
{
	BOOL32 bis4K = true;
	u16 wRet = g_pcRcv->SetIs4k(bis4K);

	EXPECT_EQ(MEDIANET_NO_ERROR, wRet);
}

//set SetAaclcTransMode
TEST_F(CTestKdvmedianetFunc, RcvSetAaclcTransMode)
{
	BOOL32 bis4K = true;
	u16 wRet = g_pcRcv->SetIs4k(bis4K);

	EXPECT_EQ(MEDIANET_NO_ERROR, wRet);
}

//set stream type
TEST_F(CTestKdvmedianetFunc, RcvSetStreamType)
{
	ENetStreamType eStreamType = EstreamType_PS;
	u16 wRet = g_pcRcv->SetStreamType(eStreamType);

	EXPECT_EQ(MEDIANET_NO_ERROR, wRet);
}

//set CBTsCalc
TEST_F(CTestKdvmedianetFunc, RcvCBTsCalc)
{
	ENetStreamType eStreamType = EstreamType_PS;
	u16 wRet = g_pcRcv->SetStreamType(eStreamType);

	EXPECT_EQ(MEDIANET_NO_ERROR, wRet);
}

//start rcv
TEST_F(CTestKdvmedianetFunc, StartRcv)
{

	u16 wRet = g_pcRcv->StartRcv();

	EXPECT_EQ(MEDIANET_NO_ERROR, wRet);
}

/*==========================rcv  end================================*/

/*============================snd===================================*/

//rtcp call back
void SndRtcpCallBack(TRtcpInfo *pRtcpInfo, KD_PTR pContext)
{
	printf("snd rtcp call back info here ...\n");
}
//create snd
TEST_F(CTestKdvmedianetFunc, SndCreate)
{
	g_pcSnd = CKdvMediaSndInit();
	assert(g_pcSnd != NULL);

	u16 wRet = g_pcSnd->Create(MAX_FRAME_SIZE, NETBAND, FRAMERATE, MEDIA_TYPE_H264, SSRC, NULL);

	EXPECT_EQ(MEDIANET_NO_ERROR, wRet);
}

//set info
TEST_F(CTestKdvmedianetFunc, RcvSetNetSndParam)
{
	assert(g_pcSnd != NULL);

	TNetSession tSession;
	memset(&tSession, 0, sizeof(tSession));
	tSession.m_dwRTCPAddr = inet_addr("127.0.0.1");
	tSession.m_dwRTPAddr  = inet_addr("127.0.0.1");
	tSession.m_wRTCPPort  = 8005;
	tSession.m_wRTPPort   = 8006;

	TNetSndParam tSndParam = {0};
	tSndParam.m_byNum = 1;
	tSndParam.m_tLocalNet = tSession;

	u16 wRet = g_pcSnd->SetNetSndParam(tSndParam);

	EXPECT_EQ(MEDIANET_NO_ERROR, wRet);
}

//set activePT
TEST_F(CTestKdvmedianetFunc, SndSetActivePT)
{
	u8 byLocalActivePT = true;
	u16 wRet = g_pcSnd->SetActivePT(byLocalActivePT);

	EXPECT_EQ(MEDIANET_NO_ERROR, wRet);
}

//set encrypt
TEST_F(CTestKdvmedianetFunc, SndSetEncrypt)
{
	s8 *pszBuf = "sndTest";
	u16 wKeySize = DES_ENCRYPT_KEY_SIZE;
	u8 byEncryptMode = DES_ENCRYPT_MODE;
	u16 wRet = g_pcSnd->SetEncryptKey(pszBuf, wKeySize, byEncryptMode);

	EXPECT_EQ(MEDIANET_NO_ERROR, wRet);
}

//set rtcp info callback
TEST_F(CTestKdvmedianetFunc, SndSetRtcpInfoCallBack)
{
	u16 wRet = g_pcSnd->SetRtcpInfoCallback(SndRtcpCallBack, NULL);

	EXPECT_EQ(MEDIANET_NO_ERROR, wRet);
}

//ResetFrameId
TEST_F(CTestKdvmedianetFunc, SndResetFrameId)
{
	u16 wRet = g_pcSnd->ResetFrameId();

	EXPECT_EQ(MEDIANET_NO_ERROR, wRet);
}

//reset ssrc
TEST_F(CTestKdvmedianetFunc, SndResetSsrc)
{
	u32 dwSsrc = 987654;
	u16 wRet = g_pcSnd->ResetSSRC(dwSsrc);

	EXPECT_EQ(MEDIANET_NO_ERROR, wRet);
}

//reset rs flag
TEST_F(CTestKdvmedianetFunc, SndResetRSFlag)
{
	u16 wBufTimeSpan = 10;
	BOOL32 bRepeat = true;
	u16 wRet = g_pcSnd->ResetRSFlag(wBufTimeSpan, bRepeat);

	EXPECT_EQ(MEDIANET_NO_ERROR, wRet);
}

//get send info
TEST_F(CTestKdvmedianetFunc, SndGetInfo)
{
	TKdvSndStatus tSndInfo = {0};
	u16 wRet = g_pcSnd->GetStatus(tSndInfo);

	EXPECT_EQ(MEDIANET_NO_ERROR, wRet);
}

//get send statistics
TEST_F(CTestKdvmedianetFunc, SndGetStatistics)
{
	TKdvSndStatistics tStatistics= {0};
	u16 wRet = g_pcSnd->GetStatistics(tStatistics);

	EXPECT_EQ(MEDIANET_NO_ERROR, wRet);
}

//send
TEST_F(CTestKdvmedianetFunc, Send)
{
	u16 wRet = 0;
	FRAMEHDR tFrameHeader = {0};
	BOOL32 bReadFile = true;

	tFrameHeader.m_pData = (u8*)malloc(MAX_FRAME_SIZE);
	if(NULL == tFrameHeader.m_pData)
	{
		bReadFile = false;
		wRet = MALLOC_BUF_ERROR;
	}
	tFrameHeader.m_dwDataSize = MAX_FRAME_SIZE;
	tFrameHeader.m_byMediaType	= MEDIA_TYPE_H264;
	tFrameHeader.m_dwDataSize	= 1500;
	tFrameHeader.m_byFrameRate	= 20;
	tFrameHeader.m_dwFrameID	= 323;
	tFrameHeader.m_dwSSRC		= SSRC;
	tFrameHeader.m_dwTimeStamp	= 0;

	FILE* pf = fopen("../src/data.264", "r");
	if(NULL == pf)
	{
		printf("fopen data error\n");
		wRet = OPEN_FILE_ERROR;
		bReadFile = false;
	}
	fseek(pf, 0L, SEEK_SET);
	while(bReadFile)
	{
		if(feof(pf))
			break;

		fread(tFrameHeader.m_pData, 1, 1024, pf);
		tFrameHeader.m_dwTimeStamp += 40;

		if(0 == tFrameHeader.m_dwTimeStamp / 36000)
			tFrameHeader.m_tVideoParam.m_bKeyFrame = true;
		else
			tFrameHeader.m_tVideoParam.m_bKeyFrame = false;

		tFrameHeader.m_tVideoParam.m_wVideoHeight = 960;
		tFrameHeader.m_tVideoParam.m_wVideoWidth  = 1024;
		wRet = g_pcSnd->Send(&tFrameHeader, true, true);

		if(MEDIANET_NO_ERROR != wRet)
			break;
	}
	
	fclose(pf);
	pf = NULL;
	free(tFrameHeader.m_pData);
	EXPECT_EQ(MEDIANET_NO_ERROR, wRet);
}