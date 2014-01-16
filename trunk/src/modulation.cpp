/*
 * modulation.cpp - 	 .
 *
 * Copyright (C) 2013 gdtd.
 *
 * Author: Tangweimin (soupyman@gmail.com)
 * Create: 2013-12-5
 */

#include "modulation.h"

bool conversion::Convert(u8 *pPkt)
{
	if(0 == ByteCout)
	{
		memcpy(ByteBuff, pPkt, 204);
		ByteCout = 204;
		return true;
	}
	else
	{
		ERR("ByteCout  %d", ByteCout);
		return false;
	}
}
/*
Qam64Symbol *conversion::GetSymbol(void)
{
	if(((ByteCout * 8) + BitCout) < 6)
	{
		return NULL;
	}
	if(BitCout < 6)
	{
		//BitBuff = BitBuff | (ByteBuff[ByteCout - 1] << BitCout);
		BitBuff = (BitBuff << 8) | (ByteBuff[204 - ByteCout ]);
		BitCout += 8;
		ByteCout --;
	}
	Symbol64.A = (BitBuff >> (BitCout - 1)) & 0x1;
	Symbol64.B = (BitBuff >> (BitCout - 2)) & 0x1;
	Symbol64.qbits = (BitBuff >> (BitCout - 6)) & 0xF; // (BitBuff & 0x3C) >> 2;
	//BitBuff = BitBuff >> 6;
	BitCout -= 6;
	return &Symbol64;
}

Qam16Symbol *conversion::GetSymbol(void)
{
	if(((ByteCout * 8) + BitCout) < 4)
	{
		return NULL;
	}
	if(BitCout < 4)
	{
		BitBuff = (BitBuff << 8) | (ByteBuff[204 - ByteCout ]);
		BitCout += 8;
		ByteCout --;
	}
	Symbol16.A = (BitBuff >> (BitCout - 1)) & 0x1;
	Symbol16.B = (BitBuff >> (BitCout - 2)) & 0x1;
	Symbol16.qbits = (BitBuff >> (BitCout - 4)) & 0x3; // (BitBuff & 0x3C) >> 2;
	BitCout -= 4;
	return &Symbol16;
}

Qam32Symbol *conversion::GetSymbol(void)
{
	if(((ByteCout * 8) + BitCout) < 5)
	{
		return NULL;
	}
	if(BitCout < 5)
	{
		BitBuff = (BitBuff << 8) | (ByteBuff[204 - ByteCout ]);
		BitCout += 8;
		ByteCout --;
	}
	Symbol32.A = (BitBuff >> (BitCout - 1)) & 0x1;
	Symbol32.B = (BitBuff >> (BitCout - 2)) & 0x1;
	Symbol32.qbits = (BitBuff >> (BitCout - 5)) & 0x7; // (BitBuff & 0x3C) >> 2;
	BitCout -= 5;
	return &Symbol32;
}
Qam128Symbol *conversion::GetSymbol(void)
{
	if(((ByteCout * 8) + BitCout) < 7)
	{
		return NULL;
	}
	if(BitCout < 7)
	{
		BitBuff = (BitBuff << 8) | (ByteBuff[204 - ByteCout ]);
		BitCout += 8;
		ByteCout --;
	}
	Symbol128.A = (BitBuff >> (BitCout - 1)) & 0x1;
	Symbol128.B = (BitBuff >> (BitCout - 2)) & 0x1;
	Symbol128.qbits = (BitBuff >> (BitCout - 7)) & 0x1F; // (BitBuff & 0x3C) >> 2;
	BitCout -= 7;
	return &Symbol128;
}
*/

bool conversion::GetSymbol(DvbMdlType_e tp, u8 &A, u8 &B, u8 &Qbits)
{
	u8 bitlen = 0;
	switch(tp)
	{
		case QAM_16:
			bitlen = 4;
			break;
		case QAM_32:
			bitlen = 5;
			break;
		case QAM_64:
			bitlen = 6;
			break;
		case QAM_128:
			bitlen = 7;
			break;
		case QAM_256:
			bitlen = 8;
			break;
		default:
			ERR("param error tp 0x%x .", tp);
			break;
	}

	if(((ByteCout * 8) + BitCout) < bitlen)
	{
		return false;
	}
	if(BitCout < bitlen)
	{
		BitBuff = (BitBuff << 8) | (ByteBuff[204 - ByteCout ]);
		BitCout += 8;
		ByteCout --;
	}
	A = (BitBuff >> (BitCout - 1)) & 0x1;
	B = (BitBuff >> (BitCout - 2)) & 0x1;
	Qbits = (BitBuff >> (BitCout - bitlen)) & (0xff >> (10 - bitlen)); // (BitBuff & 0x3C) >> 2;
	BitCout -= bitlen;
	return true;
}


bool convolutional::CvlInit(PunctureCodeRate cr)
{

	switch(cr)
	{
		case CODE_RATE_1_2 :  // 1/2
			PunctureMax = 2;
			IQBMax = 2;
			PunBool[0] = true;
			PunBool[1] = true;
			break;
		case(CODE_RATE_2_3) : // 2/3
			PunctureMax = 4;
			IQBMax = 3;
			PunBool[0] = true;
			PunBool[1] = true;
			PunBool[2] = false;
			PunBool[3] = true;
			break;
		case(CODE_RATE_3_4) : // 3/4
			PunctureMax = 6;
			IQBMax = 4;
			for(int i = 0; i < PunctureMax; i++)
			{
				PunBool[i] = true;
			}
			PunBool[2] = false;
			PunBool[5] = false;
			break;
		case(CODE_RATE_5_6) : //5/6
			PunctureMax = 10;
			IQBMax = 6;
			for(int i = 0; i < PunctureMax; i++)
			{
				PunBool[i] = true;
			}
			PunBool[2] = false;
			PunBool[5] = false;
			PunBool[6] = false;
			PunBool[9] = false;
			break;
		case(CODE_RATE_7_8) : //7/8
			PunctureMax = 14;
			IQBMax = 8;
			for(int i = 0; i < PunctureMax; i++)
			{
				PunBool[i] = true;
			}
			PunBool[2] = false;
			PunBool[4] = false;
			PunBool[6] = false;
			PunBool[9] = false;
			PunBool[10] = false;
			PunBool[13] = false;
			break;
		default:
			ERR(" no such a convolutional code %d", cr);
			break;
	}
	PuncIdx = 0;
}

bool convolutional::Convert(u8 *pPkt)
{
	if(0 == ByteCout)
	{
		memcpy(ByteBuff, pPkt, 204);
		ByteCout = 204;
		return true;
	}
	else
	{
		ERR("ByteCout  %d", ByteCout);
		return false;
	}
}

bool convolutional::GetSymbol(u8 &I, u8 &Q)
{
	if(IQBMax <= IQBIdx)
	{
		u8 X, Y, idx;
		if(0 == ByteCout)
		{
			return false;
		}
		G12BuffBit = (G12BuffBit << 8) | ByteBuff[204 - ByteCout];
		ByteCout --;
		IQBIdx = 0;
		IQBMax = 0;
		//IQBIdx = PunctureMax;
		for(idx = 0 ; idx < 8; idx ++)
		{
			/* G1(X) = 171 oct. G2(X) = 133 oct. */
			X = ((G12BuffBit >> (7 - idx)) & 0x1) ^ ((G12BuffBit >> (7 - idx + 1)) & 0x1) ^ ((G12BuffBit >> (7 - idx + 2)) & 0x1)
				^ ((G12BuffBit >> (7 - idx + 3)) & 0x1) ^ ((G12BuffBit >> (7 - idx + 6)) & 0x1);
			Y = ((G12BuffBit >> (7 - idx)) & 0x1) ^ ((G12BuffBit >> (7 - idx + 2)) & 0x1) ^ ((G12BuffBit >> (7 - idx + 3)) & 0x1)
				^ ((G12BuffBit >> (7 - idx + 5)) & 0x1) ^ ((G12BuffBit >> (7 - idx + 6)) & 0x1);
			/*need puncture following  */
			if(PunBool[PuncIdx++])
				IQBuff[IQBMax++ ] = X;

			if(PuncIdx >= PunctureMax)
				PuncIdx = 0;

			if(PunBool[PuncIdx++])
				IQBuff[IQBMax ++] = Y;

			if(PuncIdx >= PunctureMax)
				PuncIdx = 0;

		}
	}
	I = IQBuff[IQBIdx ++];
	Q = IQBuff[IQBIdx ++];
	return true;
}

IQSigal mapping::QPSKMap[4] =
{
	{1, 1}, {1, -1}, { -1, 1}, { -1, -1}
};

IQSigal mapping::Qam64Map[4][16] =
{
	{	{1, 1}, {3, 1}, {1, 3}, {3, 3},
		{7, 1}, {5, 1}, {7, 3}, {5, 3},
		{1, 7}, {3, 7}, {1, 5}, {3, 5},
		{7, 7}, {5, 7}, {7, 5}, {5, 5}
	},

	{	{1, -1}, {1, -3}, {3, -1}, {3, -3},
		{1, -7}, {1, -5}, {3, -7}, {3, -5},
		{7, -1}, {7, -3}, {5, -1}, {5, -3},
		{7, -7}, {7, -5}, {5, -7}, {5, -5}
	},

	{	{ -1, 1}, { -1, 3}, { -3, 1}, { -3, 3},
		{ -1, 7}, { -1, 5}, { -3, 7}, { -3, 5},
		{ -7, 1}, { -7, 3}, { -5, 1}, { -5, 3},
		{ -7, 7}, { -7, 5}, { -5, 7}, { -5, 5}
	},

	{	{ -1, -1}, { -3, -1}, { -1, -3}, { -3, -3},
		{ -7, -1}, { -5, -1}, { -7, -3}, { -5, -3},
		{ -1, -7}, { -3, -7}, { -1, -5}, { -3, -5},
		{ -7, -7}, { -5, -7}, { -7, -5}, { -5, -5}
	},
};

IQSigal mapping::Qam16Map[4][4] =
{
	{
		{1, 1}, {3, 1},
		{1, 3}, {3, 3}
	},

	{
		{1, -1}, {1, -3},
		{3, -1}, {3, -3}
	},

	{
		{ -1, 1}, { -1, 3},
		{ -3, 1}, { -3, 3}
	},

	{
		{ -1, -1}, { -3, -1},
		{ -1, -3}, { -3, -3}
	},
};


IQSigal mapping::Qam32Map[4][8] =
{
	{
		{1, 1}, {3, 1}, {3, 5},
		{5, 1}, {1, 3}, {3, 3},
		{1, 5}, {5, 3}
	},

	{
		{1, -1}, {1, -3}, {5, -3},
		{1, -5}, {3, -1}, {3, -3},
		{5, -1}, {3, -5}
	},

	{
		{ -1, 1}, { -1, 3}, { -5, 3},
		{ -1, 5}, { -3, 1}, { -3, 3},
		{ -5, 1}, { -3, 5}
	},

	{
		{ -1, -1}, { -3, -1}, { -3, -5},
		{ -5, -1}, { -1, -3}, { -3, -3},
		{ -1, -5}, { -5, -3}
	},
};

IQSigal mapping::Qam128Map[4][32] =
{
	{
		{1, 1}, {3, 1}, {1, 3}, {3, 3}, {7, 1}, {5, 1},
		{7, 3}, {5, 3}, {7, 9}, {5, 9}, {7, 11}, {5, 11},
		{9, 1}, {11, 1}, {9, 3}, {11, 3}, {1, 7}, {3, 7},
		{1, 5}, {3, 5}, {7, 7}, {5, 7}, {7, 5}, {5, 5},
		{1, 9}, {3, 9}, {1, 11}, {3, 11}, {9, 7}, {11, 7},
		{9, 5}, {11, 5}
	},

	{
		{1, -1}, {1, -3}, {3, -1}, {3, -3}, {1, -7}, {1, -5},
		{3, -7}, {3, -5}, {9, -7}, {9, -5}, {11, -7}, {11, -5},
		{1, -9}, {1, -11}, {3, -9}, {3, -11}, {7, -1}, {7, -3},
		{5, -1}, {5, -3}, {7, -7}, {7, -5}, {5, -7}, {5, -5},
		{9, -1}, {9, -3}, {11, -1}, {11, -3}, {7, -9}, {7, -11},
		{5, -9}, {5, -11}
	},

	{
		{ -1, 1}, { -1, 3}, { -3, 1}, { -3, 3}, { -1, 7}, { -1, 5},
		{ -3, 7}, { -3, 5}, { -9, 7}, { -9, 5}, { -11, 7}, { -11, 5},
		{ -1, 9}, { -1, 11}, { -3, 9}, { -3, 11}, { -7, 1}, { -7, 3},
		{ -5, 1}, { -5, 3}, { -7, 7}, { -7, 5}, { -5, 7}, { -5, 5},
		{ -9, 1}, { -9, 3}, { -11, 1}, { -11, 3}, { -7, 9}, { -7, 11},
		{ -5, 9}, { -5, 11}
	},

	{
		{ -1, -1}, { -3, -1}, { -1, -3}, { -3, -3}, { -7, -1}, { -5, -1},
		{ -7, -3}, { -5, -3}, { -7, -9}, { -5, -9}, { -7, -11}, { -5, -11},
		{ -9, -1}, { -11, -1}, { -9, -3}, { -11, -3}, { -1, -7}, { -3, -7},
		{ -1, -5}, { -3, -5}, { -7, -7}, { -5, -7}, { -7, -5}, { -5, -5},
		{ -1, -9}, { -3, -9}, { -1, -11}, { -3, -11}, { -9, -7}, { -11, -7},
		{ -9, -5}, { -11, -5}
	},
};

IQSigal mapping::Qam256Map[4][64] =
{
	{
		{1, 1}, {3, 1}, {1, 3}, {3, 3},
		{7, 1}, {5, 1}, {7, 3}, {5, 3},
		{1, 7}, {3, 7}, {1, 5}, {3, 5},
		{7, 7}, {5, 7}, {7, 5}, {5, 5},

		{15, 1}, {13, 1}, {15, 3}, {13, 3},
		{9, 1},  {11, 1}, {9, 3},  {11, 3},
		{15, 7}, {13, 7}, {15, 5}, {13, 5},
		{9, 7},  {11, 7}, {9, 5},  {11, 5},

		{1, 15}, {3, 15}, {1, 13}, {3, 13},
		{7, 15}, {5, 15}, {7, 13}, {5, 13},
		{1, 9},  {3, 9},  {1, 11}, {3, 11},
		{7, 9},  {5, 9},  {7, 11}, {5, 11},

		{15, 15}, {13, 15}, {15, 13}, {13, 13},
		{9, 15}, {11, 15}, {9, 13}, {11, 13},
		{15, 9}, {13, 9}, {15, 11}, {13, 11},
		{9, 9},  {11, 9}, {9, 11}, {11, 11}
	},

	{
		{1, -1}, {1, -3}, {3, -1}, {3, -3},
		{1, -7}, {1, -5}, {3, -7}, {3, -5},
		{7, -1}, {7, -3}, {5, -1}, {5, -3},
		{7, -7}, {7, -5}, {5, -7}, {5, -5},

		{1, -15}, {1, -13}, {3, -15}, {3, -13},
		{1, -9},  {1, -11}, {3, -9},  {3, -11},
		{7, -15}, {7, -13}, {5, -15}, {5, -13},
		{7, -9},  {7, -11}, {5, -9},  {5, -11},

		{15, -1}, {15, -3}, {13, -1}, {13, -3},
		{15, -7}, {15, -5}, {13, -7}, {13, -5},
		{9, -1},  {9, -3},  {11, -1}, {11, -3},
		{9, -7},  {9, -5},  {11, -7}, {11, -5},

		{15, -15}, {15, -13}, {13, -15}, {13, -13},
		{15, -9}, {15, -11}, {13, -9}, {13, -11},
		{9, -15}, {9, -13}, {11, -15}, {11, -13},
		{9, -9},  {9, -11}, {11, -9}, {11, -11}
	},

	{
		{ -1, 1}, { -1, 3}, { -3, 1}, { -3, 3},
		{ -1, 7}, { -1, 5}, { -3, 7}, { -3, 5},
		{ -7, 1}, { -7, 3}, { -5, 1}, { -5, 3},
		{ -7, 7}, { -7, 5}, { -5, 7}, { -5, 5},

		{ -1, 15}, { -1, 13}, { -3, 15}, { -3, 13},
		{ -1, 9},  { -1, 11}, { -3, 9},  { -3, 11},
		{ -7, 15}, { -7, 13}, { -5, 15}, { -5, 13},
		{ -7, 9},  { -7, 11}, { -5, 9},  { -5, 11},

		{ -15, 1}, { -15, 3}, { -13, 1}, { -13, 3},
		{ -15, 7}, { -15, 5}, { -13, 7}, { -13, 5},
		{ -9, 1},  { -9, 3},  { -11, 1}, { -11, 3},
		{ -9, 7},  { -9, 5},  { -11, 7}, { -11, 5},

		{ -15, 15}, { -15, 13}, { -13, 15}, { -13, 13},
		{ -15, 9}, { -15, 11}, { -13, 9}, { -13, 11},
		{ -9, 15}, { -9, 13}, { -11, 15}, { -11, 13},
		{ -9, 9},  { -9, 11}, { -11, 9}, { -11, 11}
	},

	{
		{ -1, -1}, { -3, -1}, { -1, -3}, { -3, -3},
		{ -7, -1}, { -5, -1}, { -7, -3}, { -5, -3},
		{ -1, -7}, { -3, -7}, { -1, -5}, { -3, -5},
		{ -7, -7}, { -5, -7}, { -7, -5}, { -5, -5},

		{ -15, -1}, { -13, -1}, { -15, -3}, { -13, -3},
		{ -9, -1},  { -11, -1}, { -9, -3},  { -11, -3},
		{ -15, -7}, { -13, -7}, { -15, -5}, { -13, -5},
		{ -9, -7},  { -11, -7}, { -9, -5},  { -11, -5},

		{ -1, -15}, { -3, -15}, { -1, -13}, { -3, -13},
		{ -7, -15}, { -5, -15}, { -7, -13}, { -5, -13},
		{ -1, -9},  { -3, -9},  { -1, -11}, { -3, -11},
		{ -7, -9},  { -5, -9},  { -7, -11}, { -5, -11},

		{ -15, -15}, { -13, -15}, { -15, -13}, { -13, -13},
		{ -9, -15}, { -11, -15}, { -9, -13}, { -11, -13},
		{ -15, -9}, { -13, -9}, { -15, -11}, { -13, -11},
		{ -9, -9},  { -11, -9}, { -9, -11}, { -11, -11}
	},
};
IQSigal mapping::MapQamIQ(DvbMdlType_e tp, u8 quadrant, u8 rotation)
{
	IQSigal ret;
	switch(tp)
	{
		case QPSK:
		{
			if(quadrant >= 4)
			{
				ERR("param error");
			}
			ret = QPSKMap[quadrant];
		}
		break;
		case QAM_16:
		{
			if((quadrant >= 4) || (rotation >= 4))
			{
				ERR("param error");
			}
			ret = Qam16Map[quadrant][rotation];
		}
		break;
		case QAM_32:
		{
			if((quadrant >= 4) || (rotation >= 8))
			{
				ERR("param error");
			}
			ret = Qam32Map[quadrant][rotation];
		}
		break;
		case QAM_64:
		{
			if((quadrant >= 4) || (rotation >= 16))
			{
				ERR("param error");
			}
			ret = Qam64Map[quadrant][rotation];
		}
		break;
		case QAM_128:
		{
			if((quadrant >= 4) || (rotation >= 32))
			{
				ERR("param error");
			}
			ret = Qam128Map[quadrant][rotation];
		}
		break;
		case QAM_256:
		{
			if((quadrant >= 4) || (rotation >= 64))
			{
				ERR("param error");
			}
			ret = Qam256Map[quadrant][rotation];
		}
		break;
		default:
			ERR("param error tp 0x%x .", tp);
			break;
	}
	return ret;
}


bool diffencoder::EncoderIQ(u8 A, u8 B, u8 *I, u8   *Q)
{
	*I = (!(A ^ B)) & (A ^ Last_I) | (A ^ B) & (A ^ Last_Q);
	*Q = (!(A ^ B)) & (B ^ Last_Q) | (A ^ B) & (B ^ Last_I);
	Last_I = *I;
	Last_Q = *Q;
	return true;
}







