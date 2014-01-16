

#include "dvb_c.h"
#include "tsfile.h"
#include "stdlib.h"


#define SOURCE_TS	"ts.dat"
#define BUFF_SIZE (204)


/*
	callback function:
		inject IQSigal to hardware.

*/
bool IQSignalProcCb(u32 CbData , IQSigal sig)
{
	static u32 wc = 0;
	CFile *pStorFile = (CFile *)CbData;
	//if(wc < 5)
	{
		//DBG("wc:%d, I:%d Q:%d;", wc, sig.I , sig.Q);
		//pStorFile->Write((u8 *)(&sig), sizeof(IQSigal), -1);
		wc++;
		sig.I += 7;
		pStorFile->Write((u8 *)(&(sig.I)), sizeof(u8), -1);
	}

	return true;
}


int test1(void)
{
	CTSFile *pFrom;
	CFile *pRand, *pRson, *pInter, *pMTuple, *pDiffEnc, *pISig, * pQSig;

	u8 I, Q, quad;
	//Qam64Symbol *pSbl;
	u8 A,  B, Qbits, Symbol;

	IQSigal sgl;
	dvb_c *pDvb_c;
	u8 buff[BUFF_SIZE], buff2[BUFF_SIZE];
	u32 PktIdx ;//, SmbIdx;

	//TestDataLenth();

	pFrom = new CTSFile(SOURCE_TS);
	pRand = new CFile("Randomization.dat");
	pRson = new CFile("RS_on.dat");
	pInter = new CFile("Interleave.dat");
	pMTuple = new CFile("m_tuple.dat");
	pDiffEnc = new CFile("diffenc.dat");
	pISig = new CFile("I_sig.dat");
	pQSig = new CFile("Q_sig.dat");
	pDvb_c = new dvb_c();

	if((pFrom->OpenAndAnalyse()) &&
			(pISig->Open(CFile::OPEN_BIN_READ_AND_WRITE)) &&
			(pQSig->Open(CFile::OPEN_BIN_READ_AND_WRITE)) &&
			(pDiffEnc->Open(CFile::OPEN_BIN_READ_AND_WRITE)) &&
			(pMTuple->Open(CFile::OPEN_BIN_READ_AND_WRITE)) &&
			(pRand->Open(CFile::OPEN_BIN_READ_AND_WRITE)) &&
			(pRson->Open(CFile::OPEN_BIN_READ_AND_WRITE)) &&
			(pInter->Open(CFile::OPEN_BIN_READ_AND_WRITE)))
	{

		if(false == pFrom->TraceTsfileState())
		{
			ERR("open ts file failed! \n");
		}


		pDvb_c->DvbCInit(NULL, 0);
		PktIdx = 0;

		while(PktIdx < pFrom->GetPacketCount())
		{
			pFrom->ReadPacket(buff, PktIdx);
			pDvb_c->Randomization(buff);
			pRand->Write(buff, 188, 188 * PktIdx);
			pDvb_c->rs_encode(buff);
			pRson->Write(buff, 204, 204 * PktIdx);
			pDvb_c->InterLeave(buff, buff2);
			pInter->Write(buff2, 204, 204 * PktIdx);

			pDvb_c->convert(buff2);
			while(true == pDvb_c->GetSymbol(QAM_64, A, B, Qbits))
			{
				Symbol = (A << 7) | (B << 6) | (Qbits << 2);
				pMTuple->Write(&Symbol, sizeof(u8), -1);
				pDvb_c->EncoderIQ(A, B, &I, &Q);
				//pSbl->A = I;
				//pSbl->B = Q;
				Symbol = (I << 7) | (Q << 6) | (Qbits << 2);
				pDiffEnc->Write(&Symbol, sizeof(u8), -1);

				quad = (I << 1 | Q);
				sgl = pDvb_c->MapQamIQ(QAM_64, quad, Qbits);
				sgl.I += 7;
				sgl.Q += 7;
				pISig->Write((u8 *)(&(sgl.I)), sizeof(u8), -1);
				pQSig->Write((u8 *)(&(sgl.Q)), sizeof(u8), -1);
			}

			PktIdx++;
		}
	}
	else
	{
		ERR("open file failed!!!");
	}
	/*delete pFrom;
	delete pRand;
	delete pRson;
	delete pInter;
	delete pDvb_c;
	delete pMTuple;
	delete pDiffEnc;
	delete pISig;
	delete pQSig;
	Object::DumpObjInfo();
*/
}


int test2(void)
{
	CTSFile *pFrom;
	CFile *pTo;

	dvb_c *pDvb_c;
	u8 buff[BUFF_SIZE], buff2[BUFF_SIZE];
	u32 PktIdx;


	pFrom = new CTSFile(SOURCE_TS);
	pTo = new CFile("IQ_Final.ts");
	pDvb_c = new dvb_c();

	if((pFrom->OpenAndAnalyse()) &&
			(pTo->Open(CFile::OPEN_BIN_WRITE_ONLY)))
	{
		pDvb_c->DvbCInit(IQSignalProcCb, (u32)pTo);
		PktIdx = 0;

		while(PktIdx < pFrom->GetPacketCount())
		{
			pFrom->ReadPacket(buff, PktIdx);
			pDvb_c->CCEncoder(buff, buff2);
			pDvb_c->Modulate(QAM_64, buff2);
			PktIdx++;
		}
	}
	else
	{
		ERR("open file failed!!!");
	}
	/*delete pFrom;
	//delete pTo;
	//delete pDvb_c;

	Object::DumpObjInfo();
*/
}


int test3(void)
{
	CFile *pMTuple, *pDiffEnc, * pISig , *pQSig ;

	dvb_c *pDvb_c;
	u8 mtuple;
	u32 MtpIdx, FileSize;
	u8 I, Q, quad;
	IQSigal sgl;

	pISig = new CFile("cj_I_sig.dat");
	pQSig = new CFile("cj_Q_sig.dat");

	pMTuple = new CFile("diffenc_input.dat");
	pDiffEnc = new CFile("cj_diffenc.dat");
	pDvb_c = new dvb_c();

	if((pMTuple->Open(CFile::OPEN_BIN_READ_ONLY)) &&
			(pDiffEnc->Open(CFile::OPEN_BIN_WRITE_ONLY)) &&
			(pISig->Open(CFile::OPEN_BIN_WRITE_ONLY)) &&
			(pQSig->Open(CFile::OPEN_BIN_WRITE_ONLY)))
	{
		pDvb_c->DvbCInit(NULL, (u32)0);

		MtpIdx = 0;
		pMTuple->GetLenth(FileSize);
		while(MtpIdx < FileSize)
		{
			pMTuple->Read((&mtuple), sizeof(u8), MtpIdx);
			pDvb_c->EncoderIQ((mtuple >> 7) & 0x1, (mtuple >> 6) & 0x1, &I, &Q);
			quad = (mtuple >> 6) & 0x3;
			mtuple &= (0x3 << 6);
			mtuple |= (I << 7);
			mtuple |= (Q << 6);
			pDiffEnc->Write((u8 *)(&mtuple), sizeof(u8), -1);

			sgl = pDvb_c->MapQamIQ(QAM_64, quad, (mtuple >> 2) & 0xF);
			sgl.I += 7;
			sgl.Q += 7;
			pISig->Write((u8 *)(&(sgl.I)), sizeof(u8), -1);
			pQSig->Write((u8 *)(&(sgl.Q)), sizeof(u8), -1);

			MtpIdx ++;
		}

	}
	else
	{
		ERR("open file failed!!!");
	}
	/*delete pMTuple;
	delete pDiffEnc;
	delete pDvb_c;
	delete pISig;
	delete pQSig;

	Object::DumpObjInfo();
*/
}


int test4(char *FileName)
{
	CTSFile *pFrom;

	dvb_c *pDvb_c;
	u8 buff[BUFF_SIZE], buff2[BUFF_SIZE];
	u32 PktIdx;


	pFrom = new CTSFile(FileName);
	pDvb_c = new dvb_c();

	if(pFrom->OpenAndAnalyse())
	{
		pDvb_c->DvbCInit(NULL, (u32)0);
		PktIdx = 0;
		INF("pDvb_c begin:");
		while(PktIdx < pFrom->GetPacketCount())
		{
			pFrom->ReadPacket(buff, PktIdx);
			pDvb_c->CCEncoder(buff, buff2);
			pDvb_c->Modulate(QAM_64, buff2);
			PktIdx++;
		}
		INF(" %d pkt pDvb_c end:", PktIdx);
	}
	else
	{
		ERR("open file %s failed!!!", FileName);
	}


}


int main(int argc, char *argv[])
{
	test1();
	test2();
	test3();
	if(argc > 1)
	{
		test4(argv[1]);
	}
	Object::DumpObjInfo();
	Object::FreeAllObj();
	Object::DumpObjInfo();
	return 0;
}

